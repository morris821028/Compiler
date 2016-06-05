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
#include <algorithm>
using namespace llvm;

extern FunctionPass *(*loadableFunction)(void);
extern ModulePass   *(*loadableFunction2)(void);
// extern ModulePass	*llvm::createMyTestPass2(void);

std::map<const Function*, std::set<const Function*>, std::less<const Function*>> MyCG;
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

bool X86TestPass::runOnMachineFunction(MachineFunction &MF) {
	dbgs() << "Fn: " << MF.getFunction()->getName() << "\n";
	dbgs() << "\t\t" << MF.getFunction() << "\n";
	const Function* fptr = MF.getFunction();
	for (auto &MBB: MF) {
		for (auto &MI: MBB) {
			for (auto &MO: MI.operands()) {
				if (!MO.isReg())
					continue;
				unsigned r = MO.getReg();
				if (r >= X86::R8B && r <= X86::R15W)
					dbgs() << "reg " << r << "\n";
			}
		}
	}
	return false;
}

FunctionPass *createMyTestPass() {
	return new X86TestPass();
}

void __attribute__((constructor)) init() {
	loadableFunction = createMyTestPass;
	loadableFunction2 = createMyTestPass2;
}

