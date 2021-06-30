/*
 *
 * Copyright (c) 2005-2019 Imperas Software Ltd., www.imperas.com
 *
 * The contents of this file are provided under the Software License
 * Agreement that you accepted before downloading this file.
 *
 * This source forms part of the Software and can be used for educational,
 * training, and demonstration purposes but cannot be used for derivative
 * works except in cases where the derivative works require OVP technology
 * to run.
 *
 * For open source models released under licenses that you can use for
 * derivative works, please visit www.OVPworld.org or www.imperas.com
 * for the location of the open source models.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "dmacRegisters.h"

typedef unsigned int  Uns32;
typedef unsigned char Uns8;

#include "riscvInterrupts.h"

#define LOG(_FMT, ...)  printf( "TEST DMA: " _FMT,  ## __VA_ARGS__)

void int_init(void (*handler)()) {

    // Set MTVEC register to point to handler function in direct mode
    int handler_int = (int) handler & ~0x1;
    write_csr(mtvec, handler_int);

    // Enable Machine mode external interrupts
    set_csr(mie, MIE_MEIE);
}

void int_enable() {
    set_csr(mstatus, MSTATUS_MIE);
}


static inline void writeReg32(Uns32 address, Uns32 offset, Uns32 value)
{
    *(volatile Uns32*) (address + offset) = value;
}


static inline Uns32 readReg32(Uns32 address, Uns32 offset)
{
    return *(volatile Uns32*) (address + offset);
}

static inline void writeReg8(Uns32 address, Uns32 offset, Uns8 value)
{
    *(volatile Uns8*) (address + offset) = value;
}

static inline Uns8 readReg8(Uns32 address, Uns32 offset)
{
    return *(volatile Uns8*) (address + offset);
}

#define ENABLE      0x00000001
#define INTEN       0x00008000
// burst size is 1<<BURST_SIZE
#define BURST_SIZE       2

volatile static Uns32 interruptCount = 0;

void interruptHandler(void)
{
/*
    LOG("Interrupt******************************************\n");
    // read interrupt status
    Uns32 intStatus = readReg8(DMA_BASE, DMA_INT_TC_STATUS);
    LOG("read interrupt status\n");
    
    // check channel 0 interrupts enabled and status indicates interrupt set
    if ( (readReg32(DMA_BASE, DMA_C0_CONFIGURATION) & 0x8000 ) &&
         (intStatus & 1<<0)){
        LOG("Interrupt ch0 0x%x (0x%02x)\n",
        readReg32(DMA_BASE, DMA_C0_CONFIGURATION), intStatus);
        // disable ch0 interrupt
        writeReg32(DMA_BASE, DMA_C0_CONFIGURATION, 0);
        // clear ch0 interrupt
        writeReg8(DMA_BASE, DMA_INT_TC_STATUS, 1<<0);
        interruptCount++;
       printf( "interuptcount = %x ",interruptCount);
    }
    LOG("check channel 0 interrupts enabled and status indicates interrupt set\n");

    // check channel 1 interrupts enabled and status indicates interrupt set
    if ( (readReg32(DMA_BASE, DMA_C1_CONFIGURATION) & 0x8000 ) &&
         (intStatus & 1<<1)){
        LOG("Interrupt ch1 0x%x (0x%02x)\n",
            readReg32(DMA_BASE, DMA_C1_CONFIGURATION), intStatus);
        // disable ch1 interrupt
        writeReg32(DMA_BASE, DMA_C1_CONFIGURATION, 0);
        // clear ch1 interrupt
        writeReg8(DMA_BASE, DMA_INT_TC_STATUS, 1<<1);
        interruptCount++;
    }*/
}

/*
static void dmaBurst(Uns32 ch, void *from, void *to, Uns32 bytes)
{
    Uns32 offset = ch * DMA_CHANNEL_STRIDE;
    LOG("dmaBurst ch:%d  bytes:%d\n", ch, bytes);
    writeReg32(DMA_BASE, DMA_C0_SRC_ADDR + offset, (Uns32)from);
    writeReg32(DMA_BASE, DMA_C0_DST_ADDR + offset, (Uns32)to);
    writeReg32(DMA_BASE, DMA_C0_CONTROL  + offset, bytes);

    writeReg32(DMA_BASE, DMA_C0_CONFIGURATION + offset, ENABLE|INTEN);
}
*/


int main(int argc, char **argv)
{   	printf("RAM3 start initializeing... put 0x200000-0x200800 to 0\n");
	
   	for(Uns32 i=0x000;i<=0x800;i=i+4){
		writeReg32(RAM3_BASE, i, 0x0);
	}
	Uns8 sendChar = '0';
	for(Uns32 i=0x400;i<=0x7FF;i++){	
			
		writeReg8(RAM3_BASE, i, sendChar);

	if((sendChar >= 'A' && sendChar < 'F') || (sendChar >= '0' && sendChar < '9'))
			sendChar = sendChar + 0x01;
		else if(sendChar == '9')
			sendChar = 'A';
		else if(sendChar == 'F')
			sendChar = '0';
	}
	printf("RAM3 finish initializeing !!!\n");
	printf("show ram3 !!!\n");
 	for(Uns32 i=0;i<32;i++)
		printf("RAM3_ADDR : %x  value : %c  \n", (RAM3_BASE + 0x400 + i), readReg8((RAM3_BASE + 0x400 + i), 0));
	printf("In short only show 32 item &&go on ram 4\n");
		
       printf("RAM4 start initializeing ..put 0x300000-0x300800 to 0 \n");
	for(Uns32 i=0x000;i<=0x800;i++){
		writeReg8(RAM4_BASE, i, 0x0);
	}
	Uns8 sendChar2 = 'F';
	for(Uns32 i=0x400;i<=0x7FF;i++){	
			
		writeReg8(RAM4_BASE, i, sendChar2);

	if((sendChar2 > 'A' && sendChar2 <= 'F') || (sendChar2 > '0' && sendChar2 <= '9'))
			sendChar2 = sendChar2 - 0x01;
		else if(sendChar2 == '0')
			sendChar2 = 'F';
		else if(sendChar2 == 'A')
			sendChar2 = '9';
	}
	printf("RAM4 finish initializeing !!!\n");
	printf("show ram4!!!\n");		
for(Uns32 i=0;i<32;i++)
		printf("RAM4_ADDR : %x  value : %c  \n", (RAM4_BASE + 0x400 + i), readReg8((RAM4_BASE + 0x400 + i), 0));	
		
	printf("In short only show 32 item &&go on moving data\n");	
			
    // Prepare test data in ram1
    /*
    writeReg32(RAM3_BASE, 0x4000, 0x1111);
printf("********************************************************: %x  value : %x  \n", (RAM3_BASE+0x4000), readReg32((RAM3_BASE), 0x4000));    
    writeReg32(RAM1_BASE, 0x4004, 0x22222);
    writeReg32(RAM1_BASE, 0x4008, 0x333333);
    writeReg32(RAM1_BASE, 0x400c, 0x4444444);
    writeReg32(RAM1_BASE, 0x4010, 0x55555555);
    writeReg32(RAM1_BASE, 0x4014, 0x66666666);
*/
		
    // Launch ISR
    int_init(trap_entry);
    int_enable();
    
    printf("first round start\n");
    // Write DMA control registers to move data
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x200400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(RAM4_BASE));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(1));
    wfi();
    
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x300400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(0x200000));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(0x1));
    wfi();
    
    printf("first round end\n");
    
    printf("second round start\n");
    // Write DMA control registers to move data
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x200400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(RAM4_BASE));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(1));
    wfi();
    
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x300400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(0x200000));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(0x1));
    wfi();
    
    printf("second round end\n");
    
    printf("third round start\n");
    // Write DMA control registers to move data
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x200400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(RAM4_BASE));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(1));
    wfi();
    
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x300400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(0x200000));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(0x1));
    wfi();
    
    printf("third round end\n");
    
    printf("fourth round start\n");
    // Write DMA control registers to move data
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x200400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(RAM4_BASE));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(1));
    wfi();
    
    writeReg32(DMA_BASE, (Uns32)(0x0), (Uns32)(0x300400));
    writeReg32(DMA_BASE, (Uns32)(0x4), (Uns32)(0x200000));
    writeReg32(DMA_BASE, (Uns32)(0x8), (Uns32)(0x400));
    writeReg32(DMA_BASE, (Uns32)(0xc), (Uns32)(0x1));
    wfi();
    
    printf("fourth round end\n");
    

    
    //for(int i =0 ;i<100000;i++);
/*
    // Check the data moving result
    printf("------------- source mem data ------------\n");
    printf("address = %x , mem_data = %x\n",(0x4000),*(Uns32*)(0x4000));
    printf("address = %x , mem_data = %x\n",(0x4004),*(Uns32*)(0x4004));
    printf("address = %x , mem_data = %x\n",(0x4008),*(Uns32*)(0x4008));
    printf("address = %x , mem_data = %x\n",(0x400c),*(Uns32*)(0x400c));
    printf("address = %x , mem_data = %x\n",(0x4010),*(Uns32*)(0x4010));
    printf("address = %x , mem_data = %x\n",(0x4014),*(Uns32*)(0x4014));

    printf("------------- target mem data ------------\n");
    printf("address = %x , mem_data = %x\n",(0xd0002000),*(Uns32*)(0xd0002000));
    printf("address = %x , mem_data = %x\n",(0xd0002004),*(Uns32*)(0xd0002004));
    printf("address = %x , mem_data = %x\n",(0xd0002008),*(Uns32*)(0xd0002008));
    printf("address = %x , mem_data = %x\n",(0xd000200c),*(Uns32*)(0xd000200c));
    printf("address = %x , mem_data = %x\n",(0xd0002010),*(Uns32*)(0xd0002010));
    printf("address = %x , mem_data = %x\n",(0xd0002014),*(Uns32*)(0xd0002014));
*/
/*
    int i;
    for (i = 0; i < 3; i++) {

    	interruptCount = 0;

        writeReg8(DMA_BASE, DMA_CONFIGURATION, BURST_SIZE);     // reset 

        LOG("initial dst1 '%s' dst2 '%s'\n", dst1, dst2);

        // toggle strings to show changes when copying 
    	char *s1 = i&1 ? src1 : src2;
    	char *s2 = i&1 ? src2 : src1;

		// write to DMAC registers to start burst
		dmaBurst(0, s1, dst1, strlen(s1)+1);
		dmaBurst(1, s2, dst2, strlen(s2)+1);

		// wait for burst to complete 
		LOG("Waiting for interrupts\n");
		while ( interruptCount < 2 ) {
			wfi();
		}
		LOG("%u interrupts received\n", interruptCount);

	    // check results 
	    LOG("DMA result dst1 '%s' dst2 '%s'\n", dst1, dst2);
    }
*/

    return 1;
}

