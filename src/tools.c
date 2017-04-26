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
    			printf("|         signextended: %x\n", idex_reg.signextended);
    			printf("|         opCode: 0x%02x\n\n", idex_reg.opCode);
    			return;

		case EXMEM:
				printf("--------------------EX/MEM Register\n");
    			printf("|               Branch: %d\n", exmem_reg.Branch);
    			printf("|               RegWrite: %d\n", exmem_reg.RegWrite);
    			printf("|               MemToReg: %d\n", exmem_reg.MemtoReg);
    			printf("|               ALU Result: %d\n\n", exmem_reg.alu_Result);


			return;

        case MEMWB:
        	return;

	}
}

void print_memoryTrace(void) {
    for(int i=0; i<MEMORY_SIZE; i++){
        printf("Memory-%u : 0x%08x | ",i,memory[i]);
        //InstructionFormat(memory[i]);  //With Formats
    }
}