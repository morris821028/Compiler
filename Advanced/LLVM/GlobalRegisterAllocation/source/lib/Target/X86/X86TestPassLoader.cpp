#include <algorithm>

#include "X86.h"
#include "llvm/Pass.h"

using namespace llvm;

FunctionPass *(*loadableFunction)();

FunctionPass *llvm::createX86TestPass() {
  return loadableFunction();
}

ModulePass *(*loadableFunction2)();

ModulePass *llvm::createX86TestPass2() {
  return loadableFunction2();
}

