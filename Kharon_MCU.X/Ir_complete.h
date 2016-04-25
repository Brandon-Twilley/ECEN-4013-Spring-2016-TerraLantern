#ifndef XC_HEADER_TEMPLATE_H
#define XC_HEADER_TEMPLATE_H

#define baudDelay 2

#define _ir1 _RB6 //pin definitions // read from ports 
#define _ir2 _RB7
#define _ir3 _RB8
#define _ir4 _RB9

#define _irOut1 _LATB6 //writing to these
#define _irOut2 _LATB7
#define _irOut3 _LATB8
#define _irOut4 _LATB9

#define _irOn1 _TRISB6 // 0 is output 1 is input / sets direction 
#define _irOn2 _TRISB7
#define _irOn3 _TRISB8
#define _irOn4 _TRISB9

#define damage 0x00
#define damageteam 0x01
#define fire 0x07
#define fireteam 0x08
#define water 0x09
#define waterteam 0x0A
#define wind 0x0D
#define windteam 0x0E
#define ice 0x0F
#define iceteam 0x10
#define lightning 0x13
#define lightningteam 0x14



/**********************END DECLARE VARIABLES*******************************/
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////


void lowSide(){
    int i; 
    int j; 
    int k; 
    
    for(i = 0; i < 15; ++i){
        _irOut1 = 0;
        _irOut2 = 0;
        _irOut3 = 0;
        _irOut4 = 0;
        
       for(j = 2; j != 0; --j){
            Nop(); 
        }
        _irOut1 = 0;
        _irOut2 = 0;
        _irOut3 = 0;
        _irOut4 = 0;
                
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
        
          for(k = 6; k != 0 ; --k){  
            Nop(); 
        }
              
    }
}

void highSide(){
      int i; 
    int j; 
    int k; 
    
    for(i = 0; i < 15; ++i){
        _irOut1 = 1;
        _irOut2 = 1;
        _irOut3 = 1;
        _irOut4 = 1;
        
        for(j = 2; j != 0; --j){
            Nop(); 
        }
        _irOut1 = 0;
        _irOut2 = 0;
        _irOut3 = 0;
        _irOut4 = 0;
        
        Nop();
        Nop();
        Nop();
        Nop();
        Nop();
       
        for(k = 6; k != 0 ; --k){  
            Nop(); 
        }
              
    }
}

int RNG(int in) {

    unsigned int return_num = TMR4;

    return_num = (return_num*7) << 2;
    return_num = return_num%in;
    return return_num;

}

void sendOne(){
   highSide(); 
   lowSide();  
}

void sendZero(){
  lowSide(); 
  highSide();
}

void counter2(){
    
    if(UIDcount == 0xFF){
        UIDcount = 0x00; 
    }
    else{
        UIDcount++;
    }

}

char strength(char spell){

    if(spell == damage){
        return 0x64; 
    }
    else if( spell == damageteam){ 
        return 0x96;
    }
    else if( spell == fire){ 
        return 0x96;
    }
    else if( spell == fireteam){ 
        return 0x96;
    }
    else if( spell == water){ 
        return 0x64;
    }
    else if( spell == waterteam){ 
        return 0x64;
    }
    else if( spell == wind){ 
        return 0x64;
    }
   else if( spell == windteam){ 
        return 0x64;
    }
    else if(spell == ice){ 
        return 0x64;
    }
    else if(spell == iceteam){ 
        return 0x64;
    }
    else if(spell == lightning){ 
        return 0x14;
    }
   else if (spell == lightningteam){ 
        return 0x14;
    }
    
    return 0x00; 
    
}

void startSeq(){
   highSide();
   highSide();
   lowSide();
   lowSide();
}
  
void austinSpell(char spell) {
    
    if(spell == damage || spell == damageteam){
       //needs to alternate
        if ((_ir1 == 0) && (_ir4 == 0)) {//left is being output
            //then switch to right side
            _irOn4 = 0; 
            _irOn2 = 0;
            _irOn3 = 1; 
            _irOn1 = 1;
        }
    }
    else if(spell == fire || spell== fireteam){
        _irOn4 = 0; 
        _irOn2 = 0; 
        _irOn1 = 0; 
        _irOn3 = 0; 
    }
    else if(spell == wind || spell == windteam){
        _irOn4 = 0; 
        _irOn2 = 0; 
        _irOn1 = 0; 
        _irOn3 = 0; 
    }
    else if(spell == water || spell == waterteam){
        _irOn1 = 0;
        _irOn2 = 1; 
        _irOn4 = 1; 
        _irOn3 = 1; 
    }
    else if(spell == ice || spell == iceteam){
        _irOn4 = 0; 
        _irOn2 = 0;
        _irOn1 = 0;  
        _irOn3 = 1;  
    }
    else if(spell == lightning || spell == lightningteam){
        //needs to be randomized
        _irOn4 = 0; 
        _irOn2 = 0; 
        _irOn1 = 0; 
        _irOn3 = 1;  
        }
}

void IRdirection() {
    //ir1 is FRONT
    //ir2 is LEFT
    //ir3 is RIGHT
    //ir4 is REAR
    
    
    //sky portal is open 
    //sky key Healing wind spell, spell strength 100, 
    //cast every 3 seconds in 360o direction.
    if (currentPortal == 1) {
        _irOn1 = 0;//0 is an enabled IR
        _irOn2 = 0;
        _irOn3 = 0;
        _irOn4 = 0;
    }
    
    //water portal
    //Damage water spell, spell strength 100, 
    //cast every 7.5 seconds in 90o cone in front
    else if (currentPortal == 2) {
        //only turn the FRONT ir on.
        _irOn1 = 0;//0 is an enabled IR
        _irOn2 = 1;//1 is disabled
        _irOn3 = 1;
        _irOn4 = 1;
    }
    
    
    //flame portal
    //Damage fire spell, spell strength 150, 
    //cast every second in 360o direction.
    else if (currentPortal == 3) {
        _irOn1 = 0;//0 is an enabled IR
        _irOn2 = 0;
        _irOn3 = 0;
        _irOn4 = 0;
    }
    
    
    //arc portal
    //Damage Lightning spell, spell strength 20, 
    //cast every 0.5 second in a random 90o cone in front, left or right side.
    else if (currentPortal == 4) {

        char seed = TMR4; //get a random(ish) number
        char mask = 0x03; //want the last 2 bits
        
        if ((seed & mask) == 1) {//then turn on front
            _irOn1 = 0;//1 is disabled IR
            _irOn2 = 1;
            _irOn3 = 1;
            _irOn4 = 1;
        }
        else if ((seed & mask) == 2) {
            _irOn1 = 1;//1 is disabled IR
            _irOn2 = 0;
            _irOn3 = 1;
            _irOn4 = 1;
        }
        else if ((seed & mask) == 3) {
            _irOn1 = 1;//1 is disabled IR
            _irOn2 = 1;
            _irOn3 = 0;
            _irOn4 = 1;
        }
        else if ((seed & mask) == 4) {
            _irOn1 = 1;//1 is disabled IR
            _irOn2 = 1;
            _irOn3 = 1;
            _irOn4 = 0;
        }
        
    }

    
    //ice portal
    //Stun ice spell, spell strength 100, 
    //cast every 5 seconds in 270o cone in front.
    else if (currentPortal == 5) {
        _irOn1 = 0;//0 is an enabled IR
        _irOn2 = 0;
        _irOn3 = 0;
        _irOn4 = 1;//270 degree cone means back is off
    }
  
    
    //plasma portal
    //Damage normal spell, spell strength 100,
    //cast every second in 90o cone alternating between the left and right sides.
    else if (currentPortal == 6) {
        if (_irOn2 == 0) {//left is on
            //turn on the right
            _irOn1 = 1;//1 is disabled IR
            _irOn2 = 1;
            _irOn3 = 0;
            _irOn4 = 1;
        }
        else if (_irOn3 == 0) {//right is on
            //turn on the left
            _irOn1 = 1;//1 is disabled IR
            _irOn2 = 0;
            _irOn3 = 1;
            _irOn4 = 1;
        }
        else {//something is weird
            
        }
    }
    
    //unknown portal
    else if (currentPortal >= 7) {
        //dont shoot IR anywhere
        _irOn1 = 1;//1 is disabled IR
        _irOn2 = 1;
        _irOn3 = 1;
        _irOn4 = 1;
        UART_SEND("[ircomplete>IRdirection]Currentportal is earth\r\n");
    }
    else {//unxpected case handler
        UART_SEND("[ircomplete>IRdirection]Current portal is erroneous\r\n");
    }
}

void sendPacket(char packet){

    char charTemp;
    int j;
    char compare;
    char mask = 0x01;
    char tempMask;
    
    charTemp = packet;
    
        for(j = 7; j >= 0; j--) { //loop through each bit
            tempMask = mask << j;
            compare = tempMask&charTemp;
            if (compare != 0) {
                sendOne(); 
            }
            else {
                sendZero();
            }

        }//end 
    
     
}

void merge(char spell){
    
    unsigned char idH = PID[0];
    unsigned char idL = PID[1];
    unsigned char packet[6];
    unsigned char cnt = UIDcount;
    unsigned char str = currentStr;//defined in portal call
    unsigned char crc = ('\xFF' - idH - idL - cnt - spell - str);

    packet[0] = idH; 
    packet[1] = idL; 
    packet[2] = (spell); 
    packet[3] = str; 
    packet[4] = cnt; 
    packet[5] = crc; 


    IRdirection(); //turn on irs depending on what portal is open, and change direction if necessary
    
    startSeq(); 
    
    sendPacket(packet[0]); 
    sendPacket(packet[1]); 
    sendPacket(packet[2]); 
    sendPacket(packet[3]); 
    sendPacket(packet[4]); 
    sendPacket(packet[5]); 
     
    counter2();
    
    //__delay_ms(1000);
    
} 

#endif  /* XC_HEADER_TEMPLATE_H */





