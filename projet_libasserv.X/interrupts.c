#include <xc.h>
#include "main.h"



/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs) User */
/* Guide for an up to date list of the available interrupt options.           */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* dsPIC33F Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt      _C1Interrupt                                           */
/* _IC1Interrupt       _DMA3Interrupt                                         */
/* _OC1Interrupt       _IC3Interrupt                                          */
/* _T1Interrupt        _IC4Interrupt                                          */
/* _DMA0Interrupt      _IC5Interrupt                                          */
/* _IC2Interrupt       _IC6Interrupt                                          */
/* _OC2Interrupt       _OC5Interrupt                                          */
/* _T2Interrupt        _OC6Interrupt                                          */
/* _T3Interrupt        _OC7Interrupt                                          */
/* _SPI1ErrInterrupt   _OC8Interrupt                                          */
/* _SPI1Interrupt      _DMA4Interrupt                                         */
/* _U1RXInterrupt      _T6Interrupt                                           */
/* _U1TXInterrupt      _T7Interrupt                                           */
/* _ADC1Interrupt      _SI2C2Interrupt                                        */
/* _DMA1Interrupt      _MI2C2Interrupt                                        */
/* _SI2C1Interrupt     _T8Interrupt                                           */
/* _MI2C1Interrupt     _T9Interrupt                                           */
/* _CNInterrupt        _INT3Interrupt                                         */
/* _INT1Interrupt      _INT4Interrupt                                         */
/* _ADC2Interrupt      _C2RxRdyInterrupt                                      */
/* _DMA2Interrupt      _C2Interrupt                                           */
/* _OC3Interrupt       _DCIErrInterrupt                                       */
/* _OC4Interrupt       _DCIInterrupt                                          */
/* _T4Interrupt        _DMA5Interrupt                                         */
/* _T5Interrupt        _U1ErrInterrupt                                        */
/* _INT2Interrupt      _U2ErrInterrupt                                        */
/* _U2RXInterrupt      _DMA6Interrupt                                         */
/* _U2TXInterrupt      _DPIN_LAISSEMA7Interrupt                               */
/* _SPI2ErrInterrupt   _C1TxReqInterrupt                                      */
/* _SPI2Interrupt      _C2TxReqInterrupt                                      */
/* _C1RxRdyInterrupt     PIN_LAISSE                                           */
/*                                                                            */
/* dsPIC33E Primary Interrupt Vector Names:                                   */
/*                                                                            */
/* _INT0Interrupt     _IC4Interrupt      _U4TXInterrupt                       */
/* _IC1Interrupt      _IC5Interrupt      _SPI3ErrInterrupt                    */
/* _OC1Interrupt      _IC6Interrupt      _SPI3Interrupt                       */
/* _T1Interrupt       _OC5Interrupt      _OC9Interrupt                        */
/* _DMA0Interrupt     _OC6Interrupt      _IC9Interrupt                        */
/* _IC2Interrupt      _OC7Interrupt      _PWM1Interrupt                       */
/* _OC2Interrupt      _OC8Interrupt      _PWM2Interrupt                       */
/* _T2Interrupt       _PMPInterrupt      _PWM3Interrupt                       */
/* _T3Interrupt       _DMA4Interrupt     _PWM4Interrupt                       */
/* _SPI1ErrInterrupt  _T6Interrupt       _PWM5Interrupt                       */
/* _SPI1Interrupt     _T7Interrupt       _PWM6Interrupt                       */
/* _U1RXInterrupt     _SI2C2Interrupt    _PWM7Interrupt                       */
/* _U1TXInterrupt     _MI2C2Interrupt    _DMA8Interrupt                       */
/* _AD1Interrupt      _T8Interrupt       _DMA9Interrupt                       */
/* _DMA1Interrupt     _T9Interrupt       _DMA10Interrupt                      */
/* _NVMInterrupt      _INT3Interrupt     _DMA11Interrupt                      */
/* _SI2C1Interrupt    _INT4Interrupt     _SPI4ErrInterrupt                    */
/* _MI2C1Interrupt    _C2RxRdyInterrupt  _SPI4Interrupt                       */
/* _CM1Interrupt      _C2Interrupt       _OC10Interrupt                       */
/* _CNInterrupt       _QEI1Interrupt     _IC10Interrupt                       */
/* _INT1Interrupt     _DCIEInterrupt     _OC11Interrupt                       */
/* _AD2Interrupt      _DCIInterrupt      _IC11Interrupt                       */
/* _IC7Interrupt      _DMA5Interrupt     _OC12Interrupt                       */
/* _IC8Interrupt      _RTCCInterrupt     _IC12Interrupt                       */
/* _DMA2Interrupt     _U1ErrInterrupt    _DMA12Interrupt                      */
/* _OC3Interrupt      _U2ErrInterrupt    _DMA13Interrupt                      */
/* _OC4Interrupt      _CRCInterrupt      _DMA14Interrupt                      */
/* _T4Interrupt       _DMA6Interrupt     _OC13Interrupt                       */
/* _T5Interrupt       _DMA7Interrupt     _IC13Interrupt                       */
/* _INT2Interrupt     _C1TxReqInterrupt  _OC14Interrupt                       */
/* _U2RXInterrupt     _C2TxReqInterrupt  _IC14Interrupt                       */
/* _U2TXInterrupt     _QEI2Interrupt     _OC15Interrupt                       */
/* _SPI2ErrInterrupt  _U3ErrInterrupt    _IC15Interrupt                       */
/* _SPI2Interrupt     _U3RXInterrupt     _OC16Interrupt                       */
/* _C1RxRdyInterrupt  _U3TXInterrupt     _IC16Interrupt                       */
/* _C1Interrupt       _USB1Interrupt     _ICDInterrupt                        */
/* _DMA3Interrupt     _U4ErrInterrupt    _PWMSpEventMatchInterrupt            */
/* _IC3Interrupt      _U4RXInterrupt     _PWMSecSpEventMatchInterrupt         */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.  There is no Alternate Vector or 'AIVT' for the 33E family.   */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <C30 compiler instal directory>/doc directory for the latest compiler      */
/* release.  For XC16, refer to the MPLAB XC16 C Compiler User's Guide in the */
/* <XC16 compiler instal directory>/doc folder.                               */
/*                                                                            */
/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/



/* Primary Exception Vector handlers:
These routines are used if INTCON2bits.ALTIVT = 0.
All trap service routines in this file simply ensure that device
continuously executes code within the trap service routine. Users
may modify the basic framework provided here to suit to the needs
of their application. */
//================================================================
// OSCFAIL: Oscillator Failure Trap Status bit

void __attribute__((interrupt, no_auto_psv)) _OscillatorFail(void) {
    INTCON1bits.OSCFAIL = 0; //Clear the trap flag
}
//================================================================
// ADDRERR: Address Error Trap Status bit
// DS read access when DSRPAG = 0x000 will force an Address Error trap.

void __attribute__((interrupt, no_auto_psv)) _AddressError(void) {
    INTCON1bits.ADDRERR = 0; //Clear the trap flag

}
//================================================================
// STKERR: Stack Error Trap Status bit

void __attribute__((interrupt, no_auto_psv)) _StackError(void) {
    INTCON1bits.STKERR = 0; //Clear the trap flag

}
//================================================================
// MATHERR: Math Error Status bit

void __attribute__((interrupt, no_auto_psv)) _MathError(void) {
    INTCON1bits.MATHERR = 0; //Clear the trap flag

}
//================================================================
// DMACERR: DMAC Trap Flag bit

void __attribute__((interrupt, no_auto_psv)) _DMACError(void) {
    INTCON1bits.DMACERR = 0; //Clear the trap flag

}
//================================================================

void __attribute__((interrupt, no_auto_psv)) _ReservedTrap7(void) {
    INTCON1bits.DMACERR = 0; //Clear the trap flag

}
//================================================================
//================================================================
// Interrupt Vector Not Trap
//================================================================
//================================================================
//===================================================================

void __attribute__((interrupt, no_auto_psv)) _U1ErrInterrupt(void) {
    IFS4bits.U1EIF = 0; // Clear interrupt flag before returning
}
//===================================================================

void __attribute__((interrupt, no_auto_psv)) _U2ErrInterrupt(void) {
    IFS4bits.U2EIF = 0; // Clear interrupt flag before returning
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _INT0Interrupt(void) {
    IFS0bits.INT0IF = 0; //Clear INT0 interrupt flag
}
//============================================================================
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _INT1Interrupt(void) {
    IFS1bits.INT1IF = 0; //Clear INT1 interrupt flag
}
//============================================================================
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _INT2Interrupt(void) {
    IFS1bits.INT2IF = 0; //Clear INT2 interrupt flag
}
//============================================================================
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _IC1Interrupt(void) {
    IFS0bits.IC1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _IC2Interrupt(void) {
    IFS0bits.IC2IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _IC7Interrupt(void) {
    IFS1bits.IC7IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _IC8Interrupt(void) {
    IFS1bits.IC8IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _OC1Interrupt(void) {
    IFS0bits.OC1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _OC2Interrupt(void) {
    IFS0bits.OC2IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _OC3Interrupt(void) {
    IFS1bits.OC3IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _OC4Interrupt(void) {
    IFS1bits.OC4IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _T1Interrupt(void) {
    IFS0bits.T1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _T5Interrupt(void) {
    IFS1bits.T5IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA0Interrupt(void) {
    IFS0bits.DMA0IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA1Interrupt(void) {
    IFS0bits.DMA1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA2Interrupt(void) {
    IFS1bits.DMA2IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA3Interrupt(void) {
    IFS2bits.DMA3IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA4Interrupt(void) {
    IFS2bits.DMA4IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA5Interrupt(void) {
    IFS3bits.DMA5IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA6Interrupt(void) {
    IFS4bits.DMA6IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _DMA7Interrupt(void) {
    IFS4bits.DMA7IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _PMPInterrupt(void) {
    IFS2bits.PMPIF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _SPI1Interrupt(void) {
    IFS0bits.SPI1IF = 0; //Clear Flag
}
//============================================================================

//void __attribute__((interrupt, shadow, no_auto_psv)) _SPI2ErrInterrupt(void) {
//    IFS2bits.SPI2EIF = 0; //Clear Flag
//}

//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _C1RxRdyInterrupt(void) {
    IFS2bits.C1RXIF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _C1Interrupt(void) {
    IFS2bits.C1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _SI2C1Interrupt(void) {
    IFS1bits.SI2C1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _MI2C1Interrupt(void) {
    IFS1bits.MI2C1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _C1TxReqInterrupt(void) {
    IFS4bits.C1TXIF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _QEI1Interrupt(void) {
    // Sources of QEI interrupts:
    // Position counter overflow or underflow event
    // Velocity counter overflow or underflow event
    // Position counter initialization process complete
    // Position counter greater than or equal compare interrupt
    // Position counter less than or equal compare interrupt
    // Index event interrupt
    // Home event interrupt
    //The QEI Status register (QEIxSTAT) contains the individual interrupt enable bits and the
    //corresponding interrupt status bits for each interrupt source. A status bit indicates that an
    //interrupt request has occurred. The module reduces all of the QEI interrupts to a single interrupt
    //signal to the interrupt controller module

    IFS3bits.QEI1IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _QEI2Interrupt(void) {
    // Sources of QEI interrupts:
    // Position counter overflow or underflow event
    // Velocity counter overflow or underflow event
    // Position counter initialization process complete
    // Position counter greater than or equal compare interrupt
    // Position counter less than or equal compare interrupt
    // Index event interrupt
    // Home event interrupt
    //The QEI Status register (QEIxSTAT) contains the individual interrupt enable bits and the
    //corresponding interrupt status bits for each interrupt source. A status bit indicates that an
    //interrupt request has occurred. The module reduces all of the QEI interrupts to a single interrupt
    //signal to the interrupt controller module

    IFS4bits.QEI2IF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _RTCCInterrupt(void) {
    IFS3bits.RTCIF = 0; //Clear Flag
}
//============================================================================

void __attribute__((interrupt, shadow, no_auto_psv)) _CRCInterrupt(void) {
    IFS4bits.CRCIF = 0; //Clear Flag
}
//=====================================================================