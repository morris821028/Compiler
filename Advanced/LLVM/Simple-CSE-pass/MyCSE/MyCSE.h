#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h" 

using namespace llvm;

class MyCSE : public FunctionPass {
	public:
		static char ID;
		MyCSE(): FunctionPass(ID) {}
		bool runOnFunction(Function &F) override;
};
