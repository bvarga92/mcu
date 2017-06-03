/*
**********************************************************************************************************
*                                               uC/OS-II
*                                         The Real-Time Kernel
*
*                                           AVR Specific code
*
* File         : OS_CPU_C.C
* By           : Ole Saether
* Port Version : V1.01
*
* AVR-GCC port version : 1.0 	2001-04-02 modified/ported to avr-gcc by Jesper Hansen (jesperh@telia.com)
*
* Some modifications by Julius Luukko (Julius.Luukko@lut.fi) to get this compiled with uc/OS-II 2.52
*
* Modifications by Julius Luukko 2003-03-06 (Julius.Luukko@lut.fi):
*
* - RAMPZ is also saved to the stack
*
* Modifications by Julius Luukko 2003-06-24 (Julius.Luukko@lut.fi):
*
* - RAMPZ is only saved to the stack if it is defined, i.e. with chips that have it
**********************************************************************************************************
*/

#define  OS_CPU_GLOBALS
#include "includes.h"




/*******************************************************************************
 * ELFELEJTETT TASZK VEGI OSTaskDel() HIVAS ESETEN FUT LE
 *
 *  Ez a fuggveny akkor hivodik meg, amikor egy taszk kodban RETURN utasitas
 *  van. Ez akkor szokott elofordulni, amikor egy single-shot taszk vegerol
 *  kifelejtjuk az OSTaskDel() hivast.
 *  A fuggveny torli az aktualis taszkot. Ezzel biztositva van az utemezes
 *  mukodokepessege a tovabbiakban.
 *
 *  A hivatalos uCOS forraskodban nincs ilyen fuggveny.
 *
 *  Naszaly Gabor
 *  naszaly@mit.bme.hu
 *******************************************************************************
 */
#if OS_TASK_DEL_MISSING_EN
void OSTaskDelMissing(INT8U first, INT8U second) {

	OSTaskDel(OS_PRIO_SELF);
}
#endif

/*
**********************************************************************************************************
*                                        INITIALIZE A TASK'S STACK
*
* Description: This function is called by either OSTaskCreate() or OSTaskCreateExt() to initialize the
*              stack frame of the task being created. This function is highly processor specific.
*
* Arguments  : task          is a pointer to the task code
*
*              pdata         is a pointer to a user supplied data area that will be passed to the task
*                            when the task first executes.
*
*              ptos          is a pointer to the top of stack. It is assumed that 'ptos' points to the
*                            highest valid address on the stack.
*
*              opt           specifies options that can be used to alter the behavior of OSTaskStkInit().
*                            (see uCOS_II.H for OS_TASK_OPT_???).
*
* Returns    : Always returns the location of the new top-of-stack' once the processor registers have
*              been placed on the stack in the proper order.
*
* Note(s)    : Interrupts are enabled when your task starts executing. You can change this by setting the
*              SREG to 0x00 instead. In this case, interrupts would be disabled upon task startup. The
*              application code would be responsible for enabling interrupts at the beginning of the task
*              code. You will need to modify OSTaskIdle() and OSTaskStat() so that they enable
*              interrupts. Failure to do this will make your system crash!
*
*              The AVR return stack is placed 64 bytes above the start of the (512 byte) stack. This
*              provides 32 levels of function call nesting which should be more than enough for most
*              applications (see the "read.me" file for more info).
*
**********************************************************************************************************
*/

OS_STK *OSTaskStkInit (void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt)
{
    INT8U  *stk;
    INT16U  tmp;

    opt     = opt;                          /* 'opt' is not used, prevent warning                       */
    stk     = (INT8U *)ptos;		    /* AVR return stack ("hardware stack")          		*/
    tmp     = (INT16U)task;

    /* "push" initial register values onto the stack */

#if OS_TASK_DEL_MISSING_EN

	/* Hagyomanyosan az egyes taszkok vermeben a legalso elem egy fuggveny
	 * mutato az adott taszk kodjat tartalmazo fuggvenyre. Ha ez ele
	 * beteszunk egy masik fuggveny mutatot, akkor elerhetjuk, hogy egy
	 * RET utasitas eseten (ami leggyakrabban single-shot taszk szervezesnel
	 * a fuggveny vegerol elfelejtett OSTaskDel() hivas eseten szokott
	 * elofordulni) egy altalunk definialt fuggveny fusson le.
	 * Lasd -> void OSTaskDelMissing(void)
	 *
	 * Ez nem resze a hagyomanyos uCOS-nek!!!
	 *
	 * Naszaly Gabor
	 * naszaly@mit.bme.hu
	 */

    *stk-- = (INT8U)(  (INT16U)OSTaskDelMissing      );
    *stk-- = (INT8U)( ((INT16U)OSTaskDelMissing) >> 8);

#endif

    *stk-- = (INT8U)tmp;                   /* Put task start address on top of stack          	        */
    *stk-- = (INT8U)(tmp >> 8);

    *stk-- = (INT8U)0x00;                   /* R0  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R1  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R2  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R3  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R4  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R5  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R6  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R7  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R8  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R9  = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R10 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R11 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R12 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R13 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R14 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R15 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R16 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R17 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R18 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R19 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R20 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R21 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R22 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R23 = 0x00                                               */
 	
    tmp    = (INT16U)pdata;
    *stk-- = (INT8U)tmp;                    /* Simulate call to function with argument                  */
    *stk-- = (INT8U)(tmp >> 8);		    /* R24, R25 contains argument pointer pdata 		*/

    *stk-- = (INT8U)0x00;                   /* R26 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R27 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R28 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R29 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R30 = 0x00                                               */
    *stk-- = (INT8U)0x00;                   /* R31 = 0x00                                               */
#ifdef RAMPZ
    *stk-- = (INT8U)0x00;                   /* RAMPZ = 0x00                                             */
#endif
    *stk-- = (INT8U)0x80;                   /* SREG = Interrupts enabled                                */
    return ((OS_STK *)stk);
}

/*$PAGE*/
#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
void OSTaskSwHook (void)
{
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}

#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif

/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OS_TCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_CPU_HOOKS_EN > 0 && OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}
#endif

