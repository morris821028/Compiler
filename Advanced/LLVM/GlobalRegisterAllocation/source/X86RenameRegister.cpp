//===-------- X86RenameRegister.cpp - rename register ---------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// test
//
//===----------------------------------------------------------------------===//

#include <algorithm>

#include "X86.h"
#include "X86InstrInfo.h"
#include "X86Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetInstrInfo.h"

using namespace llvm;

#define DEBUG_TYPE "x86-rename-register"

STATISTIC(RenameRegister, "x86 rename register");

namespace {

  struct RenameReg : public MachineFunctionPass {
    static char ID;
    RenameReg() : MachineFunctionPass(ID) {}

    bool runOnMachineFunction(MachineFunction &MF) override;

    const char *getPassName() const override {
      return "X86 Rename Register";
    }

  private:
  };

  char RenameReg::ID = 0;
}

FunctionPass *llvm::createX86RenameRegister() {
  return new RenameReg();
}

bool RenameReg::runOnMachineFunction(MachineFunction &MF) {
	for (auto &MBB: MF) {
		for (auto &MI: MBB) {
			for (auto &MO: MI.operands()) {
				if (!MO.isReg())
					continue;
				unsigned r = MO.getReg();
				switch (r) {
					case X86::RDX:
						MO.setReg(X86::RBX); break;
					case X86::RBX:
						MO.setReg(X86::RDX); break;
					case X86::EDX:
						MO.setReg(X86::EBX); break;
					case X86::EBX:
						MO.setReg(X86::EDX); break;
					case X86::DX:
						MO.setReg(X86::BX); break;
					case X86::BX:
						MO.setReg(X86::DX); break;
					case X86::DH:
						MO.setReg(X86::BH); break;
					case X86::BH:
						MO.setReg(X86::DH); break;
					case X86::DL:
						MO.setReg(X86::BL); break;
					case X86::BL:
						MO.setReg(X86::DL); break;

				}
			}
		}
	}
	return true;
}
