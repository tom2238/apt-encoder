#include "main.h"
#include "audioset.h"

int InitAudioDevice(void) {
    //Open Sound Device
    int fd = open("/dev/dsp", O_WRONLY);
    if(fd < 0) {
        perror("open of /dev/dsp failed");
        return fd;
    }

    //Configure Sound Device
    ioset(SOUND_PCM_WRITE_BITS,     WF_SAMPLEBITS);
    ioset(SOUND_PCM_WRITE_CHANNELS, WF_CHANNELS);
    ioset(SOUND_PCM_WRITE_RATE,     WF_SAMPLE_RATE);
    
    return fd;
}    
