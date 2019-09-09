#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <getopt.h>
#include "image.h"
#include "aptcode.h"

#define _APT_FILE_NO_SET "σame"

void Usage(char *p_name);

int main(int argc, char *argv[]) {
  if(argc == 1){
    Usage(argv[0]);
    return 0;
  }
  char channel_a[1024]=_APT_FILE_NO_SET;
  char channel_b[1024]=_APT_FILE_NO_SET;
  char output[1024]=_APT_FILE_NO_SET;  
  int opt = 0; 
  while ((opt = getopt(argc, argv, "a:b:o:h")) != -1){
    switch (opt) {
    case 'h': //Help
      Usage(argv[0]);
      return 0;
      break;
    case 'a': //Channel A TGA image
      strncpy(channel_a,optarg,sizeof(channel_a)-1);
      break;
    case 'b': //Channel B TGA image
      strncpy(channel_b,optarg,sizeof(channel_b)-1);
      break;  
    case 'o': //Output TGA image
      strncpy(output,optarg,sizeof(output)-1);
      break;  
    default:
      Usage(argv[0]);
      return 0;  
    } 
  }	  
  if(strncmp(channel_a,_APT_FILE_NO_SET,4) == 0) {
    printf("%s: required argument and option -- '-a <filename>'\n",argv[0]);
    exit(2);
  }
  if(strncmp(channel_b,_APT_FILE_NO_SET,4) == 0) {
    printf("%s: required argument and option -- '-b <filename>'\n",argv[0]);
    exit(2);
  }
  if(strncmp(output,_APT_FILE_NO_SET,4) == 0) {
    printf("%s: required argument and option -- '-o <filename>'\n",argv[0]);
    exit(2);
  }
  TgaImageHead ChannelA, ChannelB, OutputColor;
  ChannelA = OpenTgaImage(channel_a);
  ChannelB = OpenTgaImage(channel_b);
  OutputColor = ChannelA;
  OutputColor = WriteTgaImage(output, OutputColor);
  if(ChannelA.File == NULL) {  // On error 
    exit(1);
  }
  if(ChannelB.File == NULL) {  // On error 
    exit(1);
  }
  if(OutputColor.File == NULL) {  // On error 
    exit(1);
  }
  
  int i,j;
  unsigned int pix_a;
  unsigned int pix_b;
  HsvColor hsvc;
  AptColor aptc;
  RgbColor rgbc;
  // 24 bit RGB, but in grayscale expected
  for(i=0;i<ChannelA.Width;i++) {
     for(j=0;j<ChannelA.Height;j++) {
       pix_a = ReadTGAPixel(ChannelA.File);
       pix_b = ReadTGAPixel(ChannelB.File);
       aptc.h = GetRedSubPixel(pix_a);  // X
       aptc.sv = GetRedSubPixel(pix_b); // Y

       rgbc = AptToRgb(aptc);
       
       WriteTGAPixel(rgbc.r,rgbc.g,rgbc.b, OutputColor.File);
     }
   }
  fclose(ChannelA.File);
  fclose(ChannelB.File);
  fclose(OutputColor.File); 
  return 0;
}

void Usage(char *p_name) {
  printf("NOAA color mode decoder\n");
  printf("Usage: %s -a <filename> -b <filename> -o <filename> -h\n",p_name);
  printf("  -a <filename> Input channel A TGA image (909px width, 24bit RGB)\n");
  printf("  -b <filename> Input channel B TGA image (909px width, 24bit RGB)\n");
  printf("  -o <filename> Output color TGA image \n");
  printf("  -h            Show this help\n");
  printf("                Build: %s %s, GCC %s\n", __TIME__, __DATE__, __VERSION__); 
}

