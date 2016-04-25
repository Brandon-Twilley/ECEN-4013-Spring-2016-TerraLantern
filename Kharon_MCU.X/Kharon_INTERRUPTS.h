/* 
 * File:   Kharon_INTERRUPTS.h
 * Author: romil
 *
 * Created on March 18, 2016, 2:00 PM
 */
/*TOC
 * UART Receive interrupt
 */

#ifndef KHARON_INTERRUPTS_H
#define	KHARON_INTERRUPTS_H
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************UART RECEIVE INTERRUPT******************************/
//WILL INTERRUPT FLOW AT PRIORITY 7 WHENEVER A CHARACTER IS RECEIVED.
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{

    //check for errors
    if ((_OERR == 1) || (_PERR == 1) || (_FERR == 1)) {
        _UARTEN = 0;
        Nop();
        _UARTEN = 1;
 
        UART_SEND("\nUART ENCOUNTERED A TRANSMISSION [RX] ERROR AND WAS RESET\r\n");
    }
    short thingy;
    thingy = U1RXREG;
    UARTrbuff[(strlen(UARTrbuff))] = thingy;
    
   
    _U1RXIF = 0; //clear the UART receive flag
 
}
/**********************END UART RECEIVE INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************UART2 RECEIVE INTERRUPT******************************/
//WILL INTERRUPT FLOW AT PRIORITY 7 WHENEVER A CHARACTER IS RECEIVED.
void __attribute__((interrupt, no_auto_psv)) _U2RXInterrupt(void)
{

//    U2STAbits.OERR = 0;//try to clear this
    //check for errors
    if ((U2RXINDEX) || (U2STAbits.PERR == 1) || (U2STAbits.FERR == 1) || (U2STAbits.OERR == 1)) {
        U2MODEbits.UARTEN = 0;
        Nop();
        U2MODEbits.UARTEN = 1;
 
        UART_SEND("\r\nUART2 ENCOUNTERED A TRANSMISSION [RX] ERROR AND WAS RESET\r\n");
    }
    
    //handle the large receive buffer

    short rxbyte;
    rxbyte = U2RXREG;
    UART2rbuff[(strlen(UART2rbuff))] = rxbyte; //equivalent of concatenating 'thingy'
    int i;
    for (i = 1; i < 6; ++i) {//bit shift everything to the left
        U2RX[i-1] = U2RX[i];
    }//bitshift everything to the left
    U2RX[5] = rxbyte;
//TODO
//    U2RX[0] = U2RX[1];
//    U2RX[1] = U2RX[2];
//    U2RX[2] = U2RX[3];
//    U2RX[3] = U2RX[4];
//    U2RX[4] = U2RX[5];
//    U2RX[5] = rxbyte;//put the lastest byte at the end
//    sprintf(UARTsbuff,"Current U2RX --> '%x,%x,%x,%x,%x,%x'\r\n",U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5]);
    IFS1bits.U2RXIF = 0; //clear the UART receive flag

}
/**********************END UART2 RECEIVE INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************KEY READER INTERRUPT******************************/
//WILL INTERRUPT FLOW AT PRIORITY 7 WHENEVER A CHARACTER IS RECEIVED.
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void) //CURRENTLY DISABLED
{
    //indicate change by blinking 3rd light
    UART_SEND("KEY PIN WAS CHANGED\r\n");
    //turn the 2nd light on if pin 4 is high, off if it's low.
        //the 3rd light should blink briefly on change.
        if(_RB1 == 1) { //poll pin 7
        _LATA1 = 1;
        if (_CNIF == 1) {
            UART_SEND("KEY READ: 1\r\n");
        }
        
        }

        
        else {
            if (_RB1 == 0) {
            UART_SEND("KEY READ: 0\r\n");
            }
            if (_LATA1 == 1) {
                _LATA1 = 0;

            }
        }
    
    _CNIF = 0; //clear the UART receive flag
}
/**********************END KEY READER INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************TIMER1 INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void) //timer 1 checks how long the portal is open
{

    //TMR1CNT is the current count in seconds.
    //TMR1CNTVAL is the maximum that TMR1CNT should ever be.
    //0 should be the minimum that TMR1CNT should ever be.
    int TMR_incr = 1;//the cooldown multiplier
    /*portal is on cooldown and is not done cooling down*/
    if ((currentPortal == -1) && (TMR1CNT > 0)) {
        TMR1CNT = TMR1CNT - TMR_incr; //decrement the timer value
//        UART_SEND("CD+\r\n");

    }

    /*portal is on cooldown and cooldown value is finished*/
    else if ((currentPortal == -1) && (TMR1CNT <= 0)) {//if TMR1CNT has got back down to 0 or less.
        TMR1CNT = 0;
        currentPortal = 0;
//        UART_SEND("CDD\r\n");
    }

    /*portal is open and max value hasn't been reached yet*/
    else if ((currentPortal > 0) && (TMR1CNT < TMR1CNTVAL)) {//tmr1cnt hasn't got to its max yet

        TMR1CNT = TMR1CNT + TMR_incr; //increment the timer value
//        UART_SEND("PO+\r\n");
    }

    /*portal is open and max value HAS been reached.*/
    else if ((currentPortal > 0) && (TMR1CNT >= TMR1CNTVAL)) {
        //cooldown needs to start
        currentPortal = -1;
//        UART_SEND("POTL\r\n");
        //portal needs to be collapsed
        portalCollapse();
    }
    else if (currentPortal == 0) {//then it's idling
        portalIdle();
        T1CONbits.TON = 0; // disable this interrupt timer. 
    }
    else {
        UART_SEND("Something weird is happening with the timer1 interrupt\r\n");
    }
    
    _T1IF = 0;
}
/**********************END TIMER1 INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************TIMER2 INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void) //timer 2 controls bluetooth heartbeat
{
    //heartbeat simulation
 
    if (TMR2CNT == 0) {
       UART2_SEND0x(BTheartbeatpacket,6);
       TMR2CNT = TMR2CNTVAL;
       if (debugg == 1) {
           UART_SEND("[BT]--> <3\r\n");
       }
       
    }
    
    else {
        --TMR2CNT;
        
    }
    if (BTlinkCNT == 0) {//then we've sent 3 heartbeats without an ack
        BTlink = 0; //we don't have a connection. 
        sprintf(UARTsbuff,"[BT]--> PIU Connection Lost\r\n");
        T2CONbits.TON = 0; //disable heartbeats 
    }
        
    else if (BTlink > 0) {//decrement the timer
        BTlink = 1;//but we at least had a connection
        --BTlinkCNT;
    }
    
    _T2IF = 0;
}
/**********************END TIMER2 INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************TIMER3 INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) //timer 3 is for IR packets
{
    if (TMR3CNT == 0) {
    
        //send Bluetooth packet
        //calculate the crc and UID
        BTirtxpacket[2] = UIDcount; //set the UID, which is set after each IR is sent.
        BTirtxpacket[5] = (0xff - BTirtxpacket[1] - BTirtxpacket[2] - BTirtxpacket[3] - BTirtxpacket[4]);
        UART2_SEND0x(BTirtxpacket,6);
        unsigned char tempchar = (0xff - PID[1] - currentSpell - currentStr - UIDcount);
        sprintf(UARTsbuff,"[IR]--> '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\n",PID[0],PID[1],currentSpell,currentStr,UIDcount,tempchar);
        //send IR packet
        _DISI = 1; //DISABLE ALL INTERRUPTS
        merge(currentSpell);
        DISICNT = 0; //reenable all interrupts//charliemark
        
        
    TMR3CNT = TMR3CNTVAL;
    _T3IF = 0;//clear the interrupt flag
    }
    else {
    TMR3CNT--;
    _T3IF = 0;
    }
}
/**********************END TIMER3 INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************TIMER4 INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _T4Interrupt(void) //timer 4 is dummy running at 1/64 of fcy
{
    if (TMR4CNT == 0) {//if the seconds counter has reached the predetermined value

    TMR4CNT = TMR4CNTVAL;

    }
    else {
    TMR4CNT--;//increment the timer 4 seconds counter
 
    }
   _T4IF = 0; //clear interrupt flag
}
/**********************END TIMER4 INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************TIMER5 INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _T5Interrupt(void) //timer 5 is dummy running at 1/64 of fcy
{
    
   _T5IF = 0; //clear interrupt flag
}
/**********************END TIMER5 INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************Ext INTERRUPT******************************/
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void) //SET INT0IF TO 1 TO CALL THIS FCN
{

   _INT0IF = 0; //clear interrupt flag
}
/**********************END External INTERRUPT******************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#endif	/* KHARON_INTERRUPTS_H */

