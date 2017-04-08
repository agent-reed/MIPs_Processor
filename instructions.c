//
//  tools.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#include "instructions.h"

ins_format InstructionFormat(int ins) {
    if((ins&OP_MASK) == 2 || (ins&OP_MASK) == 3){
        // 0x02 0x03
        printf("0x%x is J_Format\n",ins);
        return J_FORMAT;
    }else if((ins&OP_MASK) == 1){
        // non-zero I Type
        printf("0x%x is I_Format\n", ins);
        return I_FORMAT;
    }
    printf("0x%x is R_Format\n", ins);
    return R_FORMAT;
}
