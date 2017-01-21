/*
 * Copyright (c) 2016, ARM Limited and Contributors. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * Neither the name of ARM nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rk3399_mcu.h"

/* Stack configuration */
#define STACK_SIZE	0x00000100
__attribute__ ((section(".co_stack")))
unsigned long pstack[STACK_SIZE];

/* Macro definition */
#define WEAK __attribute__ ((weak))

/* System exception vector handler */
__attribute__ ((used))
void WEAK reset_handler(void);
void WEAK nmi_handler(void);
void WEAK hardware_fault_handler(void);
void WEAK svc_handler(void);
void WEAK pend_sv_handler(void);
void WEAK systick_handler(void);

extern int main(void);

/* Function prototypes */
static void default_reset_handler(void);
static void default_handler(void);

/*
 * The minimal vector table for a Cortex M3.  Note that the proper constructs
 * must be placed on this to ensure that it ends up at physical address
 * 0x00000000.
 */
__attribute__ ((used, section(".isr_vector")))
void (* const g_pfnVectors[])(void) = {
	/* core Exceptions */
	(void *)&pstack[STACK_SIZE], /* the initial stack pointer */
	reset_handler,
	nmi_handler,
	hardware_fault_handler,
	0, 0, 0, 0, 0, 0, 0,
	svc_handler,
	0, 0,
	pend_sv_handler,
	systick_handler,

	/* external exceptions */
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0
};

/**
 * This is the code that gets called when the processor first
 * starts execution following a reset event. Only the absolutely
 * necessary set is performed, after which the application
 * supplied main() routine is called.
 */
static void default_reset_handler(void)
{
	/* call the application's entry point */
	main();
}

/**
 * Provide weak aliases for each Exception handler to the Default_Handler.
 * As they are weak aliases, any function with the same name will override
 * this definition.
 */
#pragma weak reset_handler = default_reset_handler
#pragma weak nmi_handler = default_handler
#pragma weak hardware_fault_handler = default_handler
#pragma weak svc_handler = default_handler
#pragma weak pend_sv_handler = default_handler
#pragma weak systick_handler = default_handler

/**
 * This is the code that gets called when the processor receives
 * an unexpected interrupt.  This simply enters an infinite loop,
 * preserving the system state for examination by a debugger.
 */
static void default_handler(void)
{
    /* go into an infinite loop. */
	while (1)
		;
}
