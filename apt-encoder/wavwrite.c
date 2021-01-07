#include "main.h"
#include "image.h"
#include "audioset.h"
#include "wavwrite.h"

unsigned int wav_head(double tdur, unsigned int fs, unsigned int bits, FILE *filename){
   unsigned int numsa = (unsigned int)(tdur*fs);   // overall number of samples
   unsigned int subchunk2 = (numsa * WF_CHANNELS * bits)/8; //
   unsigned int blockAlign = (WF_CHANNELS*bits)/8;      //block size 
   unsigned int byteRate = (fs * WF_CHANNELS * bits)/8; //byte rate per sample
   // chunk id RIFF
   fwrite("RIFF", 4, 1, filename);
   // chunk size    
   fwrite_int(36 + subchunk2, 4, filename);
   // spec. RIFF form for WAV  
   fwrite("WAVE", 4, 1, filename);
   // subchunk1 id  format description       
   fwrite("fmt ", 4, 1, filename); 
   // subchunk1 size: 16 for PCM
   fwrite_int(16, 4, filename); 
   // audio_format: 1 = PCM
   fwrite_int(1, 2, filename);   
   // channels: mono   
   fwrite_int(WF_CHANNELS, 2, filename);
   // sample rate
   fwrite_int(fs, 4, filename);
   // byte rate        
   fwrite_int(byteRate, 4, filename);   
   // block align, byte rate					
   fwrite_int(blockAlign, 2, filename); 
   // bits per sample, 16 bits
   fwrite_int(bits, 2, filename);    
   // subchunk2 id  data content
   fwrite("data", 4, 1, filename); 
   // subchunk2 size
   fwrite_int(subchunk2, 4, filename);   
   return numsa;
}

FILE *wav_open(char *filename) {
  char msg[255];
  FILE *result;
  if((result = fopen(filename, "wb")) == NULL){
    strcpy(msg, "Error opening ");
    strcat(msg, filename);
    perror(msg);
    return NULL;
  }
  return result;
}

