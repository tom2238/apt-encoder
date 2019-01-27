#ifndef NOAA_APT_GEN_H_   
#define NOAA_APT_GEN_H_
#include "image.h"
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
#define APT_WORD_MUL 5.7696
#define APT_MARKER_LOW 0
#define APT_MARKER_HIGH 255
#define APT_LINE_SIZE 2080
#define APT_FRAME_SIZE 128
#define APT_MARKER_SIZE 120

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
  uint8_t SyncA[APT_SYNC_A];
  uint8_t MarkerA[APT_MARKER_A];
  uint8_t VideoA[APT_VIDEO_A];
  uint8_t TelemetryA[APT_TELEMETRY_A];
  uint8_t SyncB[APT_SYNC_B];
  uint8_t MarkerB[APT_MARKER_B];
  uint8_t VideoB[APT_VIDEO_B];
  uint8_t TelemetryB[APT_TELEMETRY_B];
}AptLine;

typedef struct {
  uint8_t Value[APT_LINE_SIZE];
}AptLineAr;

//Generate A channel sync pulses
uint8_t AptSyncA(uint8_t word);
//Generate B channel sync pulses
uint8_t AptSyncB(uint8_t word);
//Marker A channel
uint8_t AptMarkerA(uint8_t word, uint8_t minute);
//Marker B channel
uint8_t AptMarkerB(uint8_t word, uint8_t minute);
//Create one line of apt, frame is number in 0 to 127, Seperate telemetry info for each channel
AptLine CreateAptLine(uint8_t frame, uint8_t currentline, AptTelemetry ChanA, AptTelemetry ChanB, FILE *image, uint8_t DataB);
//Concat AptLine to AptLineAr
AptLineAr ConcatAptLine(AptLine Apt);
//Load one image line
AptLineAr AptTransImageLine(uint8_t frame, uint8_t currentline, TgaImageHead tgahead, AptTelemetry telemA, AptTelemetry telemB, uint8_t DataB);

#endif 
