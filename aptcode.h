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

enum AptTelemetryData  {
  APT_TEL_WEDGE1 = 31,   // Calibration values
  APT_TEL_WEDGE2 = 63,   //
  APT_TEL_WEDGE3 = 95,   //
  APT_TEL_WEDGE4 = 127,  //
  APT_TEL_WEDGE5 = 159,  //
  APT_TEL_WEDGE6 = 191,  //
  APT_TEL_WEDGE7 = 223,  //
  APT_TEL_WEDGE8 = 255,  //
  APT_TEL_ZEROMODREF = 0, // End calibration
  APT_TEL_TEMP1 = 105,  // Black body temperatures
  APT_TEL_TEMP2 = 105,  //
  APT_TEL_TEMP3 = 105,  //
  APT_TEL_TEMP4 = 105,  // End temperatures
  APT_TEL_PATCHTEMP = 120, // 105 Kelvin
  APT_TEL_BACKSCAN = 150, // IR CH 3,4,5
  APT_TEL_CHANNELID = 31  // Channel 1
};

#define APT_CHANNELID_1  APT_TEL_WEDGE1 // Visible, 0.58 – 0.68 μm 
#define APT_CHANNELID_2  APT_TEL_WEDGE2 // Near Infrared, 0.325 – 1.10 μm 
#define APT_CHANNELID_3A APT_TEL_WEDGE3 // Thermal Infrared, 1.58 – 1.64 μm 
#define APT_CHANNELID_3B APT_TEL_WEDGE6 // Thermal Infrared, 3.55 – 3.93 μm 
#define APT_CHANNELID_4  APT_TEL_WEDGE4 // Thermal Infrared, 10.30 – 11.30 μm 
#define APT_CHANNELID_5  APT_TEL_WEDGE5 // Thermal Infrared, 11.50 – 12.50 μm 

typedef struct {
  uint8_t Value[APT_LINE_SIZE];
}AptLineAr;
// Single or multi image set
uint8_t AptImageSet;
//Generate A channel sync pulses
uint8_t AptSyncA(uint8_t word);
//Generate B channel sync pulses
uint8_t AptSyncB(uint8_t word);
//Marker A channel
uint8_t AptMarkerA(uint8_t word, uint8_t minute);
//Marker B channel
uint8_t AptMarkerB(uint8_t word, uint8_t minute);
//Create one line of apt, frame is number in 0 to 127, Seperate telemetry info for each channel
AptLine CreateAptLine(uint8_t frame, uint8_t currentline, AptTelemetry ChanA, AptTelemetry ChanB, FILE *image, FILE *secimage, uint8_t DataB);
//Concat AptLine to AptLineAr
AptLineAr ConcatAptLine(AptLine Apt);
//Load one image line
AptLineAr AptTransImageLine(uint8_t frame, uint8_t currentline, TgaImageHead firstHead, TgaImageHead secHead, AptTelemetry telemA, AptTelemetry telemB, uint8_t DataB);
//Create telemetry frame
AptTelemetry CreateTelemetry(uint8_t temp1, uint8_t temp2, uint8_t temp3, uint8_t temp4, uint8_t backscan, uint8_t channel);
#endif 
