#include "main.h"
#include "aptcode.h"
#include "image.h"
#include "audioset.h"

AptTelemetry TelemetryA = {0, 105, 105, 105, 105, 158, 0, 60, 60, 100, 135, 164, 192, 220, 245, 255};
AptTelemetry TelemetryB = {0, 105, 105, 105, 105, 160, 150, 160, 60, 100, 135, 165, 192, 220, 245, 255};

double time_taken;
TgaImageHead ReadTga;

void *SoundThread(void *vargp) { 
      //Sound Buffer    
      unsigned int i = 0; 
	  //unsigned int pix = 0;
      uint16_t imageline = 0;
      //uint8_t AptPix = 0;
      int16_t carrier = 0;
      int16_t aptdata  = 0;
	  uint8_t frame = 1;
	  uint8_t currentline = 1;
	  AptLineAr ConvLine;
      double carrier_phase=0; // current phase 
      double carrier_delta = WF_TPI*APT_CARRIER_F/WF_SAMPLE_RATE;  // delta phase of carrier 2400 Hz	
      //uint8_t AptDataBuffer[WF_SAMPLE_RATE/WF_BUFFER_DIV];
      clock_t t; 
      while(1) {
        t = clock();
        /*
        for(i=0;i<APT_LINE_SIZE;i++) {
          pix = ReadTGAPixel(OutImage);
          AptDataBuffer[i] = GetBlueSubPixel(pix);         
        }*/
		
		//for(j=0;j<ReadTga.Height;j++) {
		ConvLine = AptTransImageLine(frame, currentline, ReadTga, TelemetryA, TelemetryB);  
		frame++;
		currentline++;
		if(frame > APT_FRAME_SIZE) {
			frame = 1;
		}    
		if(currentline > APT_MARKER_SIZE) {
			currentline = 1;
		}            
		//}
        imageline++;

        //1s Sine Wave Syntheis
        for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
            carrier = 32*(sin(carrier_phase)); //amplitude sine carrier wave
            //carrier = 1;           
            //aptdata = (AptDataBuffer[(unsigned int)(i/APT_WORD_MUL)]);
			aptdata = (ConvLine.Value[(unsigned int)(i/APT_WORD_MUL)]);
            carrier = (carrier * aptdata)+carrier*32;
            carrier_phase += carrier_delta;	
            audio_buffer[i]=carrier;
        }
        
        
        if(write(AudioDevice, audio_buffer, sizeof(audio_buffer)) != WF_SAMPLE_RATE/WF_BUFFER_DIV*WF_SAMPLEBITS/8) {
          perror("wrote wrong number of bytes");
        }
        if(imageline >= ReadTga.Height) {
          printf("\nEnd of Image\nAnother loop\n");
          fseek(ReadTga.File, IMG_TGA_HEAD_SIZE, SEEK_SET);
          imageline = 0;
          
          //break;
        }
        t = clock() - t;
        time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
         
      }
    printf("Return: 0 to exit\n");  
    return NULL; 
} 

int main()
{
	printf("Build: %s %s, GCC %s\n", __TIME__, __DATE__, __VERSION__);
    AudioDevice = InitAudioDevice();
    if(AudioDevice < 0) {
      exit(1);
    }	
	ReadTga = OpenTgaImage("169049.tga");
    if(ReadTga.File == NULL) {    
      exit(1);
    }
    FILE *RGfile=NULL;   
    InImage = ReadTga.File;
    RGfile = ReadTga.File;
    //OutImage = fopen("fileAPT_out.tga","wb");
    
    //write header
    /*
   fwrite_int(ReadTga.IDLength,1,OutImage);
   fwrite_int(ReadTga.ColorMapType,1,OutImage);
   fwrite_int(ReadTga.ImageType,1,OutImage);
   fwrite_int(ReadTga.CMapStart,2,OutImage);
   fwrite_int(ReadTga.CMapLength,2,OutImage);
   fwrite_int(ReadTga.CMapDepth,1,OutImage);
   fwrite_int(ReadTga.XOffset,2,OutImage);
   fwrite_int(ReadTga.YOffset,2,OutImage);
   fwrite_int(APT_LINE_SIZE,2,OutImage);
   fwrite_int(ReadTga.Height,2,OutImage);
   fwrite_int(ReadTga.PixelDepth,1,OutImage);
   fwrite_int(ReadTga.ImageDescriptor,1,OutImage);*/
   
   ImageMaxRes = ReadTga.Height;
   /*AptLine AptTrans;
   AptLineAr ConvLine;
   uint8_t frame=1;
   uint32_t currentline = 1;
   uint8_t APTdata = 0;
   unsigned int i,j;*/
   /*
   for(j=0;j<ReadTga.Height;j++) {
	 ConvLine = AptTransImageLine(frame, currentline, ReadTga, TelemetryA, TelemetryB);  
     //AptTrans = CreateAptLine(frame, currentline, TelemetryA, TelemetryB, InImage);
     frame++;
     currentline++;
     if(frame > APT_FRAME_SIZE) {
       frame = 1;
     }    
     if(currentline > APT_MARKER_SIZE) {
       currentline = 1;
     }            
     for(i=0;i<APT_LINE_SIZE;i++) {
       APTdata = ConvLine.Value[i];
       WriteTGAPixel(APTdata, APTdata, APTdata, OutImage);
     }
   }*/
   
    //fclose(OutImage);
    //OutImage = fopen("fileAPT_out.tga","rb");
    //fseek(OutImage, IMG_TGA_HEAD_SIZE, SEEK_SET);  	
    
    pthread_t sound_buf; 
    printf("Create thread\n");
    pthread_create(&sound_buf, NULL, SoundThread, NULL);
    printf("Connected thread\n");
    pthread_detach(sound_buf); 
    //printf("All\n");
  
    unsigned int enterf = 0;
    while(1) {
      //printf("Time took %f seconds to execute \n", time_taken); 
      //printf("Enter frequency 1: ");
      //scanf("%d", &enterf);
      //freq1 = enterf;
      //deltaph1 = WF_TPI*freq1/WF_SAMPLE_RATE;  // delta phase
      printf("APT> Enter 0 to exit: ");
      scanf("%d", &enterf);
      if(enterf==0) {
         printf("\nEnding \n"); 
		 ReadTga.File = RGfile;
         fclose(ReadTga.File);
         exit(0);
      } 
      if(enterf==1) {
        printf("Loop time: %f sec\n",time_taken);
      }	
	  if(enterf==2) {
        if(ReadTga.File==NULL) {
		  ReadTga.File = RGfile;
		  //RGfile = ReadTga.File;
		}	
		else {
		  RGfile = ReadTga.File;	
	      ReadTga.File = NULL; 
		}
		
      }
	  
    }
  
    return 0;
}
