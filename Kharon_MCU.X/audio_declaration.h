/*
 * File:   audio_declaration.h
 * Author: btwille
 *
 * Created on March 21, 2016, 3:07 PM
 */

#ifndef AUDIO_DECLARATION_H
#define	AUDIO_DECLARATION_H



struct sound_module
{
        char fire;          //0x00
        char sky;           //0x01
        char aqua;          //0x02
        char arc;           //0x03
        char ice;           //0x04
        char plasma;        //0x05
        char key_unlock;    //0x06
        char key_lock;      //0x07
        char start_up;      //0x08
        char rick_roll;     //0x09
        char hobbits;       //0x0A
        char screaming_sun;  //0x0B
        char portal_wat;    //0x0C
        char portal_open;   //0x0D
        char portal_ready; //0x0E
};

struct commands
{
    const char PLAY[8];           //7E 0D 00 00 00 FF F3 EF  This will play the audio track selected.
    const char SLEEP[8];          //7E 0A 00 00 00 FF F6 EF  This will put the SoMo device to sleep.
    const char RESET[8];          //7E 0C 00 00 00 FF F4 EF  This will reset the SoMo to it's powered on state
    const char PAUSE[8];          //7E 0E 00 00 00 FF F2 EF  This will pause the SoMo's current track.  Play will continue where it left off
    const char REPEAT_MODE[8];    //7E 19 00 00 00 FF E7 EF  used to set the current track on repeat.  Audio files must be playing
    const char PLAY_SOURCE[8];     //7E 09 00 00 02 FF F5 EF  used to set how the SoMo-II plays audio files.
    const char CURRENT_TRACK[8];  //7E 4C 00 00 00 FF B4 EF  sends a request to find the current track.
    const char TRACK_TO_PLAY[8];  //7E 03 00 XX ?? FF FC EF  (Replace ?? & XX) This will play track ?? in folder XX
    const char SET_VOLUME[8];     //7E 06 00 00 ?? FF F5 EF  (Replace ??) This will set volume to ?? (Max 30)
    const char SINGLE_PLAY[8];    //7E 19 00 00 01 FF E6 EF  Disables CONTINUOUS, RANDOM TRACK or REPEAT CURRENT
    const char TRACK_REPEAT[8];
    const char CONTINUOUS[8];
    const char REPEAT_FLAG[8];
    const char SPECIFY_TRACK[8];
};

//struct recv
//{
//    const char SUCCESS[8];        //7E 41 00 00 00 FF BF EF Command Successfully Received
//    const char BUSY[8];           //7E 40 00 00 01 FF BF EF Module is busy
//    const char SLEEP[8];          //7E 40 00 00 02 FF BE EF Module is currently in sleep mode
//    const char SERIAL[8];         //7E 40 00 00 03 FF BD EF Serial received an error
//    const char CHECKSUM[8];       //7E 40 00 00 04 FF BC EF Checksum error
//    const char BEYOND_SCOPE[8];   //7E 40 00 00 05 FF BB EF Beyond scope of specified file/folder
//    const char _404_NOT_FOUND[8]; //7E 40 00 00 06 FF BA EF Specified file/Folder not found
//    const char ON_NO_MEDIA[8];    //7E 3F 00 00 00 FF C1 EF SOMO-II has just started up, No media detected
//    const char ON_SD_DETECTED[8]; //7E 3F 00 00 02 FF BF EF SOMO-II has just started up, micro-SD Card detected
//};

static int delay = 50;      //sets the delay for all commands

int UART_SEND0x(const char*,int);
void initialize_somo(struct commands *c);
void copy(const char in1[8],char in2[8]);
void play(char in,struct commands* c);
void set_volume(int in,struct commands* c);
void pause(struct commands*);

void single_play(char in, struct commands *c){
    
    play(in,c);

    char out_buffer[8];
    copy(c->SINGLE_PLAY, out_buffer);
    UART_SEND0x(out_buffer,8);
    __delay_ms(delay);
    

}

void play(char in,struct commands *c)
{
    char out_buffer[8];
    copy(c->TRACK_REPEAT,out_buffer);
    out_buffer[4] = out_buffer[4]+in;
    out_buffer[6] = out_buffer[6]-in;
    UART_SEND0x(out_buffer,8);
        //selects the track.  This causes the song to play
        //without sending the play command.
    __delay_ms(delay);

}

void pause(struct commands *c)
{
    UART_SEND0x(c->PAUSE,8);
    __delay_ms(delay);
}

void set_volume(int in,struct commands* c)
{
    char out_buffer[8];
    copy(c->SET_VOLUME,out_buffer);
    out_buffer[4] = in;
    out_buffer[6] = out_buffer[6]-(char)in;   //Not sure if works :/ TODO: check
    UART_SEND0x(out_buffer,8);
    __delay_ms(delay);
        //int in the volume.  This can only be set between 30 and 0
}

void initialize_somo(struct commands *c)
{
    UART_SEND0x(c->RESET,8);
    __delay_ms(delay*2);

    char out_buffer[8];
    copy(c->SET_VOLUME,out_buffer);
    out_buffer[4] = 0x1E;   //sets volume to 30
    UART_SEND0x(out_buffer,8);
    __delay_ms(delay*2);

        //sets the source to microSD card.
    copy(c->PLAY_SOURCE,out_buffer);
    UART_SEND0x(out_buffer,8);

    __delay_ms(delay*2);

}

void copy(const char in1[8],char in2[8])
{
    int i;
    for(i=0;i<8;i++)
    {
        in2[i]=in1[i];
    }
}
#endif	/* AUDIO_DECLARATION_H */
