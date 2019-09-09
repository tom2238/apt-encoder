#include "main.h"
#include "aptcode.h"
#include "image.h"
#include "audioset.h"
#include "wavwrite.h"

AptTelemetry TelemetryA;
AptTelemetry TelemetryB;

double time_taken;
TgaImageHead ReadTga;
TgaImageHead SecondTga;
AptOptSettings aptoptions = {_APT_AUDIO_DEVICE,_APT_FILE_NO_SET,_APT_FILE_NO_SET,0,0,'N',0,0,0,0,0}; // Common APT settings
FILE *RGfile=NULL; 
FILE *WAVfile=NULL;
uint16_t imageline = 0;
uint16_t imageline_second = 0;
uint16_t transmitted_images = 0;
uint32_t transmitted_frame = 0;
uint32_t transmitted_minutes = 0;

void *SoundThread(void *vargp) { 
  //Sound Buffer    
  TgaImageHead NullTga;
  unsigned int i = 0; 
  int16_t carrier = 0;
  int16_t aptdata  = 0;
  uint8_t frame = 1;
  uint8_t currentline = 1;
  uint8_t waittime = 0;
  AptLineAr ConvLine;
  double carrier_phase=0; // current phase 
  double carrier_delta = WF_TPI*APT_CARRIER_F/WF_SAMPLE_RATE;  // delta phase of carrier 2400 Hz	
  //uint8_t AptDataBuffer[WF_SAMPLE_RATE/WF_BUFFER_DIV];
  NullTga.File = NULL;
  clock_t t; 
  while(1) {
    t = clock();
    if(waittime<10) { // 5 seconds, 10 empty lines, No data (because NullTga)
      ConvLine = AptTransImageLine(frame, currentline, NullTga, NullTga, TelemetryA, TelemetryB, aptoptions.datab);
      waittime++;
    }
    else {
      ConvLine = AptTransImageLine(frame, currentline, ReadTga, SecondTga, TelemetryA, TelemetryB, aptoptions.datab); 
    }
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
      if(!aptoptions.isfile) {
        audio_buffer[i]=carrier;
      }else{
        audio_buffer[i]=carrier*3;
      }
    }
    if(!aptoptions.isfile) {     
      if(aptoptions.usestdout) { // Use STDOUT
        for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
          fwrite_int(audio_buffer[i],2,stdout); // aplay -f S16_LE -r 24000
        }
      } 
      else { // Use OSS device
        if(write(AudioDevice, audio_buffer, sizeof(audio_buffer)) != WF_SAMPLE_RATE/WF_BUFFER_DIV*WF_SAMPLEBITS/8) {
          perror("Wrote wrong number of bytes");
        }
      }
    }
    else { // Writing WAV file
      for(i=0; i<WF_SAMPLE_RATE/WF_BUFFER_DIV; i++) {
        fwrite(&audio_buffer[i],2,1,WAVfile);
      }
    }
    if(ReadTga.File!=NULL) {
      imageline++;
    }
    if((SecondTga.File!=NULL)&&(AptImageSet==2))  {
      imageline_second++;
    }
    if((imageline >= ReadTga.Height)&&(!aptoptions.usestdin)) { // Ignore in STDIN
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
          ClearConsole();
        }
        exit(0);
      }
    }
    if((imageline_second >= SecondTga.Height)&&(AptImageSet==2)&&(!aptoptions.usestdin)) { // Ignore in STDIN
      if(aptoptions.loop) {
        transmitted_images++;
        if(SecondTga.File!=NULL) {
          fseek(SecondTga.File, IMG_TGA_HEAD_SIZE, SEEK_SET);
          imageline_second = 0;
        }
      }
      else {
        fclose(SecondTga.File);
        if(aptoptions.console) {
          ClearConsole();
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
  signal(SIGINT, SignalHandler);
  signal(SIGTERM, SignalHandler);

  if(argc == 1){
    Usage(argv[0]);
    return 0;
  }
  /*logfile = fopen("imgerror.log", "at");
  fprintf(logfile,"(II) APT log file\n");
  fflush(logfile);*/
  int opt = 0; 
  while ((opt = getopt(argc, argv, "hi:s:d:lcm:rIOM")) != -1){
    switch (opt) {
    case 'h': //Help
      Usage(argv[0]);
      return 0;
      break;
    case 'i': //Input TGA image
      strncpy(aptoptions.filename,optarg,sizeof(aptoptions.filename)-1);
      break;
    case 's': //Second input TGA image
      strncpy(aptoptions.secondfile,optarg,sizeof(aptoptions.secondfile)-1);
      break;  
    case 'd': //Device
      aptoptions.device = optarg;
      aptoptions.chdev = 1;
      break;  
    case 'l': //Infinite loop
      aptoptions.loop = 1;
      break;  
    case 'c': //User console
      aptoptions.console = 1;
      break;
    case 'm': //Channel B data mode
      switch (optarg[0]) {
        case 'R': //Red
        case 'G': //Green
        case 'B': //Blue
        case 'N': //Negative
        case 'Y': //Yb
        case 'C': //Color
          aptoptions.datab = optarg[0];
          break;
        default:
          printf("Bad mode: %c , ",optarg[0]);
          printf("Set to N\n");
      }
      break; 
    case 'r':  // Regular file
      aptoptions.isfile=1; 
      break;
    case 'I': // Use STDIN for image data
      aptoptions.usestdin = 1;
      break;  
    case 'O': // User STDOUT for audio
      aptoptions.usestdout = 1;
      break;
    case 'M': // Multi mode on STDIN for image data
      aptoptions.multistdin = 1;
      break;
    case '?': //Unknown option
      //printf("  Error: %c\n", optopt);
      return 1;
    default:
      Usage(argv[0]);
      return 0;  
    } 
  }	  
  // Set telemetry for A and B channels
  TelemetryA = CreateTelemetry(APT_TEL_TEMP1, APT_TEL_TEMP2, APT_TEL_TEMP3, APT_TEL_TEMP4,0,APT_CHANNELID_1);
  TelemetryB = CreateTelemetry(APT_TEL_TEMP1, APT_TEL_TEMP2, APT_TEL_TEMP3, APT_TEL_TEMP4,APT_TEL_BACKSCAN,APT_CHANNELID_1);
  // Check argument
  if(strncmp(aptoptions.filename,_APT_FILE_NO_SET,4) == 0) {
    if(!aptoptions.usestdin) {
      printf("%s: required argument and option -- '-i <filename>'\n",argv[0]);
      exit(2);
    }
  }
  // Open input images
  if(!aptoptions.usestdin) {
    ReadTga = OpenTgaImage(aptoptions.filename);
  }  
  else {
    ReadTga.File = stdin;
  }
  AptImageSet = 1;
  if(ReadTga.File == NULL) {  // On error 
    exit(1);
  }
  if(strncmp(aptoptions.secondfile,_APT_FILE_NO_SET,4)) {
   if(!aptoptions.usestdin) {
     SecondTga = OpenTgaImage(aptoptions.secondfile);
   }
   else {
     SecondTga.File = stdin;
   }
   AptImageSet = 2;
    if(SecondTga.File == NULL) {   // On error 
      exit(1);
    }
  }
  if(aptoptions.usestdout) { // Use STD OUT
    aptoptions.isfile=0;
    aptoptions.console=0;
    aptoptions.loop=0;
    aptoptions.device=_APT_AUDIO_STDOUT;
  }
  if(aptoptions.usestdin) { // Use STD IN
    aptoptions.isfile=0;
    aptoptions.console=0;
    aptoptions.loop=0;
    if(aptoptions.multistdin) {
      AptImageSet = 2;
      SecondTga.File = stdin;
    }
    strncpy(aptoptions.filename,_APT_AUDIO_STDIN,sizeof(_APT_AUDIO_STDIN)-1);
    if(AptImageSet==2) {
      strncpy(aptoptions.secondfile,_APT_AUDIO_STDIN,sizeof(_APT_AUDIO_STDIN)-1);
    }
    if(fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK) < 0) { // Non-blocking STDIN
      printf("Error calling fcntl, non-blocking stdin\n");
    }
  }
  if(aptoptions.isfile) { // Write to WAV file
    aptoptions.console=0;
    aptoptions.loop=0;
    if(aptoptions.chdev) {
      WAVfile=wav_open(aptoptions.device);
      if(WAVfile==NULL){
        exit(1);
      }
      wav_head(ReadTga.Height/2,WF_SAMPLE_RATE,WF_SAMPLEBITS,WAVfile);
    }
    else {
      printf("Cannot use %s as regular file\n",_APT_AUDIO_DEVICE);
      exit(1);
    }
  }
  else{
    AudioDevice = InitAudioDevice(aptoptions.device);
    if(AudioDevice < 0) {
      exit(1);
    }
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
        if(strncmp(aptoptions.secondfile,_APT_FILE_NO_SET,4) == 0) {
          CloseImageFile(ReadTga.File, RGfile);
        }
        else {
          fclose(ReadTga.File);
          fclose(SecondTga.File);
        }
        exit(0);
      }
      else if(!strncmp(consoleinput,"help",4)){
        printf("help  - print help\n");
        printf("empty - encode empty data (only in single)\n");
        printf("image - encode image data (only in single)\n");
        printf("info  - show information\n");
        printf("load1 - load new first image\n");
        printf("load2 - load new second image (only in multi)\n");
        printf("mode  - channel B data mode (R,G,B,N,Y) (only in single)\n");
        printf("exit  - exit from APT\n") ;	
      }
      else if(!strncmp(consoleinput,"empty",5)) {
        if((ReadTga.File!=NULL)&&(strncmp(aptoptions.secondfile,_APT_FILE_NO_SET,4) == 0)) {
          RGfile = ReadTga.File;
          ReadTga.File = NULL;
        }
      }
      else if(!strncmp(consoleinput,"image",5)) {
        if((RGfile!=NULL)&&(strncmp(aptoptions.secondfile,_APT_FILE_NO_SET,4) == 0)) {
          ReadTga.File = RGfile;
          RGfile = NULL;
        }
      }
      else if(!strncmp(consoleinput,"mode R",6)) {
        aptoptions.datab = 'R';
      }  
      else if(!strncmp(consoleinput,"mode G",6)) {
        aptoptions.datab = 'G';
      }
      else if(!strncmp(consoleinput,"mode B",6)) {
        aptoptions.datab = 'B';
      }
      else if(!strncmp(consoleinput,"mode N",6)) {
        aptoptions.datab = 'N';
      }
      else if(!strncmp(consoleinput,"mode Y",6)) {
        aptoptions.datab = 'Y';
      }
      else if(!strncmp(consoleinput,"mode C",6)) {
        aptoptions.datab = 'C';
      }
      else if(!strncmp(consoleinput,"load1",5)) {
        char newimage[1024];
        printf("New first image filename: ");
        scanf("%s",newimage);
        TgaImageHead NewTgaFile = OpenTgaImage(newimage);
        if(NewTgaFile.File != NULL) { 
          strncpy(aptoptions.filename,newimage,sizeof(newimage)-1);  
          imageline = 0;
          if(AptImageSet==1) {
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
          if(AptImageSet==2){
            fclose(ReadTga.File);
            ReadTga = NewTgaFile;
          }
        }
      }
      else if(!strncmp(consoleinput,"load2",5)) {
        char newimage[1024];
        printf("New second image filename: ");
        scanf("%s",newimage);
        TgaImageHead NewTgaFile = OpenTgaImage(newimage);
        if(NewTgaFile.File != NULL) { 
          strncpy(aptoptions.secondfile,newimage,sizeof(newimage)-1);  
          imageline_second = 0;
          fclose(SecondTga.File);
          SecondTga = NewTgaFile;   
        }
      }
      else if(!strncmp(consoleinput,"info",4)) {
        printf("Sound thread loop time: %lf sec\n",time_taken);
        printf("Output audio device: %s, ",aptoptions.device);
        printf("Sample rate: %d Hz, Bits: %d, Channels: %d\n",WF_SAMPLE_RATE,WF_SAMPLEBITS,WF_CHANNELS);
        printf("Image set: ");
        if(AptImageSet==1) {
          printf("single \n");
        }
        else if (AptImageSet==2) {
          printf("multi \n");
        }
        if(AptImageSet==1) {
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
          printf("Current line %d (%d%%) ",imageline,(int)(100*imageline/ReadTga.Height));
          printf("Channel B data mode %c\n",aptoptions.datab);
        } 
        if(AptImageSet==2) {
          printf("First image:  %s",aptoptions.filename);
          printf("  Width: %dpx, Height: %dpx, ", ReadTga.Width, ReadTga.Height);
          printf("Time to transmit: %d sec\n",ReadTga.Height/2);
          printf("Second image: %s",aptoptions.secondfile);
          printf("  Width: %dpx, Height: %dpx, ", SecondTga.Width, SecondTga.Height);
          printf("Time to transmit: %d sec\n",SecondTga.Height/2);
          printf("Current line %d (%d%%) + ",imageline,(int)(100*imageline/ReadTga.Height));
          printf("%d (%d%%), ",imageline_second,(int)(100*imageline_second/SecondTga.Height));
        }
        printf("Total transmitted frames %d, ",transmitted_frame);
        printf("Total transmitted minutes %d, ",transmitted_minutes);
        printf("Image loops: %d\n",transmitted_images);
      }	
      else {
        printf("Bad command\n");
      }
    }
  }
  else {
    //if(!aptoptions.isfile) {
      while(1) {
        sleep(1);	
      }
    //}
  }
  return 0;
}

void Usage(char *p_name) {
  printf("NOAA automatic picture transmission (APT) encoder\n");
  printf("Usage: %s (-i <file> [-s <file>] | -I) [(-d <device> | -O) -m <mode> -lcrM]\n",p_name);
  printf("  -i <filename> Input TGA image (909px width, 24bit RGB)\n");
  printf("  -s <filename> Second input TGA image (B channel, mode ignored)\n");
  printf("  -d <device>   OSS audio device (default /dev/dsp) or file\n");
  printf("  -m <mode>     Channel B data mode (R,G,B,N,Y,C)\n");
  printf("  -I            Read image data from stdin\n");
  printf("  -O            Write audio data to stdout\n");
  printf("  -M            Multi image reading from stdin\n");
  printf("  -l            Enable infinite image loop\n");
  printf("  -c            Enable user console\n");
  printf("  -r            Device is regular file (write WAV audio file)\n");
  printf("  -h            Show this help\n");
  printf("                Build: %s %s, GCC %s\n", __TIME__, __DATE__, __VERSION__); 
}

void CloseImageFile(FILE *reg, FILE *alt) {
  if(AptImageSet==1) {
    if(reg==NULL) {
      fclose(alt);
    }
    else {
      fclose(reg); 
    }
  }
  else if (AptImageSet==2) {
    fclose(reg);
  }
}

void ClearConsole() {
  rl_free_line_state();
  rl_cleanup_after_signal();
  RL_UNSETSTATE(RL_STATE_ISEARCH|RL_STATE_NSEARCH|RL_STATE_VIMOTION|RL_STATE_NUMERICARG|RL_STATE_MULTIKEY);
  rl_line_buffer[rl_point = rl_end = rl_mark = 0] = 0;
  printf("\n");  
}

void SignalHandler(int number) {
   printf("\nCaught signal %d ... ", number);
   if(AptImageSet==1) {
     CloseImageFile(ReadTga.File, RGfile); 
   }
   else if(AptImageSet==2){
     fclose(ReadTga.File);
   }
   printf("abort\n");
   exit(-1);
}