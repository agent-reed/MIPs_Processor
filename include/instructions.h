//
//  tools.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef instructions_h
#define instructions_h

#include <stdio.h>

// R Formats
#define R_ADD 				0x20
#define R_ADDU 				0x21
#define R_AND				0x24
#define R_DIV 				0x1a
#define R_DIVU 				0x1b
#define R_JR				0x08
#define R_MULT 				0x18
#define R_MULTU 			0x19
#define R_NOR 				0x27
#define R_XOR 				0x26
#define R_OR 				0x25
#define R_SLT 				0x2a
#define R_SLTU 				0x2b
#define R_SLL 				0x00
#define R_SRL 				0x02
#define R_SRA 				0x03
#define R_SUB 				0x22
#define R_SUBU 				0x23
#define R_MOVN 				0x0b

// I Formats
#define I_ADDI              0x08
#define I_ADDIU             0x09
#define I_SLTI              0x0a
#define I_SLTIU             0x0b
#define I_ANDI              0x0c
#define I_BEQ		      	0x04
#define I_BNE		      	0x05
#define I_LBU		      	0x24
#define I_LHU		      	0x25
#define I_LUI		      	0x0f
#define I_LW		      	0x23
#define I_ORI		      	0x0d
#define I_SD		      	0x28
#define I_SH		      	0x29
#define I_SW		      	0x2b

// J Formats
#define J_J                 0x02
#define J_JAL               0x03
#define J_R					0x08

// Useful Masks
#define OP_MASK				0xFC000000
#define RS_MASK				0x3E00000
#define RT_MASK				0x1F0000
#define RD_MASK				0xF800
#define SHAMT_MASK			0x7C0
#define FUNCT_MASK			0x3F
#define IMM_MASK			0xFFFF

#define OP_SHIFT            0x1A
#define RS_SHIFT            0x15
#define RT_SHIFT            0x10
#define RD_SHIFT            0xB
#define SHAMT_SHIFT         0x6

typedef enum {OP = 0, RS, RT, RD, SHAMT, FUNCT, IMM} ins_element;
typedef enum {R_FORMAT = 0, I_FORMAT, J_FORMAT} ins_format;

unsigned int InstructionElement(int instruction, ins_element element);
ins_format InstructionFormat(int instruction);


#endif /* tools_h */
