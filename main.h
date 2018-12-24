#ifndef NA_MAIN_H_   
#define NA_MAIN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/soundcard.h>
#include <stdint.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <getopt.h>

void *SoundThread(void *vargp); //Audio thread 
int AudioDevice; //audio device
FILE *InImage; //Input image
FILE *OutImage;//Output image

uint16_t ImageMaxRes; //Maximum image resolution

#endif
