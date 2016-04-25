

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/******************************MAIN CONFIG************************************/
/* 
 * File:   PIC24HJ128GP502_CONFIG.h
 * Author: romil
 *
 * Created on March 18, 2016, 1:09 PM
 *
 * CHANGELOG
 *
 * 3/18/16 @ 2pm - First final version 
 */



////////////////////////////////////////////////////////////////////////
/*config definitions TOC
 * Auto generated stuff
 * Oscillator config
 * compliler includes
 */
//////////////////////////////////
#ifndef Kharon_CONFIG_H
#define	Kharon_CONFIG_H

/******************************AUTO GENERATED CONFIG BITS************************************/
// PIC24HJ128GP502 Configuration Bit Settings
// FBS
#pragma config BWRP = WRPROTECT_OFF     // Boot Segment Write Protect (Boot Segment may be written)
#pragma config BSS = NO_FLASH           // Boot Segment Program Flash Code Protection (No Boot program Flash segment)
#pragma config RBS = NO_RAM             // Boot Segment RAM Protection (No Boot RAM)

// FSS
#pragma config SWRP = WRPROTECT_OFF     // Secure Segment Program Write Protect (Secure segment may be written)
#pragma config SSS = NO_FLASH           // Secure Segment Program Flash Code Protection (No Secure Segment)
#pragma config RSS = NO_RAM             // Secure Segment Data RAM Protection (No Secure RAM)

// FGS
#pragma config GWRP = OFF               // General Code Segment Write Protect (User program memory is not write-protected)
#pragma config GSS = OFF                // General Segment Code Protection (User program memory is not code-protected)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Mode (Internal Fast RC (FRC))
#pragma config IESO = ON                // Internal External Switch Over Mode (Start-up device with FRC, then automatically switch to user-selected oscillator source when ready)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Source (Primary Oscillator Disabled)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function (OSC2 pin has clock out function)//if this is off, the osc2 is 7mhz, if it is ON, its an i/o pin.
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow Only One Re-configuration)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Both Clock Switching and Fail-Safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler (1:32,768)
#pragma config WDTPRE = PR128           // WDT Prescaler (1:128)
#pragma config WINDIS = OFF             // Watchdog Timer Window (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog timer enabled/disabled by user software)

// FPOR
#pragma config FPWRT = PWR128           // POR Timer Value (128ms)
#pragma config ALTI2C = OFF             // Alternate I2C  pins (I2C mapped to SDA1/SCL1 pins)

// FICD
#pragma config ICS = PGD1               // Comm Channel Select (Communicate on PGC1/EMUC1 and PGD1/EMUD1)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG is Disabled)


/*********************END AUTO GENERATED CONFIG BITS************************************/
//function declarations
void U1CHAN();
int UART_SEND();
void merge(char spell);
void BTINIT();
int UART_kDebug();
//Define constants before we include libpic30 to enable __delay functions.
#define FOSC 7372800 //fast oscillator frequency with OSCTUN = 0
#define FCY (FOSC/2)//FCY= instructions per second
#define U1BAUD 38400
#define U2BAUD 38400
#define U1BRGVAL ((FCY/U1BAUD)/16)-1
#define U2BRGVAL ((FCY/U2BAUD)/16)-1


#include <stdio.h> //standard C functions and definitions
#include <stdlib.h> //standard C functions and definitions
#include <string.h> // includes string functions like strcmp & strcpy
#include <stdint.h> //int32 and stuff
#include <stdbool.h> //true and false and stuff
#include <p24HJ128GP502.h> //pic header file. literally makes the pic work
#include <libpic30.h> //includes special functions like __delay_ms & __delay_us
#include <libq.h> // includes special math functions




#endif	/* PIC24HJ128GP502_CONFIG_H */
/******************************END MAIN CONFIG************************************/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
/******************************RYAN CONFIG************************************/
/*
 * File:   ryan.h
 * Author: Ryan
 *
 * Created on March 24, 2016, 10:48 PM
 */




//LCD global variables
static unsigned short Low_Nibble, High_Nibble, RS;

//RGB global variables
static uint16_t pwmbuffer[24] = {0};


//ADC global variables
static uint16_t leftADCvalue;
static uint16_t rightADCvalue;
static uint8_t leftKey;
static uint8_t rightKey;
static uint8_t currentWorld;


/******************************************************************************/
/* Function Declarations                                                      */
/******************************************************************************/

//LCD
void write_LCD_Nibble(unsigned short);
void write_LCD_Data(unsigned short);
void write_LCD_Cmd(unsigned short);
void initialize_LCD();
void position_LCD(unsigned short, unsigned short);
void write_LCD_Text(char *);
void clear_LCD();

//RGB
void initialize_RGB();
void write_RGB();
void write_RGB_Animated();
void clear_RGB();
void setPWMOneColor(uint16_t, uint16_t, uint16_t);

//ADC
void initialize_ADC();
void poll_ADC();
void setKey(uint8_t, uint16_t);
void setWorld();

//WORLDS
void noWorldVisuals();
void windWorldVisuals();
void waterWorldVisuals();
void fireWorldVisuals();
void lightningWorldVisuals();
void iceWorldVisuals();
void plasmaWorldVisuals();
/******************************END RYAN CONFIG************************************/
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////


//////////////////TABLE OF CONTENTS//////////////////////////////////////////////////////
/*config function TOC
 * OSCILLATOR CONFIG
 * PIN CONFIG
 * UART CONFIG
 * INTERRUPT CONFIG
 */
void Kharon_CONFIG() {

    // Configure Oscillator to operate the device at 40 MHz

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
    /***************************OSCILLATOR CONFIG*******************************/
OSCTUN = 0; // Tune FRC oscillator, if FRC is used, 0x0 is +0% speed (7.372800MHz)
_SWDTEN = 0; // Disable Watch Dog Timer
//the following are defined outside of Kharon_CONFIG()
//#define FOSC 7372800 //fast oscillator frequency with OSCTUN = 0
//#define FCY (FOSC/2) //FCY= instructions per second
//#define BAUDRATE 9600
//#define BRGVAL ((FCY/BAUDRATE)/16)-1
/*******************END OSCILLATOR CONFIG**********************************************/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/*******************PIN CONFIG**********************************************/
//IR pins
//#define _ir1 _RA0 //pin definitions // read from ports
//#define _ir2 _RA1
//#define _ir3 _RB0
//#define _ir4 _RB1
//
//#define _irOut1 _LATA0 //writing to these
//#define _irOut2 _LATA1
//#define _irOut3 _LATB0
//#define _irOut4 _LATB1
//
//#define _irOn1 _TRISA0 // 0 is output 1 is input / sets direction
//#define _irOn2 _TRISA1
//#define _irOn3 _TRISB0
//#define _irOn4 _TRISB1

//configure all ANx pins to be digital
AD1PCFGL = 0xFFFF; //1 is digital, 0 is analog
//RYAN pins
#define KeyInLDIR TRISAbits.TRISA0 //the direction of LeftKey
#define KeyInRDIR TRISAbits.TRISA1 //the direction of RightKey
#define KeyInL LATAbits.LATA0 //the value of Leftkey
#define KeyInR LATAits.LATA1 //the value of rightkey
#define KeyInL_AN AD1PCFGLbits.PCFG0 //an direction for left key
#define KeyInR_AN AD1PCFGLbits.PCFG1 //an direction for right key


#define LCD_Clk_PinDIR TRISBbits.TRISB10
#define LCD_Data_PinDIR TRISBbits.TRISB12
#define LCD_Enable_PinDIR TRISBbits.TRISB11

#define LCD_Clk_Pin LATBbits.LATB10
#define LCD_Data_Pin LATBbits.LATB12
#define LCD_Enable_Pin LATBbits.LATB11


#define RGB_Clk_PinDIR TRISBbits.TRISB13
#define RGB_Data_PinDIR TRISBbits.TRISB14
#define RGB_Lat_PinDIR TRISBbits.TRISB15

#define RGB_Clk_Pin LATBbits.LATB13
#define RGB_Data_Pin LATBbits.LATB14
#define RGB_Lat_Pin LATBbits.LATB15


//output pins
#define DEBUGPIN _RP1R
#define SOMOPIN _RP4R
#define SLAVEBTPIN _RP0R
#define U1TEN 0b00011 //0b00011 is the sequence for uart1 transmit channel
#define U2TEN 0b00101//0b00101 is the sequence for uart2 transmit channel
#define UVOID 0b00000//0b00000 is sequence for null channel

#define BTKEY _LATA2 //bluetooth at mode
#define BTPWR _LATA4 //bluetooth power
#define BTKEYDIR _TRISA2 //bt at mode direction
#define BTPWRDIR _TRISA4 //bt pwr direction


BTPWRDIR = 0;//set output to BTPOWER to output
BTKEYDIR = 0;//BTKEY default is output
BTKEY = 0;//BTKEY default is off for autocnt
BTPWR = 1;//set output to BTPOWER to off initially



//RP15R is the register for reprogrammable pin 15, and 0b00011 ties that pin to the uart transmit register.
DEBUGPIN = U1TEN; //configure UART transmit to debug bluetooth
SLAVEBTPIN = U2TEN; //configure UART2 transmit to slave Bluetooth
//U1RXR is the register for input uart recieve, and 0b01111 ties that register to pin 14
_U1RXR = 0b00010; //configure UART recieve to pin HARDCODED TO pin 2//charliemark
_U2RXR = 0b00011; //configure UART2 receive to pin HARDCODED TO PIN 3//bluetooth module



/*******************END PIN CONFIG**********************************************/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/****************************UART CONFIG*****************************************/
//uart 1
_STSEL = 0; // 1 Stop bit
_PDSEL = 0; // No Parity, 8 data bits
_ABAUD = 0; // Auto-Baud Disabled
_BRGH = 0; // Low Speed mode (16 clock cycles per thing)
U1BRG = U1BRGVAL; // BAUD Rate generator register. 9600 for bluetooth.

_UARTEN = 1; // Enable UART (clearing this resets all flags related to UART)
_UTXEN = 1; // Enable UART TX (only disable this if you want to prevent transmission)


//uart 2
//UART 2
U2MODEbits.STSEL = 0; // 1 Stop bit
U2MODEbits.PDSEL = 0; // No Parity, 8 data bits
U2MODEbits.ABAUD = 0; // Auto-Baud Disabled
U2MODEbits.BRGH = 0; // Low Speed mode (16 clock cycles per thing)
U2BRG = U2BRGVAL; // BAUD Rate generator register. 9600 for bluetooth.

U2MODEbits.UARTEN = 1; // Enable UART (clearing this resets all flags related to UART)
U2STAbits.UTXEN = 1; // Enable UART TX (only disable this if you want to prevent transmission)
//char UART_REC[720];
/*******************END UART CONFIG**********************************************/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/**********************************INTERRUPT CONFIG*******************************************/
//UART 1
_URXISEL = 0b00; // configure UART receive interrupt to interrupt if there is
        //      anything is transferred from the UxRSR register to the receive buffer

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/**********************************UART INTERRUPTS*******************************************/
_U1RXIP = 5; //configure UART receive interrupt as priority 7 (highest)
_U1RXIF = 0; //initialize UART receive interrupt flag as cleared
_U1RXIE = 1; //configure UART receive interrupt enabled
_U2RXIP = 6; //configure UART2 receive interrupt as priority 7 (highest)
_U2RXIF = 0; //initialize UART2 receive interrupt flag as cleared
_U2RXIE = 1; //configure UART2 receive interrupt enabled

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/**********************************CHANGE NOTIFICATION CONFIG*******************************************/
/*DISABLED*/
//_CN5IE = 1; //configure pin 5 (latb1) to have change notification interrupts enabled.
//_CNIE = 1; //enable change notice interrupts
//_CNIP = 7; //configure change notification to have priority 6
//_CNIF = 0; //clear change notice flag
//CNPU1 = 0xff; //set pull up resistors
//CNPU2 = 0xff; //set pull ups



///////////////////////TIMER INTERRUPTS////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE TIMER 1 INTERRUPTS**************************************/
T1CONbits.TON = 0; //disable the timer
_T1IP = 5; //set interrupt priority
_T1IF = 0; //clear the flag
_T1IE = 1; //enable timer1 interrupts
_TCS = //timer clock source needs to be internal oscillator
_TSYNC = 0; //timer synchronozation bit...(0 == internal)
_TGATE = 0; //gated timer.. 0 is not gated.
//timer period
_TCKPS = 0b11; //timer clock prescaler... every 32 clock cycles, increment 1 per 256 instructions
TMR1 = 0x00;//clear the timer register
PR1 = 28800; //set the period to 1 second
//28800 = 1 second --> 28800*256 = 7372800.. 7.37MHz clock speed

/**********************END CONFIGURE TIMER 1 INTERRUPTS**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE TIMER 2 INTERRUPTS**************************************/
//configure timer 2 (BLUETOOTH HEARTBEAT)
_T2IP = 5; //set interrupt priority
_T2IF = 0; //clear the flag
_T2IE = 1; //enable timer1 interrupts
T2CONbits.TCS = 0; // clock source == internal oscillator
T2CONbits.TCKPS = 0b11;//prescaler every 256 clock cycles
T2CONbits.TGATE = 0;//gate off
T2CONbits.TON = 0;//disable timer 2
TMR2 = 0x00; //clear timer register
PR2 = 14400     ;//set the interrupt period ... 1 second = 14400
/**********************END CONFIGURE TIMER 2 INTERRUPTS**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE TIMER 3 INTERRUPTS**************************************/
_T3IP = 5; //set interrupt priority
_T3IF = 0; //clear the flag
_T3IE = 1; //enable timer1 interrupts
T3CONbits.TCS = 0; // clock source == internal oscillator
T3CONbits.TCKPS = 0b11;//prescaler every 256 clock cycles
T3CONbits.TGATE = 0;//gate off
T3CONbits.TON = 0;//disable timer 3
TMR3 = 0x00; //clear timer register
PR3 = 1440;//set the interrupt period to 1/10th second -> 7372800/2/256(prescaler)/10
/**********************END CONFIGURE TIMER 3 INTERRUPTS**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE TIMER 4 INTERRUPTS**************************************/
_T4IP = 5; //set interrupt priority
_T4IF = 0; //clear the flag
_T4IE = 1; //enable timer1 interrupts
T4CONbits.TCS = 0; // clock source == internal oscillator
T4CONbits.TCKPS = 0b11;//prescaler every 256 clock cycles
T4CONbits.TGATE = 0;//gate off
T4CONbits.TON = 0;//enable timer 4
TMR4 = 0x00; //clear timer register
PR4 = 144; //set the interrupt period to 1/100 seconds
/**********************END CONFIGURE TIMER 4 INTERRUPTS**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE TIMER 5 INTERRUPTS**************************************/
_T5IP = 2; //set interrupt priority
_T5IF = 1; //clear the flag
_T5IE = 1; //enable timer1 interrupts
T5CONbits.TCS = 0; // clock source == internal oscillator
T5CONbits.TCKPS = 0b11;//prescaler every 256 clock cycles
T5CONbits.TGATE = 0;//gate off
T5CONbits.TON = 1;//enable timer 5
TMR5 = 0x00; //clear timer register
PR5 = 65535; //set the interrupt period
/**********************END CONFIGURE TIMER 5 INTERRUPTS**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*******************************END INTERRUPT CONFIG*************************************************/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/**********************CONFIGURE ryan**************************************/
initialize_LCD();
initialize_RGB();
initialize_ADC();
/**********************END CONFIGURE ryan**********************************/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

__delay_us(104);/* wait at least 104 usec (1/9600) before sending first char */






}//end Kharon_CONFIG function
////////////////////////////////////////////////////////////////////////





