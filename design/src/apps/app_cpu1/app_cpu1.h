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
#include "xil_types.h"
//#include <stdio.h>
//#include "xil_io.h"
//#include "xil_mmu.h"
//#include "xil_cache.h"
//#include "xil_printf.h"
//#include "xil_exception.h"
#include "xscugic.h"
////#include "sleep.h"
//#include "scu_sleep.h"
//#include "xpseudo_asm.h"

/**************************** Type Definitions *******************************/

/************************** Constant Definitions *****************************/

/*
 * Use SCU timer instead of global timer for 1sec sleep
 * NOTE: The SCU timer is used for profiling so do not
 * enable both profiling and USE_SCU_TIMER
 */
#define USE_SCU_TIMER

#ifdef XPAR_INTC_0_DEVICE_ID
#define INTC		    XIntc
#define INTC_HANDLER	XIntc_InterruptHandler
#else
#define INTC		    XScuGic
#define INTC_HANDLER	XScuGic_InterruptHandler
#endif /* XPAR_INTC_0_DEVICE_ID */

#define INTC_DEVICE_ID	    XPAR_PS7_SCUGIC_0_DEVICE_ID
#define PL_IRQ_ID           XPS_IRQ_INT_ID

#define IRQ_PCORE_GEN_BASE  XPAR_IRQ_GEN_0_BASEADDR


/**************************** Type Definitions *******************************/
/**
 * This typedef contains configuration information for the device driver.
 */
typedef struct {
	u16 DeviceId;		/**< Unique ID of device */
	u32 BaseAddress;	/**< Base address of the device */
} Pl_Config;


/**
 * The driver instance data. The user is required to allocate a
 * variable of this type.
 * A pointer to a variable of this type is then passed to the driver API
 * functions.
 */
typedef struct {
	Pl_Config Config;   /**< Hardware Configuration */
	u32 IsReady;		/**< Device is initialized and ready */
	u32 IsStarted;		/**< Device is running */
} XPlIrq;


/***************** Macros (Inline Functions) Definitions *********************/
#define COMM_VAL        (*(volatile unsigned long *)(0xFFFF8000))

#define COMM_TX_FLAG    (*(volatile unsigned long *)(0xFFFF9000))
#define COMM_TX_DATA    (*(volatile unsigned long *)(0xFFFF9004))
#define COMM_RX_FLAG    (*(volatile unsigned long *)(0xFFFF9008))
#define COMM_RX_DATA    (*(volatile unsigned long *)(0xFFFF900C))


/************************** Function Prototypes ******************************/

/************************** Variable Definitions *****************************/

