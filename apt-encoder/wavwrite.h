#ifndef _WAV_WRITE_APT
#define _WAV_WRITE_APT

//Write wav file header
unsigned int wav_head(double tdur, unsigned int fs,unsigned int bits, FILE *filename);
//Create and open new wav file
FILE *wav_open(char *filename);

#endif
