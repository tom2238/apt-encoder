#include "main.h"
#include "aptcode.h"
#include "image.h"
#include "audioset.h"

AptTelemetry TelemetryA = {0, 105, 105, 105, 105, 158, 0, 60, 60, 100, 135, 164, 192, 220, 245, 255};
AptTelemetry TelemetryB = {0, 105, 105, 105, 105, 160, 150, 160, 60, 100, 135, 165, 192, 220, 245, 255};

double time_taken;
TgaImageHead ReadTga;
AptOptSettings aptoptions = {_APT_AUDIO_DEVICE,_APT_FILE_NO_SET,0,0};
FILE *RGfile=NULL; 
uint16_t imageline = 0;
uint16_t transmitted_images = 0;
uint32_t transmitted_frame = 0;
uint32_t transmitted_minutes = 0;

void *SoundThread(void *vargp) { 
  //Sound Buffer    
  unsigned int i = 0; 
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

    ConvLine = AptTransImageLine(frame, currentline, ReadTga, TelemetryA, TelemetryB);  
    frame++;
    currentline++;
    if(frame > APT_FRAME_SIZE) {
      frame = 1;
      transmitted_frame++;
    }    
    if(currentline > APT_MARKER_SIZE) {
      currentline = 1;
      transmitted_minutes++;
    }                   

    //0.5s One line wave syntheis
    for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
      carrier = 32*(sin(carrier_phase)); //amplitude sine carrier wave           
      //aptdata = (AptDataBuffer[(unsigned int)(i/APT_WORD_MUL)]);
      aptdata = (ConvLine.Value[(unsigned int)(i/APT_WORD_MUL)]);
      carrier = (carrier * aptdata)+carrier*32;
      carrier_phase += carrier_delta;	
      audio_buffer[i]=carrier;
    }
        
    if(write(AudioDevice, audio_buffer, sizeof(audio_buffer)) != WF_SAMPLE_RATE/WF_BUFFER_DIV*WF_SAMPLEBITS/8) {
      perror("Wrote wrong number of bytes");
    }
    if(ReadTga.File!=NULL) {
      imageline++;
    }
    if(imageline >= ReadTga.Height) {
      if(aptoptions.loop) {
        //printf("End of image. Another loop.\n");
        transmitted_images++;
        if(ReadTga.File!=NULL) {
          fseek(ReadTga.File, IMG_TGA_HEAD_SIZE, SEEK_SET);
          imageline = 0;
        }
      }
      else {
        CloseImageFile(ReadTga.File, RGfile);
        if(aptoptions.console) {
          rl_free_line_state();
          rl_cleanup_after_signal();
          RL_UNSETSTATE(RL_STATE_ISEARCH|RL_STATE_NSEARCH|RL_STATE_VIMOTION|RL_STATE_NUMERICARG|RL_STATE_MULTIKEY);
          rl_line_buffer[rl_point = rl_end = rl_mark = 0] = 0;
          printf("\n");
        }
        exit(0);
      }
    }
    t = clock() - t;
    time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
         
  } 
  return NULL; 
} 

int main(int argc, char *argv[]) {
  if(argc == 1){
    Usage(argv[0]);
    return 0;
  }
  int opt = 0; 
  while ((opt = getopt(argc, argv, "hi:d:lc")) != -1){
    switch (opt) {
    case 'h': //Help
      Usage(argv[0]);
      return 0;
      break;
    case 'i': //Input TGA image
      strncpy(aptoptions.filename,optarg,sizeof(aptoptions.filename)-1);
      break;
    case 'd': //Device
      aptoptions.device = optarg;
      break;  
    case 'l': //Infinite loop
      aptoptions.loop = 1;
      break;  
    case 'c': //User console
      aptoptions.console = 1;
      break;  
    case '?': //Unknown option
      //printf("  Error: %c\n", optopt);
      return 1;
    default:
      Usage(argv[0]);
      return 0;  
    } 
  }	  
      
  if(strncmp(aptoptions.filename,_APT_FILE_NO_SET,4) == 0) {
    printf("%s: required argument and option -- '-i <filename>'\n",argv[0]);
    exit(2);
  }
  AudioDevice = InitAudioDevice(aptoptions.device);
  if(AudioDevice < 0) {
    exit(1);
  }	
  ReadTga = OpenTgaImage(aptoptions.filename);
  if(ReadTga.File == NULL) {    
    exit(1);
  }
  pthread_t sound_buf; 
  if(pthread_create(&sound_buf, NULL, SoundThread, NULL)!=0){
    perror("Error creating thread\n");
    exit(1);
  }
  if(pthread_detach(sound_buf)!=0){
    perror("Error detaching thread\n");
    exit(1); 
  }
  
  RGfile = ReadTga.File;    	    
  
  if(aptoptions.console) {
    char *consoleinput;
    printf("Type help to show commands\n");	
    while(1) {
      consoleinput = readline("APT> ");
      if(!consoleinput){
        CloseImageFile(ReadTga.File, RGfile);
        printf("\n");
        exit(0);
      }
      add_history(consoleinput);
      if(!strncmp(consoleinput,"exit",4)){
        CloseImageFile(ReadTga.File, RGfile);
        exit(0);
      }
      if(!strncmp(consoleinput,"help",4)){
        printf("help  - print help\n");
        printf("empty - encode empty data\n");
        printf("image - encode image data\n");
        printf("info  - show information\n");
        printf("load  - load new image\n");
        printf("exit  - exit from APT\n") ;	
      }
      if(!strncmp(consoleinput,"empty",5)) {
        if(ReadTga.File!=NULL) {
          RGfile = ReadTga.File;
          ReadTga.File = NULL;
        }
      }
      if(!strncmp(consoleinput,"image",5)) {
        if(RGfile!=NULL) {
          ReadTga.File = RGfile;
          RGfile = NULL;
        }
      }
      if(!strncmp(consoleinput,"load",4)) {
        char newimage[1024];
        printf("New image filename: ");
        scanf("%s",newimage);
        TgaImageHead NewTgaFile = OpenTgaImage(newimage);
        if(NewTgaFile.File != NULL) { 
          strncpy(aptoptions.filename,newimage,sizeof(newimage)-1);  
          imageline = 0;
          CloseImageFile(ReadTga.File, RGfile);
          if(ReadTga.File == NULL) {
            ReadTga = NewTgaFile;
            RGfile = NewTgaFile.File;
            ReadTga.File = NULL;
          }
          else {
            ReadTga = NewTgaFile;
            RGfile = NULL;            
          }    
        }
      }
      if(!strncmp(consoleinput,"info",4)) {
        printf("Sound thread loop time: %lf sec\n",time_taken);
        printf("Output audio device: %s, ",aptoptions.device);
        printf("Sample rate: %d Hz, Bits: %d, Channels: %d\n",WF_SAMPLE_RATE,WF_SAMPLEBITS,WF_CHANNELS);
        printf("Input image: %s , ",aptoptions.filename);
        printf("Width: %dpx, Height: %dpx, ",ReadTga.Width, ReadTga.Height);
        printf("Transmiting - ");
        if(ReadTga.File==NULL){
          printf("no\n");
        }
        else {
          printf("yes\n");
        }
        printf("Time to transmit: %d sec, ",ReadTga.Height/2);
        printf("Current line %d, %d%%\n",imageline,(int)(100*imageline/ReadTga.Height));
        printf("Total transmitted frames %d, ",transmitted_frame);
        printf("Total transmitted minutes %d, ",transmitted_minutes);
        printf("Image loops: %d\n",transmitted_images);
      }	
    }
  }
  else {
    while(1) {
      sleep(1);	
    }
  }
  return 0;
}

void Usage(char *p_name) {
  printf("NOAA automatic picture transmission (APT) encoder\n");
  printf("Usage: %s -i <file> [-d <device> -lc]\n",p_name);
  printf("  -i <filename> Input TGA image (909px width, 24bit RGB)\n");
  printf("  -d <device>   OSS audio device (default /dev/dsp)\n");
  printf("  -l            Enable infinite image loop\n");
  printf("  -c            Enable user console\n");
  printf("  -h            Show this help\n");
  printf("                Build: %s %s, GCC %s\n", __TIME__, __DATE__, __VERSION__); 
}

void CloseImageFile(FILE *reg, FILE *alt) {
  if(reg==NULL) {
    fclose(alt);
  }
  else {
    fclose(reg); 
  }
}
