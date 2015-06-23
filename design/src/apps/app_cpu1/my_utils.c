/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

/***************************** Include Files *********************************/
#include "xparameters.h"
//#include <stdio.h>
//#include "xil_io.h"
//#include "xil_mmu.h"
//#include "xil_cache.h"
//#include "xil_printf.h"
//#include "xil_exception.h"
//#include "xscugic.h"
#include "xil_types.h"
#include "xpseudo_asm.h"
#include "app_cpu1.h"
#include "my_utils.h"

/**************************** Type Definitions *******************************/

/************************** Constant Definitions *****************************/

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/


/**
 * Modified DCacheFlush to prevent L2 Cache controller access
 */
void MyXil_DCacheFlush(void)
{
	Xil_L1DCacheFlush();
	//Xil_L2CacheFlush();
}

/**
 * Modified SetTlbAttributes to call MyXil_DCacheFlush in order
 * to prevelt L2 Cache controller accesses
 */
void MyXil_SetTlbAttributes(u32 addr, u32 attrib)
{
	u32 *ptr;
	u32 section;

	mtcp(XREG_CP15_INVAL_UTLB_UNLOCKED, 0);
	dsb();

	mtcp(XREG_CP15_INVAL_BRANCH_ARRAY, 0);
	dsb();
	MyXil_DCacheFlush();

	section = addr / 0x100000;
	ptr = &MMUTable + section;
	*ptr = (addr & 0xFFF00000) | attrib;
	dsb();
}

/*
 * Function to send data to OCM which is consumed by Linux Application
 */
void myPutchar(char c) {

	while(COMM_TX_FLAG);	//wait for cpu0 to consume previous value
	COMM_TX_DATA = (volatile unsigned long)c;
	dmb();
	COMM_TX_FLAG = 1;

}

/*
 * Since there is no STDIO assigned to the BSP, outbyte() is not created
 * by libgen. So add outbyte() call to myPutchar() to enable STDOUT functionality
 * for print() and xil_printf()
 */
void outbyte(char c) {
	myPutchar(c);
}


