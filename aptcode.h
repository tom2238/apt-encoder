#ifndef NOAA_APT_GEN_H_   
#define NOAA_APT_GEN_H_
// A channel
#define APT_SYNC_A 39
#define APT_MARKER_A 47
#define APT_TELEMETRY_A 45
#define APT_VIDEO_A 909
// B channel
#define APT_SYNC_B 39
#define APT_MARKER_B 47
#define APT_TELEMETRY_B 45
#define APT_VIDEO_B 909
// Common
#define APT_CARRIER_F 2400
#define APT_SYNC_LOW 11
#define APT_SYNC_HIGH 244
#define APT_WORD_LEN 1/4160
// SampleRate * AptWordLen ~ 5.769230769 = 75/13
#define APT_WORD_MUL_SYNC 5.75
#define APT_WORD_MUL_MARK 5.75
#define APT_WORD_MUL_VIDEO 5.765
#define APT_WORD_MUL_TELEM 5.75
#define APT_MARKER_LOW 0
#define APT_MARKER_HIGH 255

typedef struct {
  uint8_t Wedge1;
  uint8_t Wedge2;
  uint8_t Wedge3;
  uint8_t Wedge4;
  uint8_t Wedge5;
  uint8_t Wedge6;
  uint8_t Wedge7;
  uint8_t Wedge8;
  uint8_t ZeroModRef;
  uint8_t Temp1;
  uint8_t Temp2;
  uint8_t Temp3;
  uint8_t Temp4;
  uint8_t PatchTemp;
  uint8_t BackScan;
  uint8_t ChannelID;
}AptTelemetry;

typedef struct {
  uint8_t SyncA[39];
  uint8_t MarkerA[47];
  uint8_t VideoA[909];
  uint8_t TelemetryA[45];
  uint8_t SyncB[39];
  uint8_t MarkerB[47];
  uint8_t VideoB[909];
  uint8_t TelemetryB[45];
}AptLine;

//Generate A channel pulses
uint8_t AptSyncA(uint8_t word);
//Generate B channel pulses
uint8_t AptSyncB(uint8_t word);
//Marker A channel
uint8_t AptMarkerA(uint8_t word, uint8_t minute);
//Marker B channel
uint8_t AptMarkerB(uint8_t word, uint8_t minute);
//Create one line of apt, frame is number in 0 to 127, Seperate telemetry info for each channel
AptLine CreateAptLine(uint8_t frame, uint32_t currentline, AptTelemetry ChanA, AptTelemetry ChanB, FILE *image);

#endif 
