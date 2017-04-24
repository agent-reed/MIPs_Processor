//
//  pipe.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include "pipe.h"
#include "tools.h"

void IF_op(void){
	printf(">>>>>>> IF\n");
	// if (PC_branch) {
	// 	PC = PC_one;
	// 	PC_branch = !PC_branch;
	// }
	// else {
	// 	PC = PC_zero;
	// }
	PC = PC_zero;
	PC_zero = PC+1;
	ifid_shadow.instruction = memory[PC_zero];
	ifid_shadow.nextPC = PC_zero;
}
void ID_op(void){
	printf(">>>>>>> ID\n");
	int instruction = ifid_reg.instruction;
	unsigned int rs = InstructionElement(instruction, RS);
	unsigned int rd = InstructionElement(instruction, RD);
	unsigned int rt = InstructionElement(instruction, RT);
	unsigned int op = InstructionElement(instruction, OP);
	int extendedValue = InstructionElement(instruction, IMM);
	int regVal1;
	int regVal2;

	idex_shadow.rs = rs;
	idex_shadow.rd = rd;
	idex_shadow.rt = rt;
	idex_shadow.opCode = op;

	// Set the read Registers
	idex_shadow.read_reg1 = (int)reg_file[rs];
	idex_shadow.read_reg2 = (int)reg_file[rt];

	regVal1 = (int)reg_file[rs];
  	regVal2 = (int)reg_file[rt];

	CTL_UnitOperation(op, regVal1, regVal2, extendedValue);

}
void EX_op(void){
	// Places result into EXMEM_shadow
	printf(">>>>>>> EX\n");
}
void MEM_op(void){
	// Places result into MEMWB_shadow
	// And probably also stuff into memory[]
	printf(">>>>>>> MEM\n");
}
void WB_op(void){
    // Places result into IDEX_shadow (I think?)
    printf(">>>>>>> WB\n");
}

void move_shadows_to_reg(void){
	// set each Stage_reg to be respective Stage_shadow	
	printf(">>>>>>> Shadows 2 Reg\n");
	ifid_reg = ifid_shadow;
	idex_reg = idex_shadow;
	exmem_reg = exmem_shadow;
	memwb_reg = memwb_shadow;
}

void CTL_UnitOperation(unsigned int opCode, int regVal1, int regVal2, unsigned int extendedValue) {
	unsigned int jImm;
	unsigned int msb;
	idex_shadow.ALUOp = ALUOP_NOP;
  if(ifid_reg.instruction ==  0) return; //Do nothing for case of NOP
	switch(opCode) {

	// case R-format
		case 0x0: {
			unsigned int func = extendedValue&FUNCT_MASK;
			switch (func) {
				case J_R:
					PC_one = regVal1;
					PC_branch = true;
					break;

				default:
					// Decoded R
					//if(!getPartNum(extendedValue, PART_SHM)) break;
					idex_shadow.RegWrite= true;
					idex_shadow.RegDst = true;
					idex_shadow.ALUOp = ALUOP_R;
					break;
			}
			break;
		}
		// SPECIAL BEGIN
		// case LW
		// case 0x20|I_LB:
		//   // idex_shadow.dataLen = DLEN_B;

		//   idex_shadow.RegWrite = true;
		// 	idex_shadow.MemtoReg = true;
		// 	idex_shadow.MemRead = true;
		// 	idex_shadow.ALUSrc = true;
		// 	idex_shadow.ALUOp = ALUOP_LWSW;
		// 	break;
  //   case 0x21|I_LH:
  //     // idex_shadow.dataLen = DLEN_HW;

		//   idex_shadow.RegWrite = true;
		// 	idex_shadow.MemtoReg = true;
		// 	idex_shadow.MemRead = true;
		// 	idex_shadow.ALUSrc = true;
		// 	idex_shadow.ALUOp = ALUOP_LWSW;
      break;
    case 0x23|I_LW:
			idex_shadow.RegWrite = true;
			idex_shadow.MemtoReg = true;
			idex_shadow.MemRead = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.ALUOp = ALUOP_LWSW;
			break;
    case 0x24|I_LBU:
      // idex_shadow.dataLen = DLEN_BU;

		  idex_shadow.RegWrite = true;
			idex_shadow.MemtoReg = true;
			idex_shadow.MemRead = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.ALUOp = ALUOP_LWSW;
      break;
		case 0x25|I_LHU:
      // idex_shadow.dataLen = DLEN_HWU;

		  idex_shadow.RegWrite = true;
			idex_shadow.MemtoReg = true;
			idex_shadow.MemRead = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.ALUOp = ALUOP_LWSW;
		  break;
		// case SW
		// case 0x28|I_SB:
		//   // idex_shadow.dataLen = DLEN_B;
		//   idex_shadow.MemWrite = true;
		// 	idex_shadow.ALUSrc = true;
		// 	idex_shadow.ALUOp = ALUOP_LWSW;
		//   break;
    case 0x29|I_SH:
      // idex_shadow.dataLen = DLEN_HW;
		  idex_shadow.MemWrite = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.ALUOp = ALUOP_LWSW;
      break;
    case 0x2b|I_SW:
			idex_shadow.MemWrite = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.ALUOp = ALUOP_LWSW;
			break;
		// case Branch
		case 0x04|I_BEQ:
			if(regVal1 != regVal2) break;
			PC_branch = true;
			PC_one = extendedValue + ifid_reg.nextPC;
			break;
		case 0x05|I_BNE:
			if(regVal1 == regVal2) break;
			PC_branch = true;
			PC_one = extendedValue+ifid_reg.nextPC;
			break;
   //  case 0x06|I_BLEZ:
			// if(regVal1 > regVal2) break;
			// PC_branch = true;
			// PC_one = extendedValue+ifid_reg.nextPC;
			// break;
   //  case 0x07|I_BGTZ:
   //    if(regVal1 <= 0) break;
			// PC_branch = true;
			// PC_one = extendedValue+ifid_reg.nextPC;
   //    break;
		// case J-format
		case 0x3|J_JAL:
			jImm = (ifid_reg.instruction&0x03ffffff)<<2;
			msb = ((ifid_reg.nextPC-1)<<2)&0xf0000000;
			PC_one = (jImm|msb)>>2;
			PC_branch = true;
			reg_file[31] = (ifid_reg.nextPC + 1);
			break;
		case 0x2|J_J:
			jImm = (ifid_reg.instruction&0x03ffffff)<<2;
			msb = ((ifid_reg.nextPC-1)<<2)&0xf0000000;
			PC_one = (jImm|msb)>>2;
			PC_branch = true;
			break;
    case 0x1:
      switch(idex_shadow.rt) {
        case 0x0:
          //bltz
          if(regVal1 < 0) {
          PC_branch = true;
          PC_one = extendedValue+ifid_reg.nextPC;
          }
          break;
        case 0x1:
          // bgez
          if(regVal1 >= 0) {
            PC_branch = true;
            PC_one = extendedValue+ifid_reg.nextPC;
          }
          break;
        case 0x10:
          // bltzal
          if(regVal1 < 0) {
            reg_file[31] = (ifid_reg.nextPC+1);
            PC_branch = true;
            PC_one = extendedValue+ifid_reg.nextPC;
          }
        break;
        case 0x11:
          // bgezal
          if(regVal1 >= 0) {
            reg_file[31] = (ifid_reg.nextPC+1);
            PC_branch = true;
            PC_one = extendedValue+ifid_reg.nextPC;
          }
        break;
        }
      break;
   //  case 0x1f|I_SEB:
   //    idex_shadow.rd = getPartNum(ifid_reg.instruction, PART_RD);
			// idex_shadow.RegWrite = true;
			// idex_shadow.ALUOp = ALUOP_R;
			// idex_shadow.RegDst = true;
      break;
    // SPECIAL END
		default:
			// I
			idex_shadow.RegWrite = true;
			idex_shadow.ALUOp = ALUOP_R;
			idex_shadow.ALUSrc = true;
			break;
	}
}

void step(void){
	move_shadows_to_reg();
	print_pipelineReg(IFID);
	print_pipelineReg(IDEX);
	IF_op();
	ID_op();
	EX_op();
	MEM_op();
	WB_op();
}


