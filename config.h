//
//  config.h
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/6/17.
//  Copyright © 2017 agentreed. All rights reserved.
//

#ifndef config_h
#define config_h

#define MAX_READLINE		256

#define MEMORY_SIZE			4096
#define DMEMORY_SIZE		4096
#define IMEMORY_SIZE		4096

#define REGISTER_COUNT		32

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

// Registers
#define SP					29
#define FP					30


// Useful Masks
#define OP_MASK				0xfc000000
#define RS_MASK				0x03e00000
#define RT_MASK				0x001f0000
#define RD_MASK				0x0000f800
#define SH_MASK				0x000007c0
#define FN_MASK				0x0000003f


// Type Definitions
typedef enum {R_FORMAT = 0, I_FORMAT, J_FORMAT} ins_format;
typedef enum {OP = 0, RS, RT, RD, SHAMT, FUNCT, IMM} ins_element;
typedef enum {ALUOP_LWSW = 0, ALUOP_BEQ, ALUOP_R, ALUOP_NOP} alu_op;
typedef enum {STAGE_IF=0x0, STAGE_ID, STAGE_EX, STAGE_MEM, STAGE_WB} stage;
typedef enum {false = 0, true = 1} bool;


// Register Definitions
typedef struct {
    bool PCWrite;
    unsigned int nextPC;
    unsigned int instruction;
    
} IFID_Register;

typedef struct {
    // WriteBack
    bool MemtoReg;
    bool RegWrite;
    // Memory
    bool Branch; // present in 4.51
    bool MemRead;
    bool MemWrite;
    // Execution
    bool ALUSrc;
    bool RegDst;
    alu_op ALUOp;
    unsigned opCode;
    
    unsigned int regValue1;
    unsigned int regValue2;
    unsigned int extendedValue;
    
    unsigned short rs;
    unsigned short rt;
    unsigned short rd;
    
} IDEX_Register;

typedef struct {
    // WriteBack
    bool MemtoReg;
    bool RegWrite;
    // Memory
    bool Branch; // present in 4.51
    bool MemRead;
    bool MemWrite;
    
    bool zero;
    unsigned int aluResult;
    unsigned int dataToMem;
    unsigned short rd;
    
} EXMEM_Register;

typedef struct {
    // WB
    bool MemtoReg;
    bool RegWrite;
    
    unsigned int memValue;
    unsigned int aluResult;
    unsigned short rd;
    
} MEMWB_Register;

#endif /* config_h */
