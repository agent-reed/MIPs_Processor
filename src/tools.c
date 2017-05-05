#include <stdio.h>
#include "tools.h"


void print_pipelineReg(pipeline_reg pReg) {
	switch (pReg) {
		case IFID:
			printf("\nPIPELINE REG TRACE:\n  ----------IF/ID Register\n");
			printf("|   Instruction: 0x%x\n", ifid_reg.instruction);
			printf("|   Next PC: %d\n", ifid_reg.nextPC);
			printf("|   PC Write: %d\n\n", ifid_reg.pcWrite);
			return;

		case IDEX:
				printf("---------------ID/EX Register\n");
				switch(idex_reg.ALUOp){
					case ALUOP_LWSW:
						printf("          ALUOp: LWSW\n");
						break;
					case ALUOP_BEQ:
						printf("          ALUOp:  BEQ\n");
						break;

					case ALUOP_R:
						printf("          ALUOp:  R\n");
						break;

					case ALUOP_NOP:
						printf("          ALUOp:  NOP\n");
						break;
				}
    			printf("|         read_reg1: %d\n", idex_reg.read_reg1);
    			printf("|         read_reg2: %d\n", idex_reg.read_reg2);
    			printf("|         signextended: 0x%x\n", idex_reg.signextended);
    			printf("|         opCode: 0x%02x\n", idex_reg.opCode);
    			printf("|         Branch: %d\n", idex_reg.Branch);
    			printf("|         MemRead: %d\n", idex_reg.MemRead);
    			printf("|         MemWrite: %d\n", idex_reg.MemWrite);
    			printf("|         RegWrite: %d\n", idex_reg.RegWrite);
    			printf("|         MemToReg: %d\n", idex_reg.MemtoReg);
    			printf("|         ALU_Src: %d\n", idex_reg.ALUSrc);
    			printf("|         rs: %d\n", idex_reg.rs);
    			printf("|         rt: %d\n", idex_reg.rt);
    			printf("|         rd: %d\n", idex_reg.rd);
    			printf("|         RegDst: %d\n\n", idex_reg.RegDst);
    			return;

		case EXMEM:
				printf("--------------------EX/MEM Register\n");
    			printf("|               Branch: %d\n", exmem_reg.Branch);
    			printf("|               RegWrite: %d\n", exmem_reg.RegWrite);
    			printf("|               MemToReg: %d\n", exmem_reg.MemtoReg);
    			printf("|               MemValue: %u\n", exmem_reg.dataToMem);
    			printf("|               ALU Result: %d\n", exmem_reg.alu_Result);
    			printf("|               MemWrite: %u\n", exmem_reg.MemWrite);
    			printf("|               Rd: %u\n\n", exmem_reg.rd);


			return;

        case MEMWB:
				printf("-------------------------MEM/WB Register\n");
    			printf("|                    MemVal: %u\n", memwb_reg.memValue);
    			printf("|                    alu_result: %d\n", memwb_reg.alu_Result);
    			printf("|                    Rd: %d\n", memwb_reg.rd);
    			printf("|                    RegWrite: %d\n\n", memwb_reg.RegWrite);
    			printf("|                    MemtoReg: %d\n\n", memwb_reg.MemtoReg);
        	return;

	}
}

void print_reg_file_trace(void){

	printf("(Zero): %4u  |  (at): %4u\n",reg_file[0], reg_file[1]);
	printf("  (v0): %4u  |  (v1): %4u\n",reg_file[2], reg_file[3]);
	printf("  (a0): %4u  |  (a1): %4u\n",reg_file[4], reg_file[5]);
	printf("  (a2): %4u  |  (a3): %4u\n",reg_file[6], reg_file[7]);
	for (int i=8; i<32; i=i+2){
	printf("Reg(%d): %u |  Reg(%d): %u\n",i, reg_file[i], i+1, reg_file[i+1]);
	}
}

void print_memoryTrace(int size) {
    for(int i=0; i<size; i++){
        printf("Memory-%u : 0x%08x | \n",i,memory[i]);
        //InstructionFormat(memory[i]);  //With Formats
    }
}

void print_just_PC(void) {
	printf("PC: (%d) | ifid_reg.nextPC: (%d) | pc_zero: (%d) | pc_one: (%d)\n", PC, ifid_reg.nextPC, PC_zero, PC_one);
}

void print_all_pipeRegs(void) {
	 print_pipelineReg(IFID);
	 print_pipelineReg(IDEX);
	 print_pipelineReg(EXMEM);
	 print_pipelineReg(MEMWB);
}