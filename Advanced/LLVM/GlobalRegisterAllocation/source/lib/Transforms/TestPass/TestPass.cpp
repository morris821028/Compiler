#include "../lib/Target/X86/X86.h"
#include "../lib/Target/X86/X86InstrInfo.h"
#include "../lib/Target/X86/X86Subtarget.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Debug.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetInstrInfo.h"
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace llvm;

extern FunctionPass *(*loadableFunction)(void);
extern ModulePass   *(*loadableFunction2)(void);
// extern ModulePass	*llvm::createMyTestPass2(void);

std::map<const Function*, std::set<const Function*>, std::less<const Function*>> MyCG;
std::map<const Function*, std::set<const Function*>, std::less<const Function*>> invMyCG;

namespace {
    class X86TestPass2 : public ModulePass {
        public:
            static char ID;
            X86TestPass2() : ModulePass(ID) { MyCG.clear(); }

            void getAnalysisUsage(AnalysisUsage &AU) const override {
                AU.addRequired<CallGraphWrapperPass>();
            }

            bool runOnModule(Module &MF) override;
            const char *getPassName() const override {
                return "X86TestPass2";
            }
    };
    char X86TestPass2::ID = 0;
}

bool X86TestPass2::runOnModule(Module &MF) {
    dbgs() << "run on module" << "\n";
    CallGraph &CG = getAnalysis<CallGraphWrapperPass>().getCallGraph();
    CG.dump();
    Module::FunctionListType &functions = MF.getFunctionList();
    for (Module::FunctionListType::iterator it = functions.begin(),
        it_end = functions.end(); it != it_end; it++) {
        Function &func = *it;
        // dbgs() << func.getName() << &func << "\n";
        CallGraphNode *node = CG[&func];
        dbgs() << func.getName() << "\t" << node->getFunction() << "\n";
        for (CallGraphNode::iterator cit = node->begin(), cit_end = node->end();
            cit != cit_end; cit++) {
            dbgs() << "\tcall\t" << cit->second->getFunction()->getName()
                    << "\t" << cit->second->getFunction() << "\n";
			MyCG[node->getFunction()].insert(cit->second->getFunction());
			invMyCG[cit->second->getFunction()].insert(node->getFunction());
        }
        func.getCallingConv();
    }
    return false;
}

ModulePass *llvm::createMyTestPass2() {
    dbgs() << "create my test pass2" << "\n";
    return new X86TestPass2();
}


namespace {
	class X86TestPass : public MachineFunctionPass {
		public:
			static char ID;
			X86TestPass() : MachineFunctionPass(ID) { }

			bool runOnMachineFunction(MachineFunction &MF) override;

			const char *getPassName() const override {
				return "X86TestPass";
			}
	};
//	char X86Test2Pass::ID = 0;
	char X86TestPass::ID = 0;
}

int AllUsedReg[2048] = {};
static unsigned X86MapGenRegToSin(unsigned r) {
	switch (r) {
		case X86::R8 ... X86::R15: return r;
		case X86::R8B ... X86::R15B: return r - X86::R8B + X86::R8;
		case X86::R8D ... X86::R15D: return r - X86::R8D + X86::R8;
		case X86::R8W ... X86::R15W: return r - X86::R8W + X86::R8;
	}
	dbgs() << "Error" << "\n";
}
static unsigned X86MapGenRegToSSE(unsigned u, unsigned a, unsigned b) {
	// register a -> b
	switch (u) {
		case X86::R8 ... X86::R15: 
		case X86::R8B ... X86::R15B: 
		case X86::R8D ... X86::R15D: 
		case X86::R8W ... X86::R15W: return u + (b - a);
	}
	dbgs() << "Error" << "\n";
}

bool X86TestPass::runOnMachineFunction(MachineFunction &MF) {
	dbgs() << "Fn: " << MF.getFunction()->getName() << "\n";
//	dbgs() << "\t\t" << MF.getFunction() << "\n";
	const Function* fptr = MF.getFunction();
	std::vector<MachineInstr*> A;
	int LocalUseRegU[2048] = {};
	int LocalUseRegP[2048] = {};
	for (auto &MBB: MF) {
		for (auto &MI: MBB) {
			bool has = false;
			int opcode = MI.getOpcode();
			for (auto &MO: MI.operands()) {
				if (!MO.isReg())
					continue;
				unsigned r = MO.getReg();
				if ((r >= X86::R8 && r <= X86::R15)
					|| (r >= X86::R8B && r <= X86::R15W)) {
					unsigned s = X86MapGenRegToSin(r);
					if ((opcode >= X86::POP16r && opcode <= X86::POPSS32) 
							|| (opcode >= X86::PUSH16i8 && opcode <= X86::PUSHi32)) {
						LocalUseRegP[s] = 1;
					} else {
						LocalUseRegU[s] = 1;
					}
/*
					dbgs() << MI.getOpcode() << " " << MI << "\n";
					dbgs() << "reg " << r << "\n";
					has = true;
*/
				}
			}
		}
	}
	
	int ReplaceReg[2048] = {};
	for (int i = X86::R8; i <= X86::R15; i++) {
		if (LocalUseRegP[i] == 0)
			continue;
		if (LocalUseRegU[i] == 0)
			continue;
		int has = 0;
		for (int j = X86::R8; j <= X86::R15; j++) {
			if (AllUsedReg[j] == 0) {
				AllUsedReg[j] = 1;
				ReplaceReg[i] = j;
				has = 1;
				break;
			}
		}
		if (!has)
			AllUsedReg[i] = 1;
	}
	
	std::set<MachineInstr*, std::less<MachineInstr*>> RemoveSet;
	for (int i = X86::R8; i <= X86::R15; i++) {
		if (ReplaceReg[i] == 0)
			continue;
		int u = i, v = ReplaceReg[i];
		dbgs() << u << " -------> " << v << "\n";
		for (auto &MBB: MF) {
			for (auto &MI: MBB) {
				bool has = false;
				int opcode = MI.getOpcode();
				for (auto &MO: MI.operands()) {
					if (!MO.isReg())
						continue;
				unsigned r = MO.getReg();
				if (!(r >= X86::R8 && r <= X86::R15)
					&& !(r >= X86::R8B && r <= X86::R15W))
					continue;
				unsigned s = X86MapGenRegToSin(r);
					if (s == u) {
						dbgs() << "Modify " << MI << "\n";
						MO.setReg(X86MapGenRegToSSE(r, u, v));
						dbgs() << "Modify " << MI << "\n";
						if ((opcode >= X86::POP16r && opcode <= X86::POPSS32) 
								|| (opcode >= X86::PUSH16i8 && opcode <= X86::PUSHi32)) {
							RemoveSet.insert(&MI);
						}
					}
				}
			}
		}
	}
	for (auto x : RemoveSet) {
		dbgs() << "Remove " << *x << "\n";
		x->removeFromBundle();
	}
	return true;
}

FunctionPass *createMyTestPass() {
	return new X86TestPass();
}

void __attribute__((constructor)) init() {
	loadableFunction = createMyTestPass;
	loadableFunction2 = createMyTestPass2;
}


