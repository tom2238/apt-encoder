#ifndef NA_AUDIO_SETUP_H_   
#define NA_AUDIO_SETUP_H_

//Math
#define   WF_TPI   6.283185307
//Device Parameters
#define   WF_SAMPLE_RATE  24000 //24khz
#define   WF_CHANNELS    1     //mono
#define   WF_SAMPLEBITS  16    //16bits
#define   WF_BUFFER_DIV  2     //buffer divider

//Set Device Parameter with error checking
#define ioset(field, argument)\
{\
    int arg = argument;\
    if(ioctl(fd, field, &arg)<0)\
        perror(#field "ioctl failed");\
    else if(arg!=argument)\
        perror(#argument "was not set in ioctl");\
}

int16_t audio_buffer[WF_SAMPLE_RATE/WF_BUFFER_DIV];

int InitAudioDevice(char *device);
/*
//  else\
//      printf(#argument "\t:= %d\n", arg);\
*/
#endif 
