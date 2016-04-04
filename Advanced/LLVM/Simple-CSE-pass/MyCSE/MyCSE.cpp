#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/InstIterator.h" // inst_range
#include "llvm/IR/Instructions.h" // TerminatorInst
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
using namespace llvm;

#include "MyCSE.h"
#include <map>
#include <set>
#include <vector>
#include <stdio.h>

char MyCSE::ID = 0;
static RegisterPass<MyCSE> X("myMagicCSE", "My CSE Pass", false, false);

#define MDEBUG

namespace {
	struct Expression {
		uint32_t opcode;
		Type *type;
		SmallVector<uint32_t, 2> varargs;
		Expression(uint32_t o = ~2U) : opcode(o) {}
		bool operator==(const Expression &other) const {
			if (opcode != other.opcode)
				return false;
			if (opcode == ~0U || opcode == ~1U)
				return true;
			if (type != other.type)
				return false;
			if (varargs != other.varargs)
				return false;
			return true;
		}   
		friend hash_code hash_value(const Expression &Value) {
			return hash_combine(Value.opcode, Value.type,
					hash_combine_range(Value.varargs.begin(),
						Value.varargs.end()));
		}

	};
	class ValueTable {
		DenseMap<Value*, uint32_t> valNumbering;
		DenseMap<Expression, uint32_t> exprNumbering;
		std::map<uint32_t, Instruction*> invExprNumbering;
		uint32_t nextValNumber;
		public:
		ValueTable(): nextValNumber(1) {
		}
		uint32_t lookup_or_add(Value *V);	// if not found, create new one
		Expression create_expression(Instruction *I);
		void clear();
		int replace(Instruction *I, BasicBlock *BB,
				BasicBlock::iterator BBit);
	};
}
namespace llvm {
	template <> struct DenseMapInfo<Expression> {
		static inline Expression getEmptyKey() {
			return ~0U;
		}

		static inline Expression getTombstoneKey() {
			return ~1U;
		}

		static unsigned getHashValue(const Expression e) {
			using llvm::hash_value;
			return static_cast<unsigned>(hash_value(e));
		}
		static bool isEqual(const Expression &LHS, const Expression &RHS) {
			return LHS == RHS;
		}
	};
}
void ValueTable::clear() {
	valNumbering.clear();
	exprNumbering.clear();
	nextValNumber = 1;
}
int ValueTable::replace(Instruction *I, BasicBlock *BB, 
					BasicBlock::iterator BBit) {
	BinaryOperator *bop;
	Value *L, *R;

	bop = dyn_cast<BinaryOperator>(I);
	L = bop->getOperand(0);
	R = bop->getOperand(1);

	Expression expr = create_expression(I);
/*
	I->dump();
	printf("---> %u %u %u %u\n", expr.opcode, expr.type, expr.varargs[0], expr.varargs[1]);
*/
	DenseMap<Expression, uint32_t>::iterator eit = exprNumbering.find(expr);
	if (eit != exprNumbering.end()) {
		IRBuilder<> Builder(I->getParent(), ++BasicBlock::iterator(I));
		Instruction *J = invExprNumbering[eit->second];
		assert(dyn_cast<Value>(J) && "Insturction -> Value");
		// AllocaInst *newAlloc = Builder.CreateAlloca(J->getType());
//		newAlloc->dump();
//		Value *newValue = ???????;
		//StoreInst *newStore = Builder.CreateStore(I, J);
//		LoadInst *newLoad = Builder.CreateLoad(????);
//		str->takeName(I);
		puts("duplicate !!!!!!!!!");
		I->replaceAllUsesWith(J);
		I->dump();
		J->dump();
	//	ReplaceInstWithInst(I, J);
		return 1;
	} else {
		exprNumbering[expr] = nextValNumber;
		invExprNumbering[nextValNumber] = I;
		nextValNumber++;
		return 0;
	}
}
uint32_t ValueTable::lookup_or_add(Value *V) {
	DenseMap<Value*, uint32_t>::iterator vit = valNumbering.find(V);
	if (vit != valNumbering.end())
		return vit->second;
	if (!isa<Instruction>(V)) {
		/*
		printf("loopup or add %u %d ???? \n", V, V->hasName());
		if (V->hasName()) {
			dbgs() << V->getName() << '\n';
			printf("Set %u %d\n", V, nextValNumber);
		}
		*/
		valNumbering[V] = nextValNumber;
		return nextValNumber++;
	}
	Instruction* I = cast<Instruction>(V);
	Expression exp;
	switch (I->getOpcode()) {
		case Instruction::Add:
		case Instruction::FAdd:
		case Instruction::Sub:
		case Instruction::FSub:
		case Instruction::Mul:
		case Instruction::FMul:
		case Instruction::UDiv:
		case Instruction::SDiv:
		case Instruction::FDiv:
		case Instruction::URem:
		case Instruction::SRem:
		case Instruction::FRem:
		case Instruction::Shl:
		case Instruction::LShr:
		case Instruction::AShr:
		case Instruction::And:
		case Instruction::Or:
		case Instruction::Xor:
		case Instruction::ICmp:
		case Instruction::FCmp:
		case Instruction::Trunc:
		case Instruction::ZExt:
		case Instruction::SExt:
		case Instruction::FPToUI:
		case Instruction::FPToSI:
		case Instruction::UIToFP:
		case Instruction::SIToFP:
		case Instruction::FPTrunc:
		case Instruction::FPExt:
		case Instruction::PtrToInt:
		case Instruction::IntToPtr:
		case Instruction::BitCast:
		case Instruction::Select:
		case Instruction::ExtractElement:
		case Instruction::InsertElement:
		case Instruction::ShuffleVector:
		case Instruction::InsertValue:
		case Instruction::GetElementPtr:
			exp = create_expression(I);
			break;
		default:
			valNumbering[V] = nextValNumber;
			return nextValNumber++;
	}

	uint32_t& e = exprNumbering[exp];
	if (!e) e = nextValNumber++;
	invExprNumbering[e] = I;
	valNumbering[V] = e;
	return e;
}
Expression ValueTable::create_expression(Instruction *I) {
	Expression e;
	e.type = I->getType();
	e.opcode = I->getOpcode();
	for (auto op = I->op_begin(); op != I->op_end(); op++)
		e.varargs.push_back(lookup_or_add(*op));
	return e;
}
bool MyCSE::runOnBasicBlock(BasicBlock &BB) {
	ValueTable vtable;
	bool Changed = false;
	for (BasicBlock::iterator it = BB.begin(); it != BB.end(); it++) {
		Instruction *Inst = &(*it);
		BinaryOperator *bop;
		if ((bop = dyn_cast<BinaryOperator>(Inst))) {
			if (vtable.replace(Inst, &BB, it))
				Changed = true;
		} else {
			//		printf("fflush\n");
		}
	}

	return true;
}
