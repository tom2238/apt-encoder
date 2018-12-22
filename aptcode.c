#include "main.h"
#include "aptcode.h"
#include "image.h"

uint8_t AptSyncA(uint8_t word) {
  //word in range 0 to 38 , total 39 values 
  if((word==4) || (word==5) || (word==8) || (word==9) || (word==12) || (word==13) || (word==16)) {
    return APT_SYNC_HIGH;
  } 
  else if((word==17) || (word==20) || (word==21) || (word==24) || (word==25) || (word==28) || (word==29)) {
    return APT_SYNC_HIGH;
  }
  else {
    return APT_SYNC_LOW;
  }
}

uint8_t AptSyncB(uint8_t word) {
  //word in range 0 to 38 , total 39 values
  if((word==4) || (word==5) || (word==6) || (word==9) || (word==10) || (word==11) || (word==14)) {
    return APT_SYNC_HIGH;
  } 
  else if((word==15) || (word==16) || (word==19) || (word==20) || (word==21) || (word==24) || (word==25)) {
    return APT_SYNC_HIGH;
  } 
  else if((word==26) || (word==29) || (word==30) || (word==31) || (word==34) || (word==35) || (word==36)) {
    return APT_SYNC_HIGH;
  } 
  else {
    return APT_SYNC_LOW;
  } 
}

uint8_t AptMarkerA(uint8_t word, uint8_t minute) {
  //word in range 0 to 46 , total 47 values
  //minute is O no mark, 1 mark
  if(minute==1) {
    return APT_MARKER_HIGH;
  }
  else {
    return APT_MARKER_LOW;
  }
}

uint8_t AptMarkerB(uint8_t word, uint8_t minute) {
  //word in range 0 to 46 , total 47 values
  //minute is O no mark, 1 mark
  if(minute==1) {
    return APT_MARKER_LOW;
  }
  else {
    return APT_MARKER_HIGH;
  }
}

AptLine CreateAptLine(uint8_t frame, uint32_t currentline, AptTelemetry ChanA, AptTelemetry ChanB, FILE *image) {
  //frame, number in range 1 to 128, current line in frame
  //currentline, number in range 1 to inf
  //image is pointer to file, transmitted image
  AptLine NewLine;
  uint8_t minute;
  uint8_t telemetryA;
  uint8_t telemetryB;
  uint8_t i;
  uint16_t j;
  uint8_t gray = 0;
  uint8_t Rval = 0;
  uint8_t Gval = 0;
  uint8_t Bval = 0;
  unsigned int pix;
  // Sync A and Sync B
  for(i=0;i<APT_SYNC_A;i++) {
    NewLine.SyncA[i] = AptSyncA(i);
    NewLine.SyncB[i] = AptSyncB(i);
  } 
  // Video A and Video B
  for(j=0;j<APT_VIDEO_A;j++) {
    pix = ReadTGAPixel(image);
    Rval = GetRedSubPixel(pix);
    Gval = GetGreenSubPixel(pix);
    Bval = GetBlueSubPixel(pix);
    gray = Rval*0.302 + Gval*0.59 + Bval*0.11;
    NewLine.VideoA[j] = gray;
    gray = ((Rval / 32) << 5) + ((Gval / 32) << 2) + (Bval/ 64);
    NewLine.VideoB[j] = gray; 
  }
  // Marker A and Marker B
  minute = currentline % 120;
  if(minute==0 || minute==1) {
    minute = 1;
  }
  else {
    minute = 0;
  }  
  for(i=0;i<APT_MARKER_A;i++) {
    NewLine.MarkerA[i] = AptMarkerA(i,minute);
    NewLine.MarkerB[i] = AptMarkerB(i,minute);
  }
  // Telemetry A and Telemetry B
  if((frame > 0) && (frame <= 8)) {
    telemetryA = ChanA.Wedge1;
    telemetryB = ChanB.Wedge1;
  }
  else if((frame > 8) && (frame <= 16)) {
    telemetryA = ChanA.Wedge2;
    telemetryB = ChanB.Wedge2;
  }
  else if((frame > 16) && (frame <= 24)) {
    telemetryA = ChanA.Wedge3;
    telemetryB = ChanB.Wedge3;
  }
  else if((frame > 24) && (frame <= 32)) {
    telemetryA = ChanA.Wedge4;
    telemetryB = ChanB.Wedge4;
  }
  else if((frame > 32) && (frame <= 40)) {
    telemetryA = ChanA.Wedge5;
    telemetryB = ChanB.Wedge5;
  }
  else if((frame > 40) && (frame <= 48)) {
    telemetryA = ChanA.Wedge6;
    telemetryB = ChanB.Wedge6;
  }
  else if((frame > 48) && (frame <= 56)) {
    telemetryA = ChanA.Wedge7;
    telemetryB = ChanB.Wedge7;
  }
  else if((frame > 56) && (frame <= 64)) {
    telemetryA = ChanA.Wedge8;
    telemetryB = ChanB.Wedge8;
  }
  else if((frame > 64) && (frame <= 72)) {
    telemetryA = ChanA.ZeroModRef;
    telemetryB = ChanB.ZeroModRef;
  }
  else if((frame > 72) && (frame <= 80)) {
    telemetryA = ChanA.Temp1;
    telemetryB = ChanB.Temp1;
  }
  else if((frame > 80) && (frame <= 88)) {
    telemetryA = ChanA.Temp2;
    telemetryB = ChanB.Temp2;
  }
  else if((frame > 88) && (frame <= 96)) {
    telemetryA = ChanA.Temp3;
    telemetryB = ChanB.Temp3;
  }
  else if((frame > 96) && (frame <= 104)) {
    telemetryA = ChanA.Temp4;
    telemetryB = ChanB.Temp4;
  }
  else if((frame > 104) && (frame <= 112)) {
    telemetryA = ChanA.PatchTemp;
    telemetryB = ChanB.PatchTemp;
  }
  else if((frame > 112) && (frame <= 120)) {
    telemetryA = ChanA.BackScan;
    telemetryB = ChanB.BackScan;
  }
  else if((frame > 120) && (frame <= 128)) {
    telemetryA = ChanA.ChannelID;
    telemetryB = ChanB.ChannelID;
  }
  for(i=0;i<APT_TELEMETRY_A;i++){
    NewLine.TelemetryA[i]=telemetryA;
    NewLine.TelemetryB[i]=telemetryB;
  }
  return NewLine;
}
