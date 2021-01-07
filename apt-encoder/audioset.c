#include "main.h"
#include "audioset.h"

int InitAudioDevice(char *device) {
    //Open Sound Device
    if(!strncmp(device,_APT_AUDIO_STDOUT,sizeof(_APT_AUDIO_STDOUT))) {
      return 1;  
    }
    else {
      int fd = open(device, O_WRONLY);
      char msg[512];
      if(fd < 0) {
        snprintf(msg,sizeof(msg),"Open of %s failed",device);
        perror(msg);
        return fd;
      }

      //Configure Sound Device
      ioset(SOUND_PCM_WRITE_BITS,     WF_SAMPLEBITS);
      ioset(SOUND_PCM_WRITE_CHANNELS, WF_CHANNELS);
      ioset(SOUND_PCM_WRITE_RATE,     WF_SAMPLE_RATE);
    
      return fd;
    }
}    
