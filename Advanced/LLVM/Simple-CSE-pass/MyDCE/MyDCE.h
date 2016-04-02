#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h" 

using namespace llvm;

class MyDCE : public FunctionPass {
	public:
		static char ID;
		MyDCE(): FunctionPass(ID) {}
		bool runOnFunction(Function &F) override;
};
