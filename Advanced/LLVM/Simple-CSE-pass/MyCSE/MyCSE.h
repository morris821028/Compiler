#include "llvm/Pass.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/Support/raw_ostream.h" 

using namespace llvm;

class MyCSE : public BasicBlockPass {
	public:
		static char ID;
		MyCSE(): BasicBlockPass(ID) {}
		bool runOnBasicBlock(BasicBlock &BB) override;
};
