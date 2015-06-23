/*
 * Copyright (c) 2012 Xilinx, Inc.  All rights reserved.
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <byteswap.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/mman.h>

#define PAGE_SIZE ((size_t)getpagesize())
#define PAGE_MASK ((uint64_t)(long)~(PAGE_SIZE - 1))

#define COMM_BASE       0xFFFF9000
#define COMM_TX_FLAG_OFFSET    0x00
#define COMM_TX_DATA_OFFSET    0x04
#define COMM_RX_FLAG_OFFSET    0x08
#define COMM_RX_DATA_OFFSET    0x0C

#define MAX_STR 256

int main()
{
    int fd;
    uint32_t value=0;
    uint32_t flag=0;
    volatile uint8_t *mm;
    uint8_t str[MAX_STR];
    int rcnt;

    fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd < 0) {
        fprintf(stderr, "open(/dev/mem) failed (%d)\n", errno);
        return 1;
    }

    mm = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, COMM_BASE);
    if (mm == MAP_FAILED) {
        fprintf(stderr, "mmap64(0x%x@0x%x) failed (%d)\n",
                PAGE_SIZE, (uint32_t)(COMM_BASE), errno);
        return 1;
    }

    rcnt = 0;
    while(1) {

    	//read
    	if( (flag = *(volatile uint32_t *)(mm + COMM_TX_FLAG_OFFSET)) ) {
        	value = *(volatile uint32_t *)(mm + COMM_TX_DATA_OFFSET);

    		//process non-string type data
        	if(flag > 1) {
        		printf("CPU1: 0x%08x = 0x%08x\n", (uint32_t)(mm + COMM_TX_DATA_OFFSET), value);

        	//process string type data
        	} else {

        		if(rcnt < MAX_STR) {
        			str[rcnt++] = value;
        		}
        		if(value == '\n') {
        			if(rcnt != 0) {
        				str[rcnt-1] = '\0';
        			} else {
        				str[0] = '\0';
        			}
        			printf("CPU1: %s\n", str);
        			rcnt = 0;
        		}
        	}
    		*(volatile uint32_t *)(mm + COMM_TX_FLAG_OFFSET) = 0;
    	}

    }

    munmap((void *)mm, PAGE_SIZE);
    close(fd);

    return 0;
}
