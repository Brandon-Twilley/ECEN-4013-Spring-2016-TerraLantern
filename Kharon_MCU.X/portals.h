/* 
 * File:   portals.h
 * Author: ROMIL
 *
 * Created on March 30, 2016, 3:23 AM
 */

#ifndef PORTALS_H
#define	PORTALS_H

#include "audio_declaration.h"

//SPELLS 
#define sp_damage	0x1
#define sp_stun	 	0x3
#define sp_heal 	0x5
#define sp_fire 	0x7 
#define sp_water 	0x9
#define sp_earth 	0x11
#define sp_wind 	0x13
#define sp_ice 		0x15
#define sp_rock 	0x17
#define sp_thunder 	0x19
#define sp_poison 	0x21
#define sp_psychic 	0x23
#define sp_ghost 	0x25
#define sp_shadow 	0x27 
#define sp_light 	0x29
//add +1 to the spell to make it a team spell

void portalCollapse() {//is called whenever an active portal dies
    //set currentportal to cooldown
    currentPortal = -1;
    
    //disable IR stuff
    T3CONbits.TON = 0; //start Timer 3 - IR timing interrupts
    //

    //play the portal collapse sound
    U1CHAN("SOMO");
    single_play(sm.portal_wat,&c);
    U1CHAN("DEBUG");
    
    
    UART_SEND("Portal collapsed!\r\n");
    //blink the lights twice
//    blinkAnim(3);
    
    animArray = animOff;//causes the LEDs to fade away
    return;
}//end portalcollapse

//the 'portal' that is 'open' to indicate idle stuff.
void portalIdle() {//is called whenever the portal is waiting for input
    currentPortal = 0;
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 0; //start Timer 3 - IR timing interrupts

    //portal collapse timer
    TMR1CNT = 0;
    T1CONbits.TON = 0; // turn on portal collapse counter
    
    U1CHAN("SOMO");
    single_play(sm.portal_ready,&c);
    U1CHAN("DEBUG");
    __delay_ms(500);
    //send play idle song to somo
    U1CHAN("SOMO");
    play(sm.hobbits,&c);
    U1CHAN("DEBUG");

//    blinkAnim(2);
//    noWorldVisuals();
    animArray = animIdle;
    UART_SEND("[DBG] PORTAL IS IDLING\r\n");
}

//sky key Healing wind spell, spell strength 100, 
//cast every 3 seconds in 360o direction.
void portalSky() {
    currentPortal = 1; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_wind; //set the spell to use
    currentStr = 100;//set the spell strength
    TMR3CNTVAL = 30;//set the frequency.. TMR3CNTVAL is the number of 1/10 seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interrupts
    
    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
  //  TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play plasma song to somo
    U1CHAN("SOMO");
    play(sm.sky,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    windWorldVisuals(); //light show! (+lcd stuff)
    animArray = animSky;
    UART_SEND("sky portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portalsky

//Damage water spell, spell strength 100, 
//cast every 7.5 seconds in 90o cone in front
void portalWater(){
    currentPortal = 2; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_water; //set the spell to use
    currentStr = 100;//set the spell strength
    TMR3CNTVAL = 75;//set the frequency.. TMR3CNTVAL is the number of seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interrupts
   
    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
 //   TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play plasma song to somo
    U1CHAN("SOMO");
    play(sm.aqua,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    waterWorldVisuals(); //light show! (+lcd stuff)
    animArray = animWater;
    UART_SEND("Aqua portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portalwater

//Stun ice spell, spell strength 100, 
//cast every 5 seconds in 270o cone in front.
void portalIce() {
    currentPortal = 5; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_ice; //set the spell to use
    currentStr = 100;//set the spell strength
    TMR3CNTVAL = 50;//set the frequency.. TMR3CNTVAL is the number of seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interruptsNT = 0; //send a spell immediately
 
    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
//    TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play ice song to somo
    U1CHAN("SOMO");
    play(sm.ice,&c);
    U1CHAN("DEBUG");

    //send ice visuals to RGBs
    iceWorldVisuals(); //light show! (+lcd stuff)
    animArray = animIce;
    UART_SEND("Ice portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portalice

//Damage fire spell, spell strength 150, 
//cast every second in 360o direction.
void portalFire(){
    currentPortal = 3; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_fire; //set the spell to use
    currentStr = 150;//set the spell strength
    TMR3CNTVAL = 10;//set the frequency.. TMR3CNTVAL is the number of seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interrupts
 
    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
  //  TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play fire song to somo
    U1CHAN("SOMO");
    play(sm.fire,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    fireWorldVisuals(); //light show! (+lcd stuff)
    animArray = animFire;
    UART_SEND("Fire portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portalfire

//Damage Lightning spell, spell strength 20, 
//cast every 0.5 second in a random 90o cone in front, left or right side.
void portalLightning(){
    currentPortal = 4; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_thunder; //set the spell to use
    currentStr = 20;//set the spell strength
    TMR3CNTVAL = 5;//set the frequency.. TMR3CNTVAL is the number of seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interrupts
 
    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
//    portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
  //  TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play lightning song to somo
    U1CHAN("SOMO");
    play(sm.arc,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    lightningWorldVisuals(); //light show! (+lcd stuff)
    animArray = animArc;
    UART_SEND("Arc portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portallightning

//Damage normal spell, spell strength 100,
//cast every second in 90o cone alternating between the left and right sides.
void portalPlasma(){
    //ir and bluetooth packet data definitions
    currentPortal = 6; //set the currentportal to the sky portal so it doesnt reopen
    currentSpell = sp_damage; //set the spell to use 
    currentStr = 100;//set the spell strength
    TMR3CNTVAL = 10;//set the frequency.. TMR3CNTVAL is the number of seconds to wait between IR
    TMR3CNT = 0; //send a spell immediately
    T3CONbits.TON = 1; //start Timer 3 - IR timing interrupts
 
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
    __delay_ms(1200);
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
  //  TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    //send play plasma song to somo
    U1CHAN("SOMO");
    play(sm.plasma,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    plasmaWorldVisuals(); //light show! (+lcd stuff)
    animArray = animPlasma;
    UART_SEND("Plasma portal opened: Timer, IR, SOMO, RGB, LCD activated\r\n");
}//end portalplasma

//this is portal for unused key combination
void portalEarth(){//unused key combination
    currentPortal = 7; //set the currentportal to the sky portal so it doesnt reopen

    T3CONbits.TON = 0; //start Timer 3 - IR timing interrupts
    //portal collapse timer
    TMR1CNTVAL = TMR1MAX; //set the sky portal to stay open for 30 seconds before collapsing
  //  TMR1CNT = 0;
    T1CONbits.TON = 1; // turn on portal collapse counter

    
    U1CHAN("SOMO");
    single_play(sm.portal_open,&c);
    U1CHAN("DEBUG");
//    __delay_ms(1200);
    //send play lightning song to somo
    U1CHAN("SOMO");
    play(badsong,&c);
    U1CHAN("DEBUG");

    //send sky visuals to RGBs
    animBad(5); //light show! (+lcd stuff)
    animArray = animEarth;
    UART_SEND("BAD PORTAL!!\r\n");
}//end portalearth


#endif	/* PORTALS_H */

