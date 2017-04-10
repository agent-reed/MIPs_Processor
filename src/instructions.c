//
//  tools.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include "instructions.h"

unsigned int InstructionElement(int instruction, ins_element element) {
    switch (element){
        case OP:
            return (instruction&OP_MASK)>>OP_SHIFT;
        case RS:
            return (instruction&RS_MASK)>>RS_SHIFT; 
        case RT:
            return (instruction&RT_MASK)>>RT_SHIFT;
        case RD:
            return (instruction&RD_MASK)>>RD_SHIFT;  
        case SHAMT:
            return (instruction&SHAMT_MASK)>>SHAMT_SHIFT;
        case FUNCT:
            return (instruction&FUNCT_MASK);
        case IMM:
            return (instruction&IMM_MASK);
    }
}

ins_format InstructionFormat(int ins) {
    unsigned int opCode = InstructionElement(ins, OP);
    if( opCode == 2 || opCode == 3) {
        printf("0x%x is J-Format\n",ins);
        return J_FORMAT;
    }else if(opCode){
        printf("0x%x is I-Format\n", ins);
        return I_FORMAT;
    }
    printf("0x%x is R-Format\n", ins);
    return R_FORMAT;
}


