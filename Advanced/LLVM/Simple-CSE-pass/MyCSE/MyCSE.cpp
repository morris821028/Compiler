#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h" // inst_range
#include "llvm/IR/Instructions.h" // TerminatorInst
using namespace llvm;

#include "MyCSE.h"
#include <set>
#include <vector>
#include <stdio.h>

char MyCSE::ID = 0;
static RegisterPass<MyCSE> X("myMagicCSE", "My CSE Pass", false, false);

#define MDEBUG

bool MyCSE::runOnFunction(Function &F) {
	return false;
}
