/* 
 * File:   RBG_animations.h
 * Author: ROMIL
 *
 * Created on March 30, 2016, 3:22 AM
 */

#ifndef RGB_ANIMATIONS_H
#define	RGB_ANIMATIONS_H
    /**********ANIMATIONS***************************/
    static unsigned int animRGBprog[8];//progress of each LED. 
   
    //sky is sky blue, RGB=300,4000,4000.. then fade to dark blue, then quickly go to white and less quickly back to sky
    static unsigned int animSky[] = {200,500,4000,4000,150,0,2000,4000,150,2500,2500,2500,-1};
    
    //quickly alternate between red ,orange and yellow 
    static unsigned int animFire[] = {550,4000,0,0,550,4000,2000,0,550,1000,500,0,-1};
   
    //slowly fade the blue light... go blue, the aqua, then blue, then dark, 
    static unsigned int animWater[] = {250,0,0,3500,250,0,1000,4000,250,0,0,3500,250,0,0,300,-1};
   
    //flash yellow white purple and dark
    static unsigned int animArc[] = {5000,4000,4000,0,2500,0,0,0,2500,2000,0,3500,5000,4000,4000,4000,750,0,0,0,-1};
   
    //Ice blue, then dark, then quickly dull white, repeat - iceblue = 20%,30%,100%RGB
    //should be a blink of white,then dark then ice blue again
    //ICE BLUE..slowly to blue and then dark blue, then fade, QUICK WHITE, then DARK, then slowly back up to blue and repeat
    static unsigned int animIce[] = {300,750,2300,4000,300,3000,3000,3000,300,0,0,0,2500,2500,2500,2500,2500,0,0,0,2500,2500,2500,2500,2500,0,0,0,300,3000,3000,3000,-1};
    
    //purple and red, yo. 
    static unsigned int animPlasma[] = {750,3000,0,0,750,3000,0,3000,-1};
    
    //police! red then blue
//    static unsigned int animWrong[] = {5000,4000,0,0,5000,0,0,4000,-1};
    
    //earth is green then blue then red
    static unsigned int animEarth[] = {500,4000,0,0,500,0,4000,0,500,0,0,4000,-1};
    
    //hobbits will be playing, so do dark and green slowly 
    static unsigned int animIdle[] = {150,0,2500,400,150,0,1200,0,100,0,0,0,-1};
    
    //fades it dark
    static unsigned int animOff[] = {750,0,0,0,-1};
    //ANIMNONE USED TO BE -6
    static unsigned int animNone[] = {35,1500,0,0,35,0,1500,0,35,0,0,1500,-1};//Skip the RGB update function when this is the value
    static unsigned int *animArray = animNone;//contains the animation that we want right now

void blinkAnim(int num) {
    int i;
    for (i = 0; i < num; i++) {
    memset(pwmbuffer,0xff,sizeof(pwmbuffer));
    write_RGB();
    __delay_ms(50);
    memset(pwmbuffer,0x00,sizeof(pwmbuffer));
    write_RGB();
    __delay_ms(50);
    }

    return;
}

void animBad(int num) {
    int i;
    for (i = 0; i < num; i++) {
    //write red to the first set
        pwmbuffer[0] = 0x000;//l#1, R
        pwmbuffer[1] = 0x000;//l#1, G
        pwmbuffer[2] = 0xfff;//l#1, B
        pwmbuffer[3] = 0xfff;//l#2, R
        pwmbuffer[4] = 0x000;//l#2, G
        pwmbuffer[5] = 0x000;//l#2, B
        pwmbuffer[6] = 0x000;//l#3, R
        pwmbuffer[7] = 0x000;//l#3, G
        pwmbuffer[8] = 0xfff;//l#3, B
        pwmbuffer[9] = 0xfff;//l#4, R
       pwmbuffer[10] = 0x000;//l#4, G
       pwmbuffer[11] = 0x000;//l#4, B
       pwmbuffer[12] = 0x000;//l#5, R
       pwmbuffer[13] = 0x000;//l#5, G
       pwmbuffer[14] = 0xfff;//l#5, B
       pwmbuffer[15] = 0xfff;//l#6, R
       pwmbuffer[16] = 0x000;//l#6, G
       pwmbuffer[17] = 0x000;//l#6, B
       pwmbuffer[18] = 0x000;//l#7, R
       pwmbuffer[19] = 0x000;//l#7, G
       pwmbuffer[20] = 0xfff;//l#7, B
       pwmbuffer[21] = 0xfff;//l#8, R
       pwmbuffer[22] = 0x000;//l#8, G
       pwmbuffer[23] = 0x000;//l#8, B
       write_RGB();
	    __delay_ms(150);
    //write blue to the second set
	    pwmbuffer[0] = 0xfff;//l#1, R
        pwmbuffer[1] = 0x000;//l#1, G
        pwmbuffer[2] = 0x000;//l#1, B
        pwmbuffer[3] = 0x000;//l#2, R
        pwmbuffer[4] = 0x000;//l#2, G
        pwmbuffer[5] = 0xfff;//l#2, B
        pwmbuffer[6] = 0xfff;//l#3, R
        pwmbuffer[7] = 0x000;//l#3, G
        pwmbuffer[8] = 0x000;//l#3, B
        pwmbuffer[9] = 0x000;//l#4, R
       pwmbuffer[10] = 0x000;//l#4, G
       pwmbuffer[11] = 0xfff;//l#4, B
       pwmbuffer[12] = 0xfff;//l#5, R
       pwmbuffer[13] = 0x000;//l#5, G
       pwmbuffer[14] = 0x000;//l#5, B
       pwmbuffer[15] = 0x000;//l#6, R
       pwmbuffer[16] = 0x000;//l#6, G
       pwmbuffer[17] = 0xfff;//l#6, B
       pwmbuffer[18] = 0xfff;//l#7, R
       pwmbuffer[19] = 0x000;//l#7, G
       pwmbuffer[20] = 0x000;//l#7, B
       pwmbuffer[21] = 0x000;//l#8, R
       pwmbuffer[22] = 0x000;//l#8, G
       pwmbuffer[23] = 0xfff;//l#8, B
       write_RGB();
	   __delay_ms(150);

    }

    return;
}

void setRand() {
    int i;
    for (i = 0; i < 25; i++) {
        pwmbuffer[i] = (rand()%10)*200;
        
    }
    write_RGB();
}


#endif	/* RBG_ANIMATIONS_H */

