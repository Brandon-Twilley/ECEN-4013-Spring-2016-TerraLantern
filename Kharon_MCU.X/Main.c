/* 
 * File:   Main.c
 * Author: romil
 *
 * Created on March 11, 2016, 9:27 PM
 */
//const char nullCHAR = '0x00';

    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////
    /**********************DECLARE GLOBAL VARIABLES****************************/
    static char UARTsbuff[255] = "";
   
//    static int rFlag = 0; //this flag goes high when there's something in the receive buffer
    static char UARTrbuff[255] = "";
    static char UART2rbuff[255] = "";
//    static char isATMODE = 1; //flag determining whether or not master hc05 is in atmode
    static unsigned int rbuffCounter = 0;
    //static int i = 0;
    static int debugg = 1;
    static int fancyGFX = 1;
    static int TMR1MAX = 60;//max portal open time
    static int TMR1CNTVAL = 60; //initialize Timer 1 counter value. - # seconds to keep the portal open
    static int TMR2CNTVAL = 5;//NEEDS TO BE 5 //initialize Timer 4 counter value. - # seconds between bluetooth heartbeat
    static int TMR3CNTVAL = 1; //initialize Timer 3 counter value. - # seconds between sending IR packet
    static int TMR4CNTVAL = 5; //initialize Timer 4 counter value. - # seconds between execution of timer4 (wildcard counter)
    static int TMR1CNT = 0; //initialize Timer 1 counter.
    static int TMR2CNT = 1; //initialize Timer 2 counter.
    static int TMR3CNT = 1; //initialize Timer 3 counter.
    static int TMR4CNT = 5; //initialize Timer 4 counter.

    static char badsong = 0x07;
    static int COUNTDOWNDEBUGG = 0;
    static unsigned char PID[3] = "@@"; //player id. stored when connected with PIU.
    static char PL_EFFECT = '@'; 
    static char PL_HEALTH = 114;
    static char PL_STATUS = '@';
    static unsigned char UIDcount = 0;
    static int portal1 = -1;
    static int portal2 = -1;
    static unsigned char currentSpell = 0x00; //the spell that IR will send
    static unsigned char currentStr = 0;
    static unsigned char U2RX[6]; //bluetooth receive buffer
//    static char U2RXing; //if == '1', then we found a good packet, and are processing it.
    static int U2RXINDEX; //bluetooth receive buffer index 
    static int U2SENDING = 0; //0 when uart channel is free, 1 when data is sending
    static int U1SENDING = 0; //0 when uart channel is free, 1 when data is sending
    static int currentPortal = 0; //indicates which portal is OPEN
    static int BTreceivecounter = 0;
    static int BTlink = 0;
    static int BTlinkCNT = 0;
//    static int BTlinking = 0;
    static char BTheartbeatpacket[6] = {'\x3c', '\x00','\x00','\x00','\x00','\xff'}; //hard coded heartbeat
    static char BTconnectpacket[6] = {'\x3c', '\x01','\x02','\x02','\x06','\xf4'};//start::connectfcn::0x0000=ID::0x00=red::ff-01=crc
 //   static char BTackpacket[6] = {'\x3c', '\xff','\x00','\x00','\x00','\x00'};
    static char BTirtxpacket[6] = {'\x3c', '\x02','?','\x00','\x00','?'}; //MUST CALCULATE UNI->char 3 AND CRC->char 6
//    static char BT_PIUADDR[15] = "98d3,31,8060f1";//address of the piu. MUST REPROGRAM HC-05 TO AUTO CONNECT TO NEW ONE


    /**********************END DECLARE VARIABLES*******************************/
    ////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////

    #include "Kharon_CONFIG.h" //configures everything so that it can be used
    #include "audio_declaration.h"
    #include "audio_initialization.h" //set up audio structs
    #include "RGB_animations.h"
    #include "portals.h"
    #include "Kharon_FCN.h" //contains necessary functions
    #include "Kharon_INTERRUPTS.h" //contains interrupt functions

  //other includes
    #include "ryan.h"
    #include "Ir_complete.h"
   

int main() {


    Kharon_CONFIG(); //configure the PIC
    bootMSG(); //check for seg fault and send boot up message to BT
    
    while (1) {
        
       //disable some shit
       
        idleProcess();
              
    }//end of while


}//end of main