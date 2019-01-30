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
#include <readline/readline.h>
#include <readline/history.h>

#define _APT_FILE_NO_SET "σame"
#define _APT_AUDIO_DEVICE "/dev/dsp"

typedef struct {
  char *device;
  char filename[1024];
  uint8_t loop;
  uint8_t console;
  uint8_t datab;
  uint8_t isfile;
  uint8_t chdev;
}AptOptSettings;

typedef struct {
  int device;
  FILE *file;
}AptAudioSettings;

void *SoundThread(void *vargp); //Audio thread 
int AudioDevice; //audio device descriptor
// Print help and usage of application
void Usage(char *p_name);
// Close image
void CloseImageFile(FILE *reg, FILE *alt);

#endif
