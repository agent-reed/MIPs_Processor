//
//  components.c
//  MIPs_Processor
//
//  Created by Andrew Gentry on 4/8/17.
//  Copyright © 2017 agentreed. All rights reserved.
// make change

#include <stdio.h>
#include <stdlib.h>
#include "components.h"

unsigned int program_image[MEMORY_SIZE] = {4000,4000,0,0,0,49,0,0,0,0,
    0x00001025,     // 	move	v0,zero    <load_arrays>:
    0x24060064,     // 	li	a2,100
    0x8c830000,     // 	lw	v1,0(a0)
    0x00621821,     // 	addu	v1,v1,v0
    0xac830000,     // 	sw	v1,0(a0)
    0x8ca30000,     // 	lw	v1,0(a1)
    0x00621821,     // 	addu	v1,v1,v0
    0xaca30000,     // 	sw	v1,0(a1)
    0x24420001,     // 	addiu	v0,v0,1
    0x24840004,     // 	addiu	a0,a0,4
    0x1446fff7,     // 	bne	v0,a2,400838 <load_arrays+0x8>
    0x24a50004,     // 	addiu	a1,a1,4
    0x8d090f90,		//lw $t1, 3576($t0)
    0xad090018,		//sw $t1, 24($t0)
    0x8d080418,		//lw $t0, 1048($t0)
    0x03e00008,     // 	jr	ra
    0xac030018,     // 	sw v1, 24($zer0)
    0x00000000,     // 	nop
    0x00000000,     // 	nop
    0x00000000,     // 	nop
    0x00801825,    //  	move	v1,a0		<check_result>:
    0x24870190,    // 	addiu	a3,a0,400
    0x00001025,    // 	move	v0,zero
    0x24080001,    // 	li	t0,1
    0x8c640000,    // 	lw	a0,0(v1)
    0x8ca60000,    // 	lw	a2,0(a1)
    0x00862026,    // 	xor	a0,a0,a2
    0x0104100b,    // 	movn	v0,t0,a0
    0x24630004,    // 	addiu	v1,v1,4
    0x1467fffa,    // 	bne	v1,a3,400878 <check_result+0x10>
    0x24a50004,    // 	addiu	a1,a1,4
    0xac02001c,		//sw $v0, 28($zero)
    0x8c18041c,		//lw $t8, 1052($zero)
    0xac030020,		//sw $v1, 32($zero)
    0x8c180420,		//lw $t8, 1056($zero)
    0x03e00008,    // 	jr	ra
    0x00000000,    // 	nop
    0x00000000,    // 	nop
    0x00000000,    // 	nop
    0x00000000,    // 	nop
    0x27bdfcb0,    // 	addiu	sp,sp,-848   <main>:
    0xafbf034c,    // 	sw	ra,844(sp)
    0xafb20348,    // 	sw	s2,840(sp)
    0xafb10344,    // 	sw	s1,836(sp)
    0xafb00340,    // 	sw	s0,832(sp)
    //		0x3c1c0042,    // 	lui	gp,0x42
    //		0x279c9010,    // 	addiu	gp,gp,-28656
    //		0xafbc0010,    // 	sw	gp,16(sp)
    //		0x8f928050,    // 	lw	s2,-32688(gp)
    //		0x8e420000,    // 	lw	v0,0(s2)
    0xafa2033c,    // 	sw	v0,828(sp)
    0x27b101ac,    // 	addiu	s1,sp,428
    0x02202825,    // 	move	a1,s1
    0x27b0001c,    // 	addiu	s0,sp,28
    0x02002025,    // 	move	a0,s0
    0x0c00000a,    // 	jal	10 <load_arrays>
    0x00000000,    // 	nop
    //		0x8fbc0010,    // 	lw	gp,16(sp)
    0x02202825,    // 	move	a1,s1
    0x02002025,    // 	move	a0,s0
    0x0c00001e,    // 	jal	30 <check_result>
    0x00000000,    // 	nop
    //		0x8fbc0010,    // 	lw	gp,16(sp)
    0x8fa4033c,    // 	lw	a0,828(sp)
    0x8e430000,    // 	lw	v1,0(s2)
    0x10830004,    // 	beq	a0,v1,400914 <main+0x78>
    0x8fbf034c,    // 	lw	ra,844(sp)
    //		0x8f99804c,    // 	lw	t9,-32692(gp)
    //		0x0320f809,    // 	jalr	t9
    0x00000000,    // 	nop
    0x8fb20348,    // 	lw	s2,840(sp)
    0x8fb10344,    // 	lw	s1,836(sp)
    0x8fb00340,    // 	lw	s0,832(sp)
    0x00000008,    // 	jr	$zero
    0x00000000,	   //
    0x00000000};   //

/*
 unsigned int program_image[memory_size] = {4000,4000,0,0,0,120,0,0,0,0,
 0x27bdfff0,    // addiu	sp,sp,-16 <load_arrays>
 0xafbe000c,    // sw	s8,12(sp)
 0x03a0f025,    // move	s8,sp
 0xafc40010,    // sw	a0,16(s8)
 0xafc50014,    // sw	a1,20(s8)
 0xafc00004,    // sw	zero,4(s8)
 0x1000001c,    // b	4008bc <load_arrays+0x8c>
 0x00000000,    // nop
 0x8fc20004,    // lw	v0,4(s8)
 0x00021080,    // sll	v0,v0,0x2
 0x8fc30010,    // lw	v1,16(s8)
 0x00621021,    // addu	v0,v1,v0
 0x8fc30004,    // lw	v1,4(s8)
 0x00031880,    // sll	v1,v1,0x2
 0x8fc40010,    // lw	a0,16(s8)
 0x00831821,    // addu	v1,a0,v1
 0x8c640000,    // lw	a0,0(v1)
 0x8fc30004,    // lw	v1,4(s8)
 0x00831821,    // addu	v1,a0,v1
 0xac430000,    // sw	v1,0(v0)
 0x8fc20004,    // lw	v0,4(s8)
 0x00021080,    // sll	v0,v0,0x2
 0x8fc30014,    // lw	v1,20(s8)
 0x00621021,    // addu	v0,v1,v0
 0x8fc30004,    // lw	v1,4(s8)
 0x00031880,    // sll	v1,v1,0x2
 0x8fc40014,    // lw	a0,20(s8)
 0x00831821,    // addu	v1,a0,v1
 0x8c640000,    // lw	a0,0(v1)
 0x8fc30004,    // lw	v1,4(s8)
 0x00831821,    // addu	v1,a0,v1
 0xac430000,    // sw	v1,0(v0)
 0x8fc20004,    // lw	v0,4(s8)
 0x24420001,    // addiu	v0,v0,1
 0xafc20004,    // sw	v0,4(s8)
 0x8fc20004,    // lw	v0,4(s8)
 0x28420064,    // slti	v0,v0,100
 0x1440ffe2,    // bnez	v0,400850 <load_arrays+0x20>
 0x00000000,    // nop
 0x00000000,    // nop
 0x03c0e825,    // move	sp,s8
 0x8fbe000c,    // lw	s8,12(sp)
 0x27bd0010,    // addiu	sp,sp,16
 0x03e00008,    // jr	ra
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x27bdffe8,    // 	addiu	sp,sp,-24 <check_result>
 0xafbe0014,    // sw	s8,20(sp)
 0x03a0f025,    // move	s8,sp
 0xafc40018,    // sw	a0,24(s8)
 0xafc5001c,    // sw	a1,28(s8)
 0xafc00000,    // sw	zero,0(s8)
 0xafc00008,    // sw	zero,8(s8)
 0x1000001d,    // b	400978 <check_result+0x94>
 0x00000000,    // nop
 0x8fc20008,    // lw	v0,8(s8)
 0x00021080,    // sll	v0,v0,0x2
 0x8fc30018,    // lw	v1,24(s8)
 0x00621021,    // addu	v0,v1,v0
 0x8c430000,    // lw	v1,0(v0)
 0x8fc20008,    // lw	v0,8(s8)
 0x00021080,    // sll	v0,v0,0x2
 0x8fc4001c,    // lw	a0,28(s8)
 0x00821021,    // addu	v0,a0,v0
 0x8c420000,    // lw	v0,0(v0)
 0x00621026,    // xor	v0,v1,v0
 0xafc2000c,    // sw	v0,12(s8)
 0x8fc2000c,    // lw	v0,12(s8)
 0x10400003,    // beqz	v0,40094c <check_result+0x68>
 0x00000000,    // nop
 0x24020001,    // li	v0,1
 0xafc20000,    // sw	v0,0(s8)
 0x8fc20008,    // lw	v0,8(s8)
 0x00021080,    // sll	v0,v0,0x2
 0x8fc30018,    // lw	v1,24(s8)
 0x00621021,    // addu	v0,v1,v0
 0x8c420000,    // lw	v0,0(v0)
 0x8fc30004,    // lw	v1,4(s8)
 0x00621021,    // addu	v0,v1,v0
 0xafc20004,    // sw	v0,4(s8)
 0x8fc20008,    // lw	v0,8(s8)
 0x24420001,    // addiu	v0,v0,1
 0xafc20008,    // sw	v0,8(s8)
 0x8fc20008,    // lw	v0,8(s8)
 0x28420064,    // slti	v0,v0,100
 0x1440ffe1,    // bnez	v0,400908 <check_result+0x24>
 0x00000000,    // nop
 0x8fc20000,    // lw	v0,0(s8)
 0x14400003,    // bnez	v0,40099c <check_result+0xb8>
 0x00000000,    // nop
 0x8fc20004,    // lw	v0,4(s8)
 0xafc20000,    // sw	v0,0(s8)
 0x00000000,    // nop
 0x03c0e825,    // move	sp,s8
 0x8fbe0014,    // lw	s8,20(sp)
 0x27bd0018,    // addiu	sp,sp,24
 0x03e00008,    // jr	ra
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x27bdfcb8,    // addiu	sp,sp,-840 <main>
 0xafbf0344,    // sw	ra,836(sp)
 0xafbe0340,    // sw	s8,832(sp)
 0x03a0f025,    // move	s8,sp
 //		0x3c1c0042,    // lui	gp,0x42
 //		0x279c9010,    // addiu	gp,gp,-28656
 //		0xafbc0010,    // sw	gp,16(sp)
 //		0x8f828050,    // lw	v0,-32688(gp)
 //		0x8c420000,    // lw	v0,0(v0)
 //		0xafc2033c,    // sw	v0,828(s8)
 0x27c301ac,    // addiu	v1,s8,428
 0x27c2001c,    // addiu	v0,s8,28
 0x00602825,    // move	a1,v1
 0x00402025,    // move	a0,v0
 0x0c00000a,    // jal	10 <load_arrays>
 0x00000000,    // nop
 //		0x8fdc0010,    // lw	gp,16(s8)
 0x27c301ac,    // addiu	v1,s8,428
 0x27c2001c,    // addiu	v0,s8,28
 0x00602825,    // move	a1,v1
 0x00402025,    // move	a0,v0
 0x0c00003c,    // jal	60 <check_result>
 0x00000000,    // nop
 //		0x8fdc0010,    // lw	gp,16(s8)
 0x00000000,    // nop
 //		0x8f828050,    // lw	v0,-32688(gp)
 //		0x8fc3033c,    // lw	v1,828(s8)
 //		0x8c420000,    // lw	v0,0(v0)
 //		0x10620005,    // beq	v1,v0,400a3c <main+0x88>
 0x00000000,    // nop
 //		0x8f82804c,    // lw	v0,-32692(gp)
 //		0x0040c825,    // move	t9,v0
 //		0x0320f809,    // jalr	t9
 0x00000000,    // nop
 0x03c0e825,    // move	sp,s8
 0x8fbf0344,    // lw	ra,836(sp)
 0x8fbe0340,    // lw	s8,832(sp)
 0x27bd0348,    // addiu	sp,sp,840
 0x00004020,	// add $t0, $zero, $zero
 0x8d090c44,	// lw $t1, 3140($t0)
 0xad090020,	// sw $t1, 32($t0)
 0x8d080420,	//lw $t0, 1056($t0)
 0x8d090c4c,	// lw $t1, 3148($t0)
 0xad09001c,	// sw $t1, 28($t0)
 0x8d08041c,	//lw $t0, 1054($t0)
 0x8d090f90,	//lw $t1, 3984($t0)
 0xad090018,	//sw $t1, 24($t0)
 0x8d080418,	//lw $t0, 1048($t0)
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000008,    // jr	$zero
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000,    // nop
 0x00000000};    // nop
 */

void initialize_components(void) {
    reg_file[SP] = memory[0];
    reg_file[FP] = memory[1];
    PC_zero = memory[5];
    initialize_caches(ICache, INST_CACHE);
    initialize_caches(DCache, DATA_CACHE);
    
    printf("*****************     Cache Initialized.     *****************\n\n");
    int a = ICache[5].data[0];
    printf("******        Test Value = %d\n", a);
    printf("INFO: Initializing Components | Stack Pointer: [0x%08x]\tFrame Pointer: [0x%08x] PC: [0x%08x]\n\n", reg_file[SP], reg_file[FP], PC);
}

void initialize_caches() {
	ICache = createCache(ICACHE_SIZE, ICACHE_SIZE/BLOCK_SIZE);
	DCache = createCache(DCACHE_SIZE, DCACHE_SIZE/BLOCK_SIZE);
	ICache_config = createCacheConfig(ICACHE_SIZE, ICACHE_SIZE/BLOCK_SIZE);
	DCache_config = createCacheConfig(DCACHE_SIZE, DCACHE_SIZE/BLOCK_SIZE);
}

cache * createCache(int size, int block_num) {
	cache *cache_out = NULL;
	unsigned int *data = NULL;
	
	cache_out = (cache *)calloc(block_num, sizeof(cache));
	if (!cache_out) { 
		printf("ERROR:   Unable to allocate heap memory!\n");
	}
	
	for (int i=0; i<block_num; i++) {
		data = (unsigned int*)calloc(BLOCK_SIZE, sizeof(unsigned int));
		if (!data) {
			printf("ERROR:   Unable to allocate heap memory!\n");
		}
		cache_out[i].data = data;
	}
	
	return cache_out;	
}

cache_config * createCacheConfig(int size, int block_num) {
	cache_config *config_out = NULL;
	config_out = (cache_config *)malloc(sizeof(cache_config));
	
	config_out->hits = 0;
	config_out->misses = 0;
	config_out->size = size;
	config_out->block_num = block_num;
	config_out->write_policy = WRITE_POLICY;

	return config_out; 
}

void initialize_simulation_memory(void){
    
    for (int i=0; i < MEMORY_SIZE; i++){
        memory[i] = program_image[i];
    }
}


