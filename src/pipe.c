//
//  pipe.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include "pipe.h"

bool PC_branch = false;
int writeData;
int oldData;

bool lu_hazard = false;

void IF_op(void){
	//printf(">>>>>>> IF\n");
	if (PC == 0){
		printf("PC is Zero\n");
	}

	if (PC_branch) {
		printf("	BRANCH DETECTED\n");
		PC = PC_one<<2;
		PC_branch = !PC_branch;
		printf(" 	New PC: (%d)\n", PC>>2);
	}
	else {
		PC = PC_zero;
	}

	PC_zero = PC+4;
	//ifid_shadow.instruction = memory[PC];
	readCache(INST_CACHE, &ifid_shadow.instruction ,PC, word);
	ifid_shadow.nextPC = PC_zero>>2;

	return;
}

void ID_op(void){
	//printf(">>>>>>> ID\n");
	unsigned int instruction = ifid_reg.instruction;
	unsigned short rs = InstructionElement(instruction, RS);
	unsigned short rd = InstructionElement(instruction, RD);
	unsigned short rt = InstructionElement(instruction, RT);
	unsigned int op = InstructionElement(instruction, OP);
	unsigned int imm = InstructionElement(instruction, IMM);
	int signextended;
	printf("immediate: %x\n",imm);
	if (imm&0x00008000){
		//printf("	SignEx\n");
		signextended = imm|0xffff0000;
	} else {
		//printf("	notSignEx\n");
		signextended = imm&0x0000ffff;
	}
	idex_shadow.signextended = signextended;

	int read1;
	int read2;

	idex_shadow.rs = rs;
	idex_shadow.rd = rd;
	idex_shadow.rt = rt;
	idex_shadow.opCode = op;

	// Set the read Registers
	idex_shadow.read_reg1 = reg_file[rs];
	idex_shadow.read_reg2 = reg_file[rt];

	read1 = reg_file[rs];
  	read2 = reg_file[rt];

  	//TODO: Do ID forwarding here

	CTL_Perform(op, read1, read2, signextended);
	bool lu_true = load_use_handler();
	if (lu_true) {
		lu_hazard = true;
		printf("Found a Load Use Case -- Inserting Bubble\n");
	}

	return;
}

void EX_op(void){
	//printf(">>>>>>> EX\n");
	if (PC == 0){
		printf("PC is Zero\n");
		return;
	}
	int val1 = idex_reg.read_reg1;
	int val2 = idex_reg.read_reg2;

	printf("PRIOR TO FORWARD: val1(%d)  val2(%d)\n",val1, val2);

	bool found_forward = forward_handler(&val1, &val2);
	if (found_forward) {
		//printf("Found Forwarding Condition\n");
		//printf("writedata: %d\n",writeData );
		//printf("val1: %d  |  val2: %d\n",val1, val2 );
	}

	//Continue to pass on control to next pipeline registers
	exmem_shadow.RegWrite = idex_reg.RegWrite;
	exmem_shadow.MemtoReg = idex_reg.MemtoReg;
	exmem_shadow.Branch = idex_reg.Branch;
	exmem_shadow.MemRead = idex_reg.MemRead;
	exmem_shadow.MemWrite = idex_reg.MemWrite;
	exmem_shadow.dataToMem = val2;

	int rt = idex_reg.rt;
	int rs = idex_reg.rs;
	int rd = idex_reg.rd;

	if (idex_reg.RegDst) {
		exmem_shadow.rd = idex_reg.rd;
	}

	else {
		exmem_shadow.rd = idex_reg.rt;
	}

	if (idex_reg.ALUSrc){
		val2 = idex_reg.signextended;
	}

	ALU_Perform(val1, val2, rt, rs, rd, idex_reg.ALUOp);

	return;
}
void MEM_op(void){
	// Places result into MEMWB_shadow
	// And probably also stuff into memory[]
	//printf(">>>>>>> MEM\n");
	if (PC == 0){
		printf("PC is Zero\n");
		return;
	}
	unsigned int addr;
	unsigned short offset;
  	unsigned short shamt;
  	unsigned int data;

	// MEM Operation
	memwb_shadow.RegWrite = exmem_reg.RegWrite;
	memwb_shadow.MemtoReg = exmem_reg.MemtoReg;
	memwb_shadow.alu_Result = exmem_reg.alu_Result;
	memwb_shadow.rd = exmem_reg.rd;

	offset = exmem_reg.alu_Result;

	addr = ((unsigned int)exmem_reg.alu_Result); // >>2? For byte address
	//printf("Address: %u\n", addr);

	if(exmem_reg.MemWrite) {
		printf("Writing %d to memory[%u]\n",exmem_reg.dataToMem, addr);
		writeCache(DATA_CACHE, &exmem_reg.dataToMem, addr, word);

		//memory[addr] = exmem_reg.dataToMem;
		//printf("	MemWrite true -> Write to cache?\n");
		//Success = writeToCache(addr, (unsigned int)exmem_reg.dataToMem, offset, exmem_reg.dataLen);
	}

	//Temporary
	if(exmem_reg.MemRead) {
		memwb_shadow.memValue = memory[addr];
	}
	//printf("	Address: %u\n", addr);
	//printf("	Memvalue to write: 0x%x\n", memory[addr]);

	// if(exmem_reg.MemRead) {
	//  if(!readFromCache(CACHE_D, addr, &data)) {
 //      	  cacheMissed |= DCACHE_MISSED;
 //    	 }

 //    switch(exmem_reg.dataLen) {
 //        case DLEN_W:
 //          memwb_shadow.memValue = data;
 //          break;
 //        case DLEN_B:
 //          shamt = (3-offset)*8;
 //          data >>= shamt;
 //          data = data&0x80?(data|0xffffff00):data&0xff;
 //          memwb_shadow.memValue = data;
 //          break;
 //        case DLEN_BU:
 //          shamt = (3-offset)*8;
 //          data = data&0xff;
 //          memwb_shadow.memValue = data;
 //          break;
 //        case DLEN_HW:
 //          shamt = (1-offset)*16;
 //          data >>= shamt;
 //          data = data&0x8000?(data|0xffff0000):data&0xffff;
 //          memwb_shadow.memValue = data;
 //          break;
 //        case DLEN_HWU:
 //          shamt = (1-offset)*16;
 //          data = data&0xffff;
 //          memwb_shadow.memValue = data;
 //          break;
 //        default:
 //          printf("Error MEM_READ @ clock: %u, PC: %04d, instruction: [0x%x]\n",
 //                      clock, exmem_reg.progCounter, memory[exmem_reg.progCounter]);
 //          exit(1);
 //      }
}

void WB_op(void){
    //printf(">>>>>>> WB\n");
    if (PC == 0){
		printf("PC is Zero\n");
		return;
	}
    if (memwb_reg.MemtoReg){
    	writeData = memwb_reg.memValue;
    } else {
    	writeData = memwb_reg.alu_Result;
    }
    //printf("Write data@wb: %d\n", writeData);

    //printf("RegWrite: %d && reg.id: %d\n",memwb_reg.RegWrite, memwb_reg.rd);
	if(memwb_reg.RegWrite && (memwb_reg.rd != 0)) {
    	oldData = reg_file[memwb_reg.rd];
		reg_file[memwb_reg.rd] = writeData;
		printf("Writing %d to Reg: %u\n", writeData, memwb_reg.rd);
	}


	return;
}

void move_shadows_to_reg(void){
	// set each Stage_reg to be respective Stage_shadow	
	//printf(">>>>>>> Shadows 2 Reg\n");

	if(lu_hazard){
		ifid_reg = ifid_reg;
		PC_zero = PC_zero - 4;
		insert_bubble();
		idex_reg = idex_shadow;
		exmem_reg = exmem_shadow;
		memwb_reg = memwb_shadow;

		// Lower the flag
		lu_hazard = false;
		return;
	} 

	ifid_reg = ifid_shadow;
	idex_reg = idex_shadow;
	exmem_reg = exmem_shadow;
	memwb_reg = memwb_shadow;
}

void CTL_Perform(unsigned int opCode, int regVal1, int regVal2, unsigned int extendedValue) {
	unsigned int jImm;
	unsigned int msb;
	// Important to set these values false by default to avoid doing unwanted operations
	idex_shadow.MemWrite = false;
	idex_shadow.ALUSrc = false;
	idex_shadow.MemRead = false;
	idex_shadow.RegDst = false;
	idex_shadow.MemtoReg = false;
	idex_shadow.ALUOp = ALUOP_NOP;
  	if(ifid_reg.instruction ==  0){
  		idex_shadow.RegWrite = false;
  		idex_shadow.MemWrite = false;
  		idex_shadow.MemRead = false;
  		return;
  	}; //Do nothing for case of NOP
  	printf("CTL- OPCODE (%x)\n",opCode);
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
					printf("~default 0x0 opcode\n");
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
			idex_shadow.RegWrite = false;
			idex_shadow.ALUOp = ALUOP_LWSW;
      break;
    case 0x2b|I_SW:
			idex_shadow.MemWrite = true;
			idex_shadow.ALUSrc = true;
			idex_shadow.RegWrite = false;
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
			printf("ex: %d  npc: %d\n",extendedValue, ifid_reg.nextPC);
			PC_one = extendedValue+ifid_reg.nextPC;
			printf("PC_one %d\n",PC_one );
			break;
	    case 0x06:
				if(regVal1 > regVal2) break;
				PC_branch = true;
				PC_one = extendedValue+ifid_reg.nextPC;
				break;
	    case 0x07:
	      if(regVal1 <= 0) break;
				PC_branch = true;
				PC_one = extendedValue+ifid_reg.nextPC;
	      		break;
		case 0x3|J_JAL:
			printf("Contol Msg: J_JAL\n");
			jImm = (ifid_reg.instruction&0x03ffffff)<<2;
			msb = ((ifid_reg.nextPC-1)<<2)&0xf0000000;
			PC_one = (jImm|msb)>>2;
			PC_branch = true;
			reg_file[31] = (ifid_reg.nextPC + 1);
			break;
		case 0x2|J_J:
			printf("Contol Msg: J_J\n");
			jImm = (ifid_reg.instruction&0x03ffffff)<<2;
			msb = ((ifid_reg.nextPC-1)<<2)&0xf0000000;
			PC_one = (jImm|msb)>>2;
			PC_branch = true;
			break;
    case 0x1:
      switch(idex_shadow.rt) {
        case 0x0:
          //bltz
          printf("Contol Msg: bltz\n");
          if(regVal1 < 0) {
          PC_branch = true;
          PC_one = extendedValue+ifid_reg.nextPC;
          }
          break;
        case 0x1:
          // bgez
        printf("Contol Msg: bgez\n");
          if(regVal1 >= 0) {
            PC_branch = true;
            PC_one = extendedValue+ifid_reg.nextPC;
          }
          break;
        case 0x10:
        printf("Contol Msg: bltzal\n");
          // bltzal
          if(regVal1 < 0) {
            reg_file[31] = (ifid_reg.nextPC+1);
            PC_branch = true;
            PC_one = extendedValue+ifid_reg.nextPC;
          }
        break;
        case 0x11:
          // bgezal
        printf("Contol Msg: bgezal\n");
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

void ALU_Perform(int val1, int val2, int rt, int rs, int rd, alu_op operation) {
	int result = 0;
	unsigned int zero = 0;
	unsigned int shamt;
	printf("ALU ARGUMENTS: val1(%d) | val2(%d) | rt(%d) | rs(%d) | rd(%d) \n",val1, val2, rt, rs, rd );
	//printf("	ALU OPERATION:\n");
	shamt = InstructionElement(idex_reg.signextended, SHAMT);
	if(idex_reg.ALUOp == ALUOP_LWSW) {
		result = val1 + val2;
		printf("	Executing LWSW. Res: (%d) \n", result);
	}
	else if(idex_reg.ALUOp == ALUOP_R) {
		if(idex_reg.opCode) {
			printf("	I with op: (0x%x)\n", idex_reg.opCode);
			switch(idex_reg.opCode) {
					case I_ADDI:
						//result = (int)val1 + (int)val2;
						result = val1 + val2;
						break;
					case I_ADDIU:
						result = val1 + val2;
						break;
					case I_ANDI:
						//result = val1 & (int)(val2&IMM_MASK);
						result = val1 &(val2&IMM_MASK);
						break;
					case I_ORI:
						//result = val1 | (int)(val2&IMM_MASK);
						result = val1 |(val2&IMM_MASK);
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
						printf("	Did not recognize the op code for EX...");
			}
		} 
		else {
			// R
			unsigned int func = InstructionElement(idex_reg.signextended, FUNCT);
			printf("	R with func: (0x%x)\n", func);
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
					result = val2<<shamt;
					printf("Result of Shift: (%d)\n", result );
					break;
				case R_SRA:
					result = val2>>shamt;
					break;
				case R_SRL:
					result = val2>>shamt;
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

bool load_use_handler(void) {
	// Make sure there isnt a load/use case
	// If there is - stall the pipeline
	bool found = false;

	if (idex_reg.MemRead && (idex_reg.rt != 0) && ((idex_reg.rt == idex_shadow.rt) || (idex_reg.rt == idex_shadow.rs))){
      found = true;
    }
    return found;
}

void insert_bubble(void) {
	idex_shadow.rs = 0;
	idex_shadow.rd = 0;
	idex_shadow.rt = 0;
	idex_shadow.opCode = 0;
	idex_shadow.read_reg1 = 0;
	idex_shadow.read_reg2 = 0;
	idex_shadow.signextended = 0;
	idex_shadow.RegWrite= 0;
	idex_shadow.RegDst = 0;
	idex_shadow.MemWrite = 0;
	idex_shadow.ALUOp = ALUOP_NOP;
}

bool forward_handler(int *val1, int *val2){
		unsigned short forwardA = 0;
		unsigned short forwardB = 0;
		bool found = false;

		// From Lecture 16:

		//Ex hazrard
		if ((exmem_reg.RegWrite && (exmem_reg.rd != 0)) && (exmem_reg.rd == idex_reg.rs)) {
			forwardA = 2;
			printf("ex hazard 1\n");
		}
		if ((exmem_reg.RegWrite && (exmem_reg.rd != 0)) && (exmem_reg.rd == idex_reg.rt)) {
			forwardB = 2;
			printf("ex hazard 2\n");			
		}

		// Mem hazard
		if (memwb_reg.RegWrite && memwb_reg.rd!=0 && !(exmem_reg.RegWrite && (exmem_reg.rd!=0 && exmem_reg.rd == idex_reg.rs)) && memwb_reg.rd==idex_reg.rs){
			forwardA = 1;
			printf("mem hazard 1\n");	
		}
		if (memwb_reg.RegWrite && memwb_reg.rd!=0 && !(exmem_reg.RegWrite && (exmem_reg.rd!=0 && exmem_reg.rd == idex_reg.rt)) && memwb_reg.rd==idex_reg.rt) {
			forwardB = 1;
			printf("mem hazard 2\n");	
		}

		if (memwb_reg.MemtoReg){
    		writeData = memwb_reg.memValue;
    	} else {
    		writeData = memwb_reg.alu_Result;
    	}

		switch(forwardA) {
			case 0:
				break;
			case 1:
				*val1 = writeData;
				printf("!! Forward: Setting val1 to (%d)\n",writeData );
				found = true;
				break;
			case 2:
				*val1 = exmem_reg.alu_Result;
				printf("!! Forward: Setting val1 to (%d)\n",exmem_reg.alu_Result );
				found = true;
				break; 
		}

		switch(forwardB) {
			case 0:
				break;
			case 1:
				*val2 = writeData;
				printf("!! Forward: Setting val2 to (%d)\n",writeData );
				found = true;
				break;
			case 2:
				*val2 = exmem_reg.alu_Result;
				printf("!! Forward: Setting val2 to (%d)\n",exmem_reg.alu_Result );
				found = true;
				break;
		}
		return found;
	}

void step(void){
	IF_op();
	WB_op();
	ID_op();
	EX_op();
	MEM_op();
	move_shadows_to_reg();
}


