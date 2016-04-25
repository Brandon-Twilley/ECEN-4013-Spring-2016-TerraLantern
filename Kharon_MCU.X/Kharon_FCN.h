/* 
 * File:   Kharon_FCN.h
 * Author: romil
 *
 * Created on March 18, 2016, 2:14 PM
 */


#ifndef KHARON_FCN_H
#define	KHARON_FCN_H

#include "audio_declaration.h"


int UART_SEND(char* sendString) {
    U1SENDING = 1;
    int sendLength = strlen(sendString);
    char sendChar;
    int i;
    //for loop goes through the passed string until no more is left
    for (i = 0; i < sendLength; i++) {
        sendChar = sendString[i];
        //check to see if transmission buffer is full
        if (_UTXBF == 0) {//if it's not full, write the character to send buffer
            U1TXREG = sendChar; 
        }
        else {//if it's full, then do this iteration again
            i--;
            continue; 
        }


    }//end for
    U2SENDING = 0;
    return 0;
}//end UART_SEND

int UART2_SEND(char* sendString) {
    U2SENDING = 1;
    int sendLength = strlen(sendString);
    char sendChar;
    //for loop goes through the passed string until no more is left
    int i;
    for (i = 0; i < sendLength; ++i) {
        sendChar = sendString[i];
        //check to see if transmission buffer is full
        if (U2STAbits.UTXBF == 0) {//if it's not full, write the character to send buffer
            U2TXREG = sendChar;
        }
        else {//if it's full, then do this iteration again
            --i;
            continue;
        }


    }//end for
    U2SENDING = 0;
    return 0;
}//end UART2_SEND

int UART_SEND0x(const char* sendString, int stringSize) {

    int sendLength = stringSize;
    int i;
    char sendChar;
    //for loop goes through the passed string until no more is left
    for (i = 0; i < sendLength; i++) {
        sendChar = sendString[i];
        //check to see if transmission buffer is full
        if (_UTXBF == 0) {//if it's not full, write the character to send buffer
            U1TXREG = sendChar;
        }
        else {//if it's full, then do this iteration again
            i--;
            continue;
        }


    }//end for

    return 0;
}//end UART_SEND0x

int UART2_SEND0x(char* sendString, int stringSize) {

    int sendLength = stringSize;
    int i;
    char sendChar;
    //for loop goes through the passed string until no more is left
    for (i = 0; i < sendLength; i++) {
        sendChar = sendString[i];
        //check to see if transmission buffer is full
        if (U2STAbits.UTXBF == 0) {//if it's not full, write the character to send buffer
            U2TXREG = sendChar;
        }
        else {//if it's full, then do this iteration again
            i--;
            continue;
        }


    }//end for

    return 0;
}//end UART_SEND0x

int LATCTRL() {
        char tempCMD[25];
        strcpy(tempCMD,"LATCTRL ");
        UART_SEND("Command: '");
        UART_SEND(tempCMD);
        UART_SEND("'\r\n");

//////////////////////////Command parsing/////////////////////////////////////
        //delete LATCTRL from the string
        strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "LATCTRL" from command

        //grab the pin name from the string
        char pinname[4] = "\0\0\0\0"; //4 bytes.. 1 byte for null character.
        memcpy(pinname,UARTrbuff,3);
        strcpy(UARTrbuff,(UARTrbuff+4)); //delete pin name from command...
        //+4 = 3 characters in pin name + followed by a space

        UART_SEND("Pin name: '");
        UART_SEND(pinname);
        UART_SEND("'\r\n");


        //grab the number at the end and store it in tempval
        char tempval = 0x7E;

        tempval = UARTrbuff[0];
        strcpy(UARTrbuff,UARTrbuff+1); //delete pin value from command, not counting space (was deleted with kD$)

        char tempstring[25];
        memset(tempstring,0x00,sizeof(tempstring));
        strcat(tempstring,"Value: '");
        tempstring[strlen(tempstring)] = tempval;
        strcat(tempstring,"'\r\n");
        UART_SEND(tempstring);

        memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
        //UART_SEND("rbuff cleared\r\n");

        int val; //convert tempval from char to int.
        if (tempval==49) {//then the input was 1
            val = 1;
        }
        else if (tempval == 48) {//then the input was 0
            val = 0;
        }
        else {
            UART_SEND("value not recognized\r\n");
            memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
            //UART_SEND("rbuff cleared\r\n");
            return 1; //invalid command
        }

//////////////////////////actions/////////////////////////////////
       if (strcmp(pinname,"RA0")==0) {
            UART_SEND("PIN RA0 SET\r\n");
            _LATA0 = val;
        }
        else if (strcmp(pinname,"RA1")==0) {
            UART_SEND("PIN RA1 SET\r\n");
            _LATA1 = val;
        }
         else if (strcmp(pinname,"RA2")==0) {
            UART_SEND("PIN RA2 SET\r\n");
            _LATA2 = val;
        }
         else if (strcmp(pinname,"RA3")==0) {
            UART_SEND("PIN RA3 SET\r\n");
            _LATA3 = val;
        }
        else if (strcmp(pinname,"RA4")==0) {
            UART_SEND("PIN RA4 SET\r\n");
            _LATA4 = val;
        }
        else if (strcmp(pinname,"RB0")==0) {
            UART_SEND("PIN RB0 SET\r\n");
            _LATB0 = val;
        }
        else if (strcmp(pinname,"RB1")==0) {
            UART_SEND("PIN RB1 SET\r\n");
            _LATB1 = val;
        }
        else if (strcmp(pinname,"RB2")==0) {
            UART_SEND("PIN RB2 SET\r\n");
            _LATB2 = val;
        }
        else if (strcmp(pinname,"RB3")==0) {
            UART_SEND("PIN RB3 SET\r\n");
            _LATB3 = val;
        }
        else if (strcmp(pinname,"RB4")==0) {
            UART_SEND("PIN RB4 SET\r\n");
            _LATB4 = val;
        }
        else {
            UART_SEND("pinname not recognized\r\n");
        }
        return 0; //return with no errors
        
} //control lat values

int TRISCTRL() {
     //if the string has "LATCTRL" in it
                char tempCMD[25];
                strcpy(tempCMD,"TRISCTRL ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete TRISCTRL  from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "TRISCTRL " from command

                //grab the pin name from the string
                char pinname[4] = "\0\0\0\0"; //4 bytes.. 1 byte for null character.
                memcpy(pinname,UARTrbuff,3);
                strcpy(UARTrbuff,(UARTrbuff+4)); //delete pin name from command...
                //+4 = 3 characters in pin name + followed by a space

                UART_SEND("Pin name: '");
                UART_SEND(pinname);
                UART_SEND("'\r\n");


                //grab the number at the end and store it in tempval
                char tempval = 0x7E;

                tempval = UARTrbuff[0];
                strcpy(UARTrbuff,UARTrbuff+1); //delete pin value from command, not counting space (was deleted with kD$)

                char tempstring[25];
                memset(tempstring,0x00,sizeof(tempstring));
                strcat(tempstring,"Value: '");
                tempstring[strlen(tempstring)] = tempval;
                strcat(tempstring,"'\r\n");
                UART_SEND(tempstring);

                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                //UART_SEND("rbuff cleared\r\n");

                int val; //convert tempval from char to int.
                if (tempval==49) {//then the input was 1
                    val = 1;
                }
                else if (tempval == 48) {//then the input was 0
                    val = 0;
                }
                else {
                    UART_SEND("value not recognized\r\n");
                    memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                    //UART_SEND("rbuff cleared\r\n");
                    return 1; //invalid command
                }

        //////////////////////////actions/////////////////////////////////
                if (strcmp(pinname,"RA0")==0) {
                    UART_SEND("PIN RA0 SET\r\n");
                    _TRISA0 = val;
                }
                else if (strcmp(pinname,"RA1")==0) {
                    UART_SEND("PIN RA1 SET\r\n");
                    _TRISA1 = val;
                }
                 else if (strcmp(pinname,"RA2")==0) {
                    UART_SEND("PIN RA1 SET\r\n");
                    _TRISA2 = val;
                }
                 else if (strcmp(pinname,"RA3")==0) {
                    UART_SEND("PIN RA1 SET\r\n");
                    _TRISA3 = val;
                }
                else if (strcmp(pinname,"RB0")==0) {
                    UART_SEND("PIN RB0 SET\r\n");
                    _TRISB0 = val;
                }
                else if (strcmp(pinname,"RB1")==0) {
                    UART_SEND("PIN RB1 SET\r\n");
                    _TRISB1 = val;
                }
                else if (strcmp(pinname,"RB2")==0) {
                    UART_SEND("PIN RB2 SET\r\n");
                    _TRISB2 = val;
                }
                else if (strcmp(pinname,"RB3")==0) {
                    UART_SEND("PIN RB3 SET\r\n");
                    _TRISB3 = val;
                }
                else {
                    UART_SEND("pinname not recognized\r\n");
                }
                return 0; //return with no errors

} //control pin directions

void ATMODE() { //set master hc05 (not the debug module) to at mode
    char tempCMD[25];
                strcpy(tempCMD,"ATMODE ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete command from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command

                //store the value
                char tempval;
                tempval = UARTrbuff[0]; //just holds the first char 
                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                if (tempval == '1'){ //then enable AT mode
                    T2CONbits.TON = 0;//stop the bluetooth  timer
                    BTKEY = 1;//set key high
                    BTPWR = 0;//turn off power to HC05
                    __delay_ms(150);//wait 150ms
                    BTPWR = 1;//turn on power to HC05
                    __delay_ms(777);//wait 777ms to boot up

                    UART_SEND("ATMODE enabled, BT reset\r\n");
                    return;
                }
                else if (tempval == '0') {
                    BTKEY = 0;//set key high
                    BTPWR = 0;//turn off power to HC05
                    __delay_ms(150);//wait 150ms
                    BTPWR = 1;//turn on power to HC05
                    __delay_ms(777);//wait 777ms to boot up
                    UART_SEND("ATMODE disabled, BT reset\r\n");
                    return;
                }
                else {
                    UART_SEND("Value for enable.ATMODE is invalid\r\n");
                    return;
                }
                __delay_ms(50);
                memset(UART2rbuff,0x00,sizeof(UART2rbuff));
                return;
} //enable/disable at command mode

int ATCMD() {

                char tempCMD[25];
                strcpy(tempCMD,"ATCMD ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete command from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command

                //grab the string that needs to be sent
                char atCMD[33];
                strcpy(atCMD,UARTrbuff);
                UART_SEND("AT command: '");
                UART_SEND(atCMD);
                UART_SEND("'\r\n");

                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
       //////////////////////////Command Execution/////////////////////////////////////
                //should be in AT mode.. if not set it to AT mode 
                
                strcat(atCMD,"\r\n"); //the last part of the command must contain \r\n
                UART2_SEND(atCMD); //dump the command to the slave
                //if it returns anything, it'll print back somewhere else
                
                


                return 0;
} //send at commands to the master bluetooth device

int REGCTRL() {
                char tempCMD[50];
                char detectedCMD[50];
                strcpy(detectedCMD,"REGCTRL ");
                UART_SEND("Command: '");
                UART_SEND(detectedCMD);
                UART_SEND("'\r\n");

               //////////////////////////Command parsing/////////////////////////////////////
                       //////////////////////////Command parsing/////////////////////////////////////
                //delete LATCTRL from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "LATCTRL" from command

                //grab the pin name from the string
                char regname[7] = "\0\0\0\0\0\0\0"; //6 bytes..+ 1 byte for null character.
                memcpy(regname,UARTrbuff,6);

                memset(tempCMD,0x00,sizeof(tempCMD));  //safety first!
                strcpy(tempCMD,(UARTrbuff+7)); //delete pin name from command...
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));
                strcpy(UARTrbuff,tempCMD);
                //+4 = 6 characters in reg name + followed by a space

                UART_SEND("Register: '");
                UART_SEND(regname);
                UART_SEND("'\r\n");


                //grab the number at the end and store it in tempval
                char tempval[10];
                memset(tempval,0x00,sizeof(tempval));
                strcpy(tempval,UARTrbuff);

                UART_SEND("Value: '");
                UART_SEND(tempval);
                UART_SEND("'\r\n");


                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                //UART_SEND("rbuff cleared\r\n");

                long val;
                char *ptr;
                val = strtol(tempval,&ptr,10);
        

//////////////////////////actions/////////////////////////////////

                if (strcmp(regname,"U1BAUD")) {//then change the baud rate of UART1
                    U1BRG = ((FCY/val)/16)-1;
                    _UARTEN = 0;
                    __delay_ms(150);
                    _UARTEN = 1;
                    _UTXEN = 1;
                    
                    UART_SEND("U1BAUD Changed\r\n");
                }

                else if (strcmp(regname,"U2BAUD")) {//change baud of U2
                    U2BRG = ((FCY/val)/16)-1;
                    U2MODEbits.UARTEN = 0;
                    __delay_ms(150);
                    U2MODEbits.UARTEN = 1; // Enable UART (clearing this resets all flags related to UART)
                    U2STAbits.UTXEN = 1; // Enable UART TX (only disable this if you want to prevent transmission)
                    
                    UART_SEND("UART2 baud was changed - U2 Channel reset!\r\n");
                }
                else {
                    UART_SEND("Unrecognized REG\r\n");
                    return 1;
                }
                return 0;
} //change variable values

int KEYSIM() {
                char tempCMD[25];
                strcpy(tempCMD,"KEYSIM ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete command from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command

                //store the keyname
                char keyname[10] = {0};
                strcpy(keyname,UARTrbuff);
                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                //UART_SEND("rbuff cleared\r\n");
                UART_SEND("Portal: '");
                UART_SEND(keyname);
                UART_SEND("'\r\n");

                if (strcmp(keyname,"SKY") == 0) {
                    portalSky();
                }
                else if (strcmp(keyname,"WATER") == 0) {
                    portalWater();
                }
                else if (strcmp(keyname,"ICE") == 0) {
                    portalIce();
                }
                else if (strcmp(keyname,"FIRE") == 0) {
                    portalFire();
                }
                else if (strcmp(keyname,"PLASMA") == 0) {
                    portalPlasma();
                }
                else if (strcmp(keyname,"LIGHTNING") == 0) {
                    portalLightning();
                }
                else if (strcmp(keyname,"EARTH") == 0) {
                    portalEarth();
                }
                else if (strcmp(keyname,"NONE") == 0) {
                    if (_TON == 1) {

                    UART_SEND("Kharon's keys removed from lock! The portal is collapsing!!\r\n");
                    portalCollapse();
                    }
                    else {
                        UART_SEND("You put the not-a-key in the lock. Nothing happens. What did you expect? Get Good Lucas.\r\n");
                    }


                }
                else {
                    UART_SEND("Portal name not recognized, simulation aborted\r\n");
                }
                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                //UART_SEND("rbuff cleared\r\n");
                return 0;
} //simulate an inserted key

int pushBTPacket() { //shove some hex, yo.
    

    UART2_SEND0x(BTconnectpacket,6);
    memset(UARTrbuff,0x00,sizeof(UARTrbuff));
    return 0;
}

void U1CHAN(char *channel) { //switch between somo/uart debugger 

    if (strcmp(channel,"SOMO") == 0) {
         _LATA0 = 1;
        __delay_ms(50); //wait for U1 channel to clear out
        _UARTEN = 0; //diable uart channel
        #undef U1BAUD
        #define U1BAUD 9600 //recalculate the baud rate
        #undef U1BRGVAL
        #define U1BRGVAL ((FCY/U1BAUD)/16)-1
        U1BRG = U1BRGVAL;

        SOMOPIN = U1TEN;
        DEBUGPIN = UVOID;
        __delay_ms(1);
        _UARTEN = 1; //enable uart
        _UTXEN = 1;
        __delay_ms(52); //wait for uart to set itself up
        _LATA0 = 0;
    }
    else if (strcmp(channel,"DEBUG") == 0) {
//        _LATA0 = 1;//flash a light
        __delay_ms(50); //wait for U1 channel to clear out
        _UARTEN = 0; //diable uart channel
        #undef U1BAUD
        #define U1BAUD 38400//recalculate the baud rate
        #undef U1BRGVAL
        #define U1BRGVAL ((FCY/U1BAUD)/16)-1
        U1BRG = U1BRGVAL;
        SOMOPIN = UVOID;
        DEBUGPIN = U1TEN;
        __delay_ms(1);
        _UARTEN = 1; //enable uart
        _UTXEN = 1;
        __delay_ms(52); //wait for uart to set itself up
//        _LATA0 = 0;//flash a light
        
    }
    else {
        DEBUGPIN = U1TEN;
        __delay_us(104);
        UART_SEND("Switching UART channels failed\r\n");
    }

}

int RGBCTRL() {//syntax: RBGCTRL <LED #> <R> <G> <B> kD$
    char tempCMD[50];
    strcpy(tempCMD,"RGBCTRL ");
    UART_SEND("Command: '");
    UART_SEND(tempCMD);
    UART_SEND("'\r\n");

//////////////////////////Command parsing/////////////////////////////////////
    //delete command from the string
    strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command
    int Rval;
    int Gval;
    int Bval;
    int LEDnum;
    char *temp; //temp is just a fuckin' pointer. idk.
    
    LEDnum = strtol(UARTrbuff,&temp,10);
    strcpy(UARTrbuff,temp);
    
    Rval = strtol(UARTrbuff,&temp,10); //read in Rval
    strcpy(UARTrbuff,temp); //shift rbuff forward
    
    Gval = strtol(UARTrbuff,&temp,10);
    strcpy(UARTrbuff,temp);
   
    Bval = strtol(UARTrbuff,&temp,10);
    strcpy(UARTrbuff,temp);
    

    sprintf(tempCMD,"LED#: '%i'\r\n",LEDnum);
    UART_SEND(tempCMD);
    sprintf(tempCMD,"Rval: '%i'\r\n",Rval);
    UART_SEND(tempCMD);
    sprintf(tempCMD,"Gval: '%i'\r\n",Gval);
    UART_SEND(tempCMD);
    sprintf(tempCMD,"Bval: '%i'\r\n",Bval);
    UART_SEND(tempCMD);

    memset(UARTrbuff,0x00,sizeof(UARTrbuff));//clear rbuff now that we're done with it.
//actions

    //set R value of LED
    pwmbuffer[(3 * (LEDnum - 1)) + 0] = Rval;
    //set G value of LED
    pwmbuffer[(3 * (LEDnum - 1)) + 1] = Gval;
    //set B value of LED
    pwmbuffer[(3 * (LEDnum - 1)) + 2] = Bval;
    //write
    write_RGB();
    return 1;
}//

int LCDCTRL() {
                char tempCMD[50];
                strcpy(tempCMD,"LCDCTRL ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete command from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command
                char sendString[50];
                int rowint;
                int colint;
                memset(sendString,0x00,sizeof(sendString)); //paranoid android

                  //parse the string to send
                int i;
                //loop through until we find a quotation mark, note the index
                //then loop through until we find another quotation mark, and note the index.
                int strstart = -1;
                int strend = -1;

                for (i = 0; i < strlen(UARTrbuff); ++i) { //find indicies of end and begin of input string
                    

                    
                    if (UARTrbuff[i] == '"') {//found a "
                        if (strstart == -1) {//FOUND BEGINNING
                          strstart = i;
                        }
                        else if (strstart != -1) {//FOUND END
                          strend = i;
                            break;
                        }
                    }//end found "
                }//end for loop to find whats inside "s
                if ((strstart == -1)||(strend == -1)) {//then there werent 2 "s found
                    UART_SEND("There was an error parsing the input string\r\n");
                    return 0;
                }


                //copy the contents from indicies strstart to strend into stringSend
                strstart++; //ignore the first quotation mark by using +1
                for (i = 0; i < (strend - strstart); i++) { //ignore the second quotation mark by using < instead of <=
                    sendString[i] = UARTrbuff[i+strstart]; //copy the ith letter into sendString
                }
                
                UART_SEND("Input string: '");
                UART_SEND(sendString);
                UART_SEND("'\r\n");

                //delete the string from the receive buffer 
                strcpy(UARTrbuff,UARTrbuff+strlen(sendString)+3); //+3 characters for quotation marks and space
                //get the row from the receive buffer
                rowint = strtol(UARTrbuff,NULL,10);
                //delete the row from the receive buffer
                strcpy(UARTrbuff,UARTrbuff+2); //one int char and one space
                //get the col from the receive buffer
                colint = strtol(UARTrbuff,NULL,10);
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));
                

                memset(tempCMD,0x00,sizeof(tempCMD));//because i'm paranoid.
                
                UART_SEND("Row, col: '");
                sprintf(tempCMD,"%i,%i",rowint,colint);
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");


                //clear uartrbuff
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));

                if (colint > 16) {//then we cant write over there.
                    UART_SEND("invalid column number\r\n");
                    return 0;
                }
                if (rowint>2) {//we cant write over there.
                    UART_SEND("invalid row number\r\n");
                    return 0;
                }
                if((strlen(sendString) + rowint) > 16) {
                    UART_SEND("String was too long, and was truncated to: '");
                    sendString[17-rowint] = 0x00;
                    UART_SEND(sendString);
                    UART_SEND("'\r\n");
                }
                //do stuff
                
                position_LCD(rowint,colint);
                write_LCD_Text(sendString);
                UART_SEND("DONE WRITING TO LCD\r\n");

                return 1;
}//control LCD stuff

//syntax: SOMOPLAY <Song#(int)> <repeat?(0\1)>
int SOMOPLAY() {
    char tempCMD[50];
    strcpy(tempCMD,"SOMOPLAY ");
    UART_SEND("Command: '");
    UART_SEND(tempCMD);
    UART_SEND("'\r\n");

//////////////////////////Command parsing/////////////////////////////////////
    //delete command from the string
    strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command
    memset(tempCMD,0x00,sizeof(tempCMD));//paranoia
char tempstr[50];
    //get song # from cmd
    int songNum;
         
    songNum = atoi(UARTrbuff);
//    sprintf(tempstr,"the string is: '%s'\r\nstrtol: '%i'\r\natoi: '%i'",UARTrbuff,strtol(UARTrbuff,NULL,10),atoi(UARTrbuff));
//    UART_SEND(tempstr);

   
    
    sprintf(tempstr,"Song number: '%i'\r\n",songNum);
    UART_SEND(tempstr);

    U1CHAN("SOMO");
    play((char)songNum,&c);
    U1CHAN("DEBUG");
    
    memset(UARTrbuff,0x00,sizeof(UARTrbuff));
    return 1;
}//play a song

void ASDF() {
    
    UART_SEND("Nothing in temp fcn\r\n");
    return;
}

int SOMOVOL() {
char tempCMD[50];
    strcpy(tempCMD,"SOMOVOL ");
    UART_SEND("Command: '");
    UART_SEND(tempCMD);
    UART_SEND("'\r\n");

//////////////////////////Command parsing/////////////////////////////////////
    //delete command from the string
    strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command
    memset(tempCMD,0x00,sizeof(tempCMD));//paranoia
    //sprintf(tempCMD,"Command after delete: %s\r\n",UARTrbuff);
   // UART_SEND(tempCMD);
    //get volume from cmd
    int volume;
    volume = strtol(UARTrbuff,NULL,10);
    
    sprintf(tempCMD,"Volume: '%i'\r\n",volume);
    UART_SEND(tempCMD);

    U1CHAN("SOMO");
    set_volume(volume,&c);
    U1CHAN("DEBUG");

    UART_SEND("Volume changed\r\n");
    memset(UARTrbuff, 0x00, sizeof(UARTrbuff));
    return 1;
}//change the volume

void PORTIME() {
    int porlen = 60;
    char tempstr[50];
    
    sprintf(tempstr,"Command received: '%s'\r\n",UARTrbuff);
    UART_SEND(tempstr);
    strcpy(UARTrbuff,UARTrbuff+strlen("PORTIME ")); //delete "tempCMD" from command
    memset(tempstr,0x00,sizeof(tempstr));//paranoia

    porlen = strtol(UARTrbuff,NULL,10);
    TMR1MAX = porlen;//max portal open time
    TMR1CNTVAL = porlen; //initialize Timer 1 counter value. - # seconds to keep the portal open

    sprintf(tempstr,"Portal time change to: '%i",porlen);
    UART_SEND(tempstr);

    memset(UARTrbuff,0x00,sizeof(UARTrbuff));

}

void TESTSOMO() {

 //BRANDON INTEGRATION SHOWCASE
    memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuffer
    //sm contains the song names
    //c is the struc containing the command structure
    UART_SEND("PLAYING MOMS SPAGHETTI SONG @ 20 volume \r\n");
    U1CHAN("SOMO");//set uart1 channel to somo
    set_volume(20,&c);
    __delay_ms(40);
    play((char)0x07,&c);
    UART_SEND0x(c.REPEAT_MODE,8);
    U1CHAN("DEBUG");//set uart1 channel to debugger
    
    __delay_ms(5000);


    UART_SEND("TURNING THE VOLUME UP!!!!\r\n");
    int myvol = 30;
    U1CHAN("SOMO");//set uart1 channel to somo
    set_volume(myvol,&c);
    U1CHAN("DEBUG");//set uart1 channel to somo

    __delay_ms(5000);

    UART_SEND("TURNING THE VOLUME DOWN!!!!\r\n");
    myvol = 10;
    U1CHAN("SOMO");//set uart1 channel to somo
    set_volume(myvol,&c);
    U1CHAN("DEBUG");//set uart1 channel to somo
    return;

    __delay_ms(5000);

    //play a different song
    UART_SEND("PLAYING A DIFFERENT SONG!!\r\n");
    U1CHAN("SOMO");//set uart1 channel to somo
    play(sm.plasma,&c);
    UART_SEND0x(c.REPEAT_MODE,8);
    U1CHAN("DEBUG");//set uart1 channel to debugger
    __delay_ms(1000);
    UART_SEND("ALL DONE\r\n");
    return;
}

void TESTLCD() {
   //RYAN INTEGRATION SHOWCASE
    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("Ryan is cool.. kinda");

    plasmaWorldVisuals();



memset(UARTrbuff,0x00,sizeof(UARTrbuff));
    return;
}

void PORRST() {
    //disable the timer1 interrupt
    T1CONbits.TON = 0;
    //set timer 1 back to defaults
    TMR1CNT = 0;

    //disable timer3 IR packet interrupt
    T3CONbits.TON = 0;
    
    //set currentportal to 0
    currentPortal = 0;

    portalIdle();
    //

}//force the portal to idling mode

void TESTIR() {
    merge(0x0d);
    
   memset(UARTrbuff,0x00,sizeof(UARTrbuff));
    return;
}

void testAnim() {
    //set animation to the right one
    animArray = animFire;
    //turn lights off
    memset(pwmbuffer,0,sizeof(pwmbuffer));
    write_RGB();

}

void BTINIT() {
   //send connect packet
    UART2_SEND0x(BTconnectpacket,6);
    //start heartbeat timer.. bluetooth interrupt will handle whether or not it stays on.
    TMR2CNTVAL = 6; //send every 3 seconds
    TMR2CNT = 0; //send first heartbeat after n iterations.
    T2CONbits.TON = 1;

    
}//initialize bluetooth

void updateBT() {

    //if attempting to connect

    //if not connecting

    //

}

void updateLCD() {
    //update the portal name 
    
    
////////////////////////////////////////////////////////////////////////////////    
    char portalString[10];//8 characters plus null character

    if (currentPortal == 0) {
        strcpy(portalString,"P:IDLE  |");
    }
    else if (currentPortal == -1) {
        strcpy(portalString,"P:COLDWN|");
    }
    else if (currentPortal == 1) {
        strcpy(portalString,"P:SKY   |");
    }
    else if (currentPortal == 2) {
        strcpy(portalString,"P:WATER |");
    }
    else if (currentPortal == 3) {
        strcpy(portalString,"P:FLAME |");
    }
    else if (currentPortal == 4) {
        strcpy(portalString,"P:ARC   |");
    }
    else if (currentPortal == 5) {
        strcpy(portalString,"P:ICE   |");
    }
    else if (currentPortal == 6) {
        strcpy(portalString,"P:PLASMA|");
    }
    else if (currentPortal == 7) {
        strcpy(portalString,"P:EARTH |");
    }
    else if (currentPortal == 8) {
        strcpy(portalString,"P:EARTH |");
    }
    else if (currentPortal == 9) {
        strcpy(portalString,"P:EARTH |");
    }
    else if (currentPortal == 10) {
        strcpy(portalString,"P:EARTH |");
    }
    
    
    position_LCD(1,1);//put the cursor on the topleft
    write_LCD_Text(portalString);//write the thing
////////////////////////////////////////////////////////////////////////////////
    //update the time value 
    char timestring[8]; //7 characters plus null
    char blankstring[8];
    memset(blankstring,0x00,sizeof(blankstring));


        //if portal open
    if (currentPortal > 0) {
        sprintf(timestring,"TOPN:%2i",TMR1CNT);
    }
        //if cooldown
    else if (currentPortal == -1) {
        sprintf(timestring,"TCDN:%2i",TMR1CNT);
    }
        //if idle
    else if (currentPortal == 0) {
        sprintf(timestring,"[READY]");
    }

    position_LCD(1,10);
    write_LCD_Text(blankstring);
    position_LCD(1,10);
    write_LCD_Text(timestring);
////////////////////////////////////////////////////////////////////////////////
    //update the player health
    char hpstring[10];
    sprintf(hpstring,"HP:[%3u]|",PL_HEALTH);

   
    position_LCD(2,1);
    write_LCD_Text(hpstring);
////////////////////////////////////////////////////////////////////////////////
    //update player status
    char statstring[8]; //7 characters + null string
    sprintf(statstring,"STAT: %i",PL_STATUS);
    position_LCD(2,10);
    write_LCD_Text(blankstring);
    position_LCD(2,10);
    write_LCD_Text(statstring);
    return;
}//update the LCD with game values

void updateRGB() {
    if ((fancyGFX != 1)||(animArray[0] == -6)) {
        return; //dont do anything if it's the none animation
       //or fancy gfx are off
    }
    int i;
    int j;
    int dR,dG,dB,oR,oG,oB;
    int t;
    //begin changing LEDs
    for (i = 0; i <= 7; i++, j++) {//led#1 is 0th index, led#8 is 7th index

        //the animation array is stored in animArray pointer.

        //get the t from animArray
        t = (animArray[animRGBprog[i]+0]);
            //if it's -1, then change the pointer and start this iteration over
        if (t <= -1) {//then it's the end of the animation, so start it over.
            animRGBprog[i] = 0;
            i--;
            continue;
//            UART_SEND("PARTY\r\n");
        }
        else {
        //get the new RGB values
        dR = animArray[animRGBprog[i]+1]; //the new value of the R
        dG = animArray[animRGBprog[i]+2]; //g
        dB = animArray[animRGBprog[i]+3]; //b
        oR = pwmbuffer[(i*3)+0]; //initial value of R
        oG = pwmbuffer[(i*3)+1];
        oB = pwmbuffer[(i*3)+2];
//        __delay_ms(250);

//        UART_SEND(tempstr);
//        __delay_ms(250);
        if ((dR == oR)&&(dG == oG)&&(dB == oB)) {//then there's no change to be had. increment the pointer and start iteration over
            animRGBprog[i] = animRGBprog[i] + 4;//move this LED to next color to be changed to
            --i;
//            UART_SEND("RGB value reached\r\n");
            if ((t != -6)||j < 30) {
                continue;
            }
        }//otherwise keep going
       // sprintf(tempstr,"old # R,g,b: '#%i - %u,%u,%u'\r\nTarget: '#%i - %u,%u,%u'\r\nIncrement: '%u'\r\n",i,oR,oG,oB,i,dR,dG,dB,t);
     //   UART_SEND(tempstr);


           //change values
           ///////////////////////////////R//////////////////////////////
            if (dR < oR) {//then we need to subtract from original
                oR = oR - t;

                if(oR < dR) {//then we went under target
                    oR = dR;
                }

            }
            else if (dR > oR) {//then we need to add
                oR = oR + t;

                if (oR > dR) {//then we went over target
                    oR = dR;
                }
            }
            else if (dR == oR) {//then we are at the target
                //do nothing
            }

           ///////////////////////////////B//////////////////////////////
            if (dB < oB) {//then we need to subtract from original
                oB = oB - t;

                if(oB < dB) {//then we went under target
                    oB = dB;
                }

            }
            else if (dB > oB) {//then we need to add
                oB = oB + t;

                if (oB > dB) {//then we went over target
                    oB = dB;
                }
            }
            else if (dB == oB) {
                //do nothing
            }
                   ///////////////////////////////G//////////////////////////////
            if (dG < oG) {//then we need to subtract from original
                oG = oG - t;

                if(oG < dG) {//then we went under target
                    oG = dG;
                }

            }
            else if (dG > oG) {//then we need to add
                oG = oG + t;

                if (oG > dG) {//then we went over target
                    oG = dG;
                }
            }
            else if (dG == oG) {
                //do nothing
            }


        }


        //change PWM buffer  (|NEW VALUE| < |OLD VALUE|)*-1+(|NEW VALUE| == |OLD VALUE|)*0+(|NEW VALUE| > |OLD VALUE|)*1
//        __delay_ms(250);
//        sprintf(tempstr,"New values of RGBs: '#%i - %u,%u,%u'\r\n",i,oR,oG,oB);
//        UART_SEND(tempstr);
//        __delay_ms(250);
        if (oR > 4050) {
            oR = 4000;
        }
        if (oG > 4050) {
            oG = 4000;
        }
        if (oB > 4050) {
            oB = 4000;
        }
        pwmbuffer[(i*3)+0] = oR; //R component of LED #i+1
        pwmbuffer[(i*3)+1] = oG; //G component of LED #i+1
        pwmbuffer[(i*3)+2] = oB; //B component of LED #i+1
        //make it so!

//        __delay_ms(1);
    }
    write_RGB();
    return;
}//update the RGBs


void BTHEART() {
        char tempCMD[25];
                strcpy(tempCMD,"BTHEART ");
                UART_SEND("Command: '");
                UART_SEND(tempCMD);
                UART_SEND("'\r\n");

       //////////////////////////Command parsing/////////////////////////////////////
                //delete command from the string
                strcpy(UARTrbuff,UARTrbuff+strlen(tempCMD)); //delete "tempCMD" from command

                //store the value
                char tempval;
                tempval = UARTrbuff[0]; //just holds the first char
                memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
                if (tempval == '1'){ //then enable AT mode
                    T2CONbits.TON = 1;
                    UART_SEND("turned on bluetooth heartbeats\r\n");
                }
                else if (tempval == '0') {
                    T2CONbits.TON = 0;
                    UART_SEND("turned off bluetooth heartbeats\r\n");
                }
                else {
                    UART_SEND("wrong value\r\n");
                }
                __delay_ms(50);
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));
                return;
}

void BTreceive() {
//           char tempstr[50] = "asdf";
        //check to see if the checksum matches
//        UART_SEND("now checking sum\r\n");
//        char tempstr[50];
//        sprintf(tempstr,"packet sum: '%X'\r\ncrc is: '%X'\r\n",(0xff - U2RX[1] - U2RX[2] - U2RX[3] - U2RX[4]),U2RX[5]);
//        UART_SEND(tempstr);
           char bitsum; 
           char bitcrcmp;
           char crc;
           bitsum = ((char)('\xff' - U2RX[1] - U2RX[2] - U2RX[3] - U2RX[4]));
           crc = (U2RX[5]);
           bitcrcmp = (bitsum == crc);//will be 1 if bitsum and crc are equal
          // sprintf(UARTsbuff,"[PIU]--> '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\nBITSUM: '%.2x'\r\nCRC: '%.2x'\r\nCompare Bitsum to CRC: %i\r\n",U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5],bitsum,crc,bitcrcmp);
           

        
        if ((bitcrcmp) && (U2RX[0] == '\x3c')) {//checksum passes and first byte is our start byte
            //sprintf(UARTsbuff,"Packet CRC success: '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\n",U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5]);
            //determine the nature of the packet 
            BTlink = 1;//we got something, right?
            if (U2RX[1] == 0x00) {//0x00 is heartbeat function. should not be received from piu. throw an error.
                UART_SEND("[ERR>U2RXIE]Erroneous heartbeat received from PIU. \r\n");
//                UART2_SEND(BTackpacket);
            }
            else if (U2RX[1] == 0x01) {//connect packet received. 
                memcpy(PID,U2RX+2,2);//store the 3rd byte into our player ID global
                PID[2] = 0x00;//make sure the end is anull string
                
                sprintf(UARTsbuff,"[PIU]-->CONNECT\r\n Player id is: '%.2x %.2x'\r\nPacket: '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\n",PID[0],PID[1],U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5]);
            //    UART2_SEND(BTackpacket);
                BTlinkCNT = 3;//refresh the bt link timer
                
            }
            else if (U2RX[1] == 0x02) {//IR tx. should not come from PIU. throw error
                UART_SEND("[ERR>U2RXIE]Erroneous IRTX packet received from PIU.\r\n");
//                UART2_SEND(BTackpacket);
            }
            else if (U2RX[1] == 0x03) {//update packet received from piu. update game variables 
                               
                PL_HEALTH = U2RX[2];
                PL_STATUS = U2RX[3];
                PL_EFFECT = U2RX[4];
                sprintf(UARTsbuff,"[PIU]-->UPDATE\r\nHP: '%i'|EFFECT: '%i'|STATUS: '%i'\r\nPacket: '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\n",PL_HEALTH,PL_EFFECT,PL_STATUS,U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5]);
                BTlinkCNT = 3;//refresh the bt link timer
              //  UART2_SEND(BTackpacket);
                
            }
            else if (U2RX[1] == 0xff) {//ack packet. do nothing. 
                UART_SEND("[PIU]-->ACK\r\n");
                BTlinkCNT = 3;//refresh the bt link timer
            }
            else {
                UART_SEND("[ERR>U2RXIE]A packet with an unrecognized function byte was received from the PIU.\r\n");
            }//end packet cases

            memset(U2RX,0x00,sizeof(U2RX));//packet processed. discard it.
            memset(UART2rbuff,0x00,sizeof(UART2rbuff));//packet processed. discard it.
        }//end checksum if
        else if (BTreceivecounter < 30) {
            BTreceivecounter++;
        }
        else if (BTreceivecounter >= 30) {
            sprintf(UARTsbuff,"Erroneous packet timed out & discarded: '%.2x,%.2x,%.2x,%.2x,%.2x,%.2x'\r\n",U2RX[0],U2RX[1],U2RX[2],U2RX[3],U2RX[4],U2RX[5]);
            memset(U2RX,0x00,sizeof(U2RX));//packet processed. discard it.
            memset(UART2rbuff,0x00,sizeof(UART2rbuff));//packet processed. discard it.
            BTreceivecounter = 0;
        }
        else {
            UART_SEND("Something weird happened with bt receive\r\n");
        }


    
        
    return;
}



void telltime() {
//    char tempstr[50];


    int time1 = TMR5;
    __delay_ms(1000);
    int time2 = TMR5;
    sprintf(UARTsbuff,"delay 1 sec took '%i' units\r\n t1 = '%i', t2 = '%i'\r\n",time2-time1,time1,time2);
   // UART_SEND(tempstr);
    memset(UARTrbuff,0x00,sizeof(UARTrbuff));
}

//Poll this to enable debugging
int UART_kDebug() { //all debug inputs to the UART

//////////////////////Buffer length request////////////////////////
    char tempstr[50];

        //handle PIU comms first
        if (strlen(UART2rbuff) != 0) { //if UART2 has anything in it
                        
            if (((strstr(UART2rbuff,"OK")!=NULL))||((strstr(UART2rbuff,"FAIL")!=NULL))||((strstr(UART2rbuff,"ERROR")!=NULL))) {//if OK is received back from slave, it'll get printed here.

                
                sprintf(tempstr,"AT reply: '%s'\r\n",UART2rbuff);
                UART_SEND(tempstr);
                memset(UART2rbuff,0x00,sizeof(UART2rbuff));
                return 4; //return 4 means a good response was received from AT.
            }
            else if (strchr(UART2rbuff,'\x3c')!=NULL) {//check to see if we got a reply from piu
                BTreceive();
            }
        }
        else if (strlen(UARTrbuff) != 0) {//check Debug comms next
           //do nothing.. which skips to next if/else if statement containing kD# comparisons and such.
        }
        else {

            return 0; //Nothing in UART2rbuff
        }



        //handle debug comms next
        if(strstr(UARTrbuff,"kD$") != NULL) {

            
            memset(strstr(UARTrbuff,"kD$")-1,0x00,4);//remove the kD$ as well as the space that's supposed to be before it.
            //which makes most string functions treat it as truncated past that point.

            sprintf(tempstr,"Command received: '%s'\r\n",UARTrbuff);
            UART_SEND(tempstr);
            



//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/*******************************DEBUG COMMANDS***********************************************/
            if (strstr(UARTrbuff,"LATCTRL ")!=NULL) { //if the string has "LATCTRL" in it
                LATCTRL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"TRISCTRL ")!=NULL) {
                TRISCTRL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"ATCMD")!=NULL) {
                ATCMD();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"REGCTRL")!=NULL) {
                REGCTRL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"KEYSIM")!=NULL) {
                KEYSIM();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"LCDCTRL")!=NULL) {
                LCDCTRL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"PORRST")!=NULL) {
                PORRST();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"TEST")!=NULL) {
                ASDF();
                return 1; //return 1 means that code was mailed successfully
            }
           
            else if (strstr(UARTrbuff,"RGBCTRL")!=NULL) {
                RGBCTRL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"ATMODE")!=NULL) {
                ATMODE();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"LCDCLR")!=NULL) {
                clear_LCD();
                UART_SEND("LCD CLEARED\r\n");
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"RGBCLR")!=NULL) {
                clear_RGB();
                UART_SEND("RGB CLEARED\r\n");
                memset(UARTrbuff,0x00,sizeof(UARTrbuff));
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"SOMOPLAY")!=NULL) {
                SOMOPLAY();
                return 1; //return 1 means that code was mailed successfully
            } 
            else if (strstr(UARTrbuff,"SOMOVOL")!=NULL) {
                SOMOVOL();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"PORTIME")!=NULL) {
                PORTIME();
                return 1; //return 1 means that code was mailed successfully
            }
             else if (strstr(UARTrbuff,"TIME")!=NULL) {
                telltime();
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"BTPUSH")!=NULL) {
                pushBTPacket();  //shove some hex, yo.
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"BTHEART")!=NULL) {
                BTHEART();  //shove some hex, yo.
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"TESTIR")!=NULL) {
                TESTIR();  //shove some hex, yo.
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"TESTSOMO")!=NULL) {
                TESTSOMO();  //shove some hex, yo.
                return 1; //return 1 means that code was mailed successfully
            }
            else if (strstr(UARTrbuff,"TESTLCD")!=NULL) {
                TESTLCD();  //shove some hex, yo.
                return 1; //return 1 means that code was mailed successfully
            }
            else {

             UART_SEND("Command not recognized\r\n");
             memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
             //UART_SEND("rbuff cleared\r\n");
             return 0; //return 0 indicates that something was recieved, but discarded
            }
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
/***************************END DEBUG COMMANDS***********************************************/
            
        }//end the if to check to see if kD$ was found
        else if ((rbuffCounter == 20)&&(strlen(UARTrbuff) != 0)) {//if it takes too long ()to send a command, clear the receive buffer.
            sprintf(tempstr,"Input on U1 was: '%s'\r\n",UARTrbuff);
            UART_SEND(tempstr);
            
            memset(UARTrbuff,0x00,sizeof(UARTrbuff)); //clear the rbuff string
            //UART_SEND("rbuff cleared\r\n");
            UART_SEND("Input window time exhausted\r\n");
            rbuffCounter = 0;
            return 0; //return 0 indicates that something was recieved, but discarded
            }//end checking UART1 receive buffer
         else if ((rbuffCounter == 20)&&(strlen(UART2rbuff) != 0)) {//if it takes too long ()to send a command, clear the receive buffer.

             sprintf(tempstr,"Input on U2 was: '%s'\r\n",UART2rbuff);
            UART_SEND(tempstr);

            memset(UART2rbuff,0x00,sizeof(UART2rbuff));
            //UART_SEND("rbuff cleared\r\n");
            UART_SEND("Input window time exhausted\r\n");
            rbuffCounter = 0;
            return 0; //return 0 indicates that something was recieved, but discarded
            }//end checking UART2 receive buffer
        else {
            rbuffCounter++;
            return 2;//return 2 means that rbuff counter incremented 
        }

        return 3; //return 3 means that it got to the bottom or that nothing was received
}//end UART_kDebug

void checkKey() {

        //POLL adc sets the world value to one of the following:
        // ice = 5
        //plas = 6
        //wind/sky is 1
        //water is 2
        //fire is 3
        //light is 4
        //no portal = 0
        //if its 7-10, earth portal (i.e. incorrect combination.)

        poll_ADC();//sets current world variable after polling analog key value.

//        if (portal2 == currentWorld) {//check the previous value of portal 1. If this is true, then the key is the same as it was before.
//            return;//do nothing unless the key changed.
//        }

        portal1 = currentWorld;//store for comparison later

        //we're repeating the poll to prevent something weird happening between key insertions.
        __delay_ms(50); //lazy debouncing
        poll_ADC();
        if (portal2 == currentWorld) {//check the previous value of portal 2. If this is true, then the key is the same as it was before.
            return;//do nothing unless the key changed.
        }
        int portal2 = currentWorld; //store for comparison later
        
        //only do something if we have a solid read on the key.
        if (portal1 == portal2) //then it's safe to say the key is safely in the lock (else do nothing and return)
        {
            
            //open a portal depending on which key is in
            //if currentPortal == -1, then portals are on cooldown.
            //if currentPortal == 0, then there's an idle portal.
            //if currentPortal > 0, then a portal is currently open
            //we want to always be able to change the portal unless there's no key in the lock.
            //if there's no key in the lock, we want the portal to collapse, and the cooldown timer to start.
            //if a key is inserted while a portal is already going, the portal should change, but not collapse.
            //if the timer reaches maximum, we want the portal to collapse. //HANDLED IN TIMER 1 INTERRUPT
            //we want a portal to open if the portal timer is not at maximum, and it's not the same portal that's open.
//            char tempstr[50];
//            sprintf(tempstr,"key detected is: '%i'\r\nPortal is: '%i'\r\n",currentWorld,currentPortal);
//            UART_SEND(tempstr);
           
            if ((currentWorld == 0) && (currentPortal != 0) && (currentPortal != -1)) {
               
//                UART_SEND("THE NO KEY\r\n");
                portalCollapse();
                
            }
            else if (currentPortal == -1) {//then we should be on cooldown
//                UART_SEND("cdkey\r\n");
                return;//do nothing
            }
            else if ((currentWorld == 1) && (currentPortal != 1) && (currentPortal != -1)) {//sky key was read, and the sky portal is not open.
                portalSky();//open portal
            }
            else if ((currentWorld == 2) && (currentPortal != 2) && (currentPortal != -1)) {//water key was read, and the water portal is not open.
                portalWater();//open portal
            }
            else if ((currentWorld == 3) && (currentPortal != 3) && (currentPortal != -1)) {//fire key was read, and the fire portal is not open.
                portalFire();//open portal
            }
            else if ((currentWorld == 4) && (currentPortal != 4) && (currentPortal != -1)) {//arc key was read, and the arc portal is not open.
                portalLightning();//open portal
            }
            else if ((currentWorld == 5) && (currentPortal != 5) && (currentPortal != -1)) {//ice key combo was read, and the ice portal is not open.
                portalIce();//open portal
            }
            else if ((currentWorld == 6) && (currentPortal != 6) && (currentPortal != -1)) {//plasma key combo was read, and the plasma portal is not open.
                portalPlasma();//open portal
            }
            else if ((currentWorld >= 7) && !(currentPortal >= 7) && (currentPortal != -1)) {//plasma key combo was read, and the plasma portal is not open.
                portalEarth();//open portal
            }



        }
        else {
            return;//if portal1 and portal2 arent the same then somehitng is wrong and we need to not be opening portals.
        }
}

void idleProcess() {
    //IF THERE's something in sendbuffer, send it
        if(strcmp(UARTsbuff,"")!=0) {
            UART_SEND(UARTsbuff);
            memset(UARTsbuff,0x00,sizeof(UARTsbuff)); //clear the sbuff string
        }
//        UART_SEND("UPDATE LCD\r\n");
        updateLCD();//update the lcd.. duh
//        UART_SEND("UPDATE RGB\r\n");
        updateRGB();
//        UART_SEND("UPDATE debug\r\n");
        UART_kDebug();//do debug stuff

///////////////////////////////////////////////////////////////////////////////
//        UART_SEND("checkkey\r\n");
        checkKey();//check the key reader
        return;
}

void bootMSG() {
    //check for segmentation fault (if the leds all blink continuously, we know
    //that the pic is rebooting a lot.
   
   
    UART_SEND(" \r\nBOOTING LANTERN...\r\n");
    BTKEY = 0;//boot up bluetooth module 
    BTPWR = 1;
    __delay_ms(150);
    

    clear_LCD();
    position_LCD(1,1);
    write_LCD_Text("LANTERN os v6.1");
    position_LCD(2,1);
    write_LCD_Text("Charlie is DOPE!");
    


    U1CHAN("SOMO");//set uart1 channel to somo
    play(sm.start_up,&c);//play bootup song
    U1CHAN("DEBUG");//set uart1 channel to somo

     U1CHAN("SOMO");
    set_volume(24,&c);//loud af
    U1CHAN("DEBUG");
    //end check for segfaul
    
    BTINIT(); //initialize bluetooth connection and heartbeats.
    __delay_ms(1100);
    if (BTlink == 0) {
        UART_SEND("[DBG]--> Did not connect BT with PIU\r\n");
    }
    else if(BTlink == 1) {
        UART_SEND("[DBG]--> BT Connected\r\n");
    }
    else {
        UART_SEND("WTF\r\n");
    }
    
    UART_SEND("\r\nBOOT SUCCESS\r\n\r\n\r\n");
    fireWorldVisuals();
    windWorldVisuals();
    iceWorldVisuals();
    plasmaWorldVisuals();
    waterWorldVisuals();
    lightningWorldVisuals();
    noWorldVisuals();
    blinkAnim(5); //blink 5 times
    clear_RGB();
    setRand();
    
     
}


#endif	/* KHARON_FCN_H */

