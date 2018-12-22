#include "main.h"
#include "aptcode.h"
#include "image.h"
#include "audioset.h"

void *SoundThread(void *vargp);
void *SoundData(void *vargp);

unsigned int freq1;
unsigned int freq2;
double carrier_delta;  // delta phase of carrier 2400 Hz	
double deltaph2 ;  // delta phase
int fd; //device
FILE *InImage;
uint8_t state = 0;
AptTelemetry TelemetryA = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
AptTelemetry TelemetryB = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
//double time_taken;

void *SoundThread(void *vargp) { 
      //Sound Buffer
      
      unsigned int i = 0; 
      unsigned int j = 0; 
      int AptState = 0;
      uint32_t currentline = 1; 
      uint8_t frame = 1; 
      int16_t carrier = 0;
      int16_t aptdata  = 0;
      double carrier_phase=0, ph2=0;
      AptLine AptTrans;
      uint8_t AptDataBuffer[WF_SAMPLE_RATE/WF_BUFFER_DIV];
      //clock_t t; 
      while(1) {
        //t = clock();
        AptTrans = CreateAptLine(frame, currentline, TelemetryA, TelemetryB, InImage); 
        currentline++;
        frame++;
        if(frame > 128) {
          frame = 1;
        }
        j = 0;
        for(i=0; i<225;i++){
          AptDataBuffer[i] = AptTrans.SyncA[j];
          j += (unsigned int)(j/APT_WORD_MUL_SYNC);
        }
        j = 0;
        for(i=225; i<496;i++){
          AptDataBuffer[i] = AptTrans.MarkerA[j];
          j += (unsigned int)(j/APT_WORD_MUL_MARK);
        }
        j = 0;
        for(i=496; i<5740;i++){
          AptDataBuffer[i] = AptTrans.VideoA[j];
          j += (unsigned int)(j/APT_WORD_MUL_VIDEO);
        }
        j = 0;
        for(i=5740; i<6000;i++){
          AptDataBuffer[i] = AptTrans.TelemetryA[j];
          j += (unsigned int)(j/APT_WORD_MUL_TELEM);
        }
        j = 0;
        for(i=6000; i<6225;i++){
          AptDataBuffer[i] = AptTrans.SyncB[j];
          j += (unsigned int)(j/APT_WORD_MUL_SYNC);
        }
        j = 0;
        for(i=6225; i<6496;i++){
          AptDataBuffer[i] = AptTrans.MarkerB[j];
          j += (unsigned int)(j/APT_WORD_MUL_MARK);
        }
        j = 0;
        for(i=6496; i<11740;i++){
          AptDataBuffer[i] = AptTrans.VideoB[j];
          j += (unsigned int)(j/APT_WORD_MUL_VIDEO);
        }
        j = 0;
        for(i=11740; i<12000;i++){
          AptDataBuffer[i] = AptTrans.TelemetryB[j];
          j += (unsigned int)(j/APT_WORD_MUL_TELEM);
        }
        
        /*for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
          if(AptState == 0) {
            AptDataBuffer[i] = AptTrans.SyncA[(unsigned int)(i/APT_WORD_MUL)];
          }
          else if(AptState == 1) {
            AptDataBuffer[i] = AptTrans.MarkerA[i];
          }
          else if(AptState == 2) {
            AptDataBuffer[i] = AptTrans.VideoA[i];
          }
          else if(AptState == 3) {
            AptDataBuffer[i] = AptTrans.TelemetryA[i];
          }
          else if(AptState == 4) {
            AptDataBuffer[i] = AptTrans.SyncB[i];
          }
          else if(AptState == 5) {
            AptDataBuffer[i] = AptTrans.MarkerB[i];
          }
          else if(AptState == 6) {
            AptDataBuffer[i] = AptTrans.VideoB[i];
          }
          else if(AptState == 7) {
            AptDataBuffer[i] = AptTrans.TelemetryB[i];
          }
            
        }*/
        //1s Sine Wave Syntheis
        for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
            //carrier = 150*(sin(carrier_phase)); //amplitude sine carrier wave
            carrier = 1;
            aptdata = 16*(AptDataBuffer[i]+128);
            carrier = (carrier * aptdata)+carrier*40;
            carrier_phase += carrier_delta;	
            ph2 += deltaph2;				//phase
            audio_buffer[i]=carrier;
        }
        
        
        if(write(fd, audio_buffer, sizeof(audio_buffer)) != WF_SAMPLE_RATE/WF_BUFFER_DIV*WF_SAMPLEBITS/8) {
          perror("wrote wrong number of bytes");
        }
        //t = clock() - t;
        //time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
         
      }
    return NULL; 
} 

void *SoundData(void *vargp) {
   while(1) {
   
    sleep(1);
   
   }

   return NULL;
} 

int main()
{
    TgaImageHead ReadTga = OpenTgaImage("ic.tga");
    if(ReadTga.File == NULL) {
      exit(1);
    }
    fd = InitAudioDevice();
    if(fd < 0) {
      exit(1);
    }   
    InImage = ReadTga.File;
    

    carrier_delta = WF_TPI*APT_CARRIER_F/WF_SAMPLE_RATE;  // delta phase	
    deltaph2 = WF_TPI*freq2/WF_SAMPLE_RATE;  // delta phase
    
    
    pthread_t sound_buf, sound_data; 
    printf("Create thread\n");
    pthread_create(&sound_buf, NULL, SoundThread, NULL);
    pthread_create(&sound_data, NULL, SoundData, NULL);
    printf("Connected thread\n");
    //pthread_join(sound_buf, NULL);
    pthread_detach(sound_buf); 
    pthread_detach(sound_data);
    //printf("All\n");
  
    unsigned int enterf = 0;
    while(1) {
      //printf("Time took %f seconds to execute \n", time_taken); 
      //printf("Enter frequency 1: ");
      //scanf("%d", &enterf);
      //freq1 = enterf;
      //deltaph1 = WF_TPI*freq1/WF_SAMPLE_RATE;  // delta phase
      printf("Enter frequency 2: ");
      scanf("%d", &enterf);
      freq2 = enterf;
      if(enterf==0) {
         printf("\nEnding \n"); 
         exit(0);
      } 	
      deltaph2 = WF_TPI*freq2/WF_SAMPLE_RATE;  // delta phase
    }
  
    return 0;
}
