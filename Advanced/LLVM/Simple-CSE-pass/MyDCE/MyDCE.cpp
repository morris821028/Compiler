#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h" // inst_range
#include "llvm/IR/Instructions.h" // TerminatorInst
using namespace llvm;

#include "MyDCE.h"
#include <set>
#include <vector>
#include <stdio.h>

char MyDCE::ID = 0;
static RegisterPass<MyDCE> X("myMagicDCE", "My DCE Pass", false, false);

#define MDEBUG

bool MyDCE::runOnFunction(Function &F) {
/**
 * Mark-and-Sweep Algorithm
 */
	SmallVector<Instruction*, 128> Worklist;	// use as a stack
	SmallPtrSet<Instruction*, 128> Marked;

#ifdef MDEBUG	
	puts("\t\033[36mFind Critical Instruction\033[0m");
#endif

	for (Instruction &Inst : inst_range(F)) {
		if (Inst.mayHaveSideEffects() || Inst.isTerminator()) {
			Worklist.push_back(&Inst);
			Marked.insert(&Inst);

#ifdef MDEBUG
			Inst.dump();
			if (Inst.isTerminator())
				puts("\033[1;31m\tTerminatorInst \033[0m\n");
			if (Inst.mayHaveSideEffects())
				puts("\033[1;31m\tmayHaveSideEffects \033[0m\n");
#endif
		}
	}

#ifdef MDEBUG
	puts("\t\033[36mFind Dependency Instruction\033[0m");
#endif

	while (!Worklist.empty()) {
		Instruction &Inst = *Worklist.pop_back_val();
		for (auto op = Inst.op_begin(); op != Inst.op_end(); op++) {
			if (isa<Instruction>(op->get())) {
				Instruction &j = static_cast<Instruction&>(*(op->get()));
				if (Marked.count(&j) == 0) {
					Worklist.push_back(&j);
					Marked.insert(&j);

#ifdef MDEBUG
					j.dump();
#endif

				}
			}
		}
	}
	
	bool Changed = false;
	int EliminateCnt = 0;

#ifdef MDEBUG
	puts("Safe-Remove");
#endif

	for (Instruction &Inst : inst_range(F)) {
		if (Marked.count(&Inst) == 0) {
			Inst.dropAllReferences();
			Worklist.push_back(&Inst);
		}
	}

#ifdef MDEBUG
	puts("DEAD");
#endif

	for (auto Inst : Worklist) {

#ifdef MDEBUG
		Inst->dump();
#endif

		Inst->eraseFromParent();
		Changed = true;
		EliminateCnt++;
	}

#ifdef MDEBUG
	printf("\033[32m\t%s, #EliminateInst %d \033[0m\n", Changed ? "Changed" : "Unchanged", EliminateCnt);
#endif

	return Changed;
}
