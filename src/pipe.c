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

//bool PC_branch = false;

void IF_op(void){
	//printf(">>>>>>> IF\n");
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

	return;
}
void ID_op(void){
	//printf(">>>>>>> ID\n");
	int instruction = ifid_reg.instruction;
	unsigned int rs = InstructionElement(instruction, RS);
	unsigned int rd = InstructionElement(instruction, RD);
	unsigned int rt = InstructionElement(instruction, RT);
	unsigned int op = InstructionElement(instruction, OP);
	int signextended = InstructionElement(instruction, IMM);
	if (signextended&0x00008000){
		printf("yes, SE\n");
		signextended = signextended|0xffff0000;
	} else {
		printf("no, SE\n");
		signextended = signextended&0x0000ffff;
	}
	idex_shadow.signextended = signextended;

	int read1;
	int read2;

	idex_shadow.rs = rs;
	idex_shadow.rd = rd;
	idex_shadow.rt = rt;
	idex_shadow.opCode = op;

	// Set the read Registers
	idex_shadow.read_reg1 = (int)reg_file[rs];
	idex_shadow.read_reg2 = (int)reg_file[rt];

	read1 = (int)reg_file[rs];
  	read2 = (int)reg_file[rt];

  	//TODO: Do ID forwarding here

	CTL_Perform(op, read1, read2, signextended);

	return;
}

void EX_op(void){
	//printf(">>>>>>> EX\n");

	int val1 = idex_reg.read_reg1;
	int val2 = idex_reg.read_reg2;

	//TODO: forward EX here

	//Continue to pass on control to next pipeline registers
	exmem_shadow.RegWrite = idex_reg.RegWrite;
	exmem_shadow.MemtoReg = idex_reg.MemtoReg;
	exmem_shadow.Branch = idex_reg.Branch;
	exmem_shadow.MemRead = idex_reg.MemRead;
	exmem_shadow.MemWrite = idex_reg.MemWrite;
	exmem_shadow.dataToMem = val2;

	if (idex_reg.RegDst) {
		exmem_shadow.rd = idex_reg.rd;
	}

	else {
		exmem_shadow.rd = idex_reg.rt;
	}

	if (idex_reg.ALUSrc){
		val2 = idex_reg.signextended;
	}

	ALU_Perform(val1, val2, idex_reg.ALUOp);

	return;
}
void MEM_op(void){
	// Places result into MEMWB_shadow
	// And probably also stuff into memory[]
	//printf(">>>>>>> MEM\n");
}
void WB_op(void){
    // Places result into IDEX_shadow (I think?)
    //printf(">>>>>>> WB\n");
}

void move_shadows_to_reg(void){
	// set each Stage_reg to be respective Stage_shadow	
	//printf(">>>>>>> Shadows 2 Reg\n");
	ifid_reg = ifid_shadow;
	idex_reg = idex_shadow;
	exmem_reg = exmem_shadow;
	memwb_reg = memwb_shadow;
}

void CTL_Perform(unsigned int opCode, int regVal1, int regVal2, unsigned int extendedValue) {
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

void ALU_Perform(int val1, int val2, alu_op operation) {
	int result = 0;
	int zero = 0;
	unsigned int shamt;

	printf("\nALU OPERATION: ");
	shamt = InstructionElement(idex_reg.signextended, SHAMT);
	if(idex_reg.ALUOp == ALUOP_LWSW) {
		result = val1 + val2;
		printf("Executing LWSW. Res: (%d) \n", result);
	}
	else if(idex_reg.ALUOp == ALUOP_R) {
		if(idex_reg.opCode) {
			printf(" I with op: (0x%x)\n", idex_reg.opCode);
			switch(idex_reg.opCode) {
					case I_ADDI:
						result = (int)val1 + (int)val2;
						break;
					case I_ADDIU:
						result = val1 + val2;
						break;
					case I_ANDI:
						result = val1 & (int)(val2&IMM_MASK);
						break;
					case I_ORI:
						result = val1 | (int)(val2&IMM_MASK);
						break;
					case I_SLTI:
						if (val1<val2) result = 1;
						else result = 0;
						break;
					case I_SLTIU:
						if (val1<val2) result = 0;
						else result = 1;
						break;
					default:
						printf("Did not recognize the op code for EX...");
			}
		}
		else {
			// R
			unsigned int func = InstructionElement(idex_reg.signextended, FUNCT);
			printf(" R with func: (0x%x)\n", func);
			switch (func) {
				case R_ADD:
					result = val1 + val2;
					break;
				case R_ADDU:
					result = val1 + val2;
					break;
				case R_SUB:
					result = val1 - val2;
					break;
				case R_AND:
					result = val1 & val2;
					break;
					// OR and MOV
				case R_OR:
					result = val1 | val2;
					break;
				case R_XOR:
					result = val1^val2;
				case R_SLT:
					if (val1<val2) result = 1;
					else result = 0;
					break;
				case R_SLTU:
					if (val1<val2) result = 1;
					else result = 0;
					break;
				case R_SLL:
					result = val1<<shamt;
					break;
				case R_SRA:
					result = val1>>shamt;
					break;
				case R_SRL:
					result = val1>>shamt;
					break;
				case R_NOR:
					result = ~(val1|val2);
					break;
				case R_MOVN:
					if(val2 != 0){
						result = val1;
					}
					break;
				default:
					printf("Unknown R command... (%d)", func);
			}
		}
	}
	exmem_shadow.zero = zero;
	exmem_shadow.alu_Result = result;
}

void step(void){
	move_shadows_to_reg();
	IF_op();
	ID_op();
	EX_op();
	MEM_op();
	WB_op();
	//print_pipelineReg(IFID);
	//print_pipelineReg(IDEX);
	//print_pipelineReg(EXMEM);
}


