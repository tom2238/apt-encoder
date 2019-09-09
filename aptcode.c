#include "main.h"
#include "aptcode.h"

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

AptLine CreateAptLine(uint8_t frame, uint8_t currentline, AptTelemetry ChanA, AptTelemetry ChanB, FILE *image, FILE *secimage, uint8_t DataB) {
  //frame, number in range 1 to 128, current line in frame
  //currentline, number in range 1 to 120
  //image is pointer to file, transmitted image
  //ChanA and ChanB are telemetry data
  AptLine NewLine;
  uint8_t minute;
  uint8_t telemetryA=0;
  uint8_t telemetryB=0;
  uint8_t i;
  uint16_t j;
  uint8_t Rval = 0;
  uint8_t Gval = 0;
  uint8_t Bval = 0;
  RgbColor pixel_r;
  AptColor pixel_a;
  unsigned int pix;
  // Sync A and Sync B
  for(i=0;i<APT_SYNC_A;i++) {
    NewLine.SyncA[i] = AptSyncA(i);
    NewLine.SyncB[i] = AptSyncB(i);
  } 
  // Video A and Video B
  if(AptImageSet == 1) {
    for(j=0;j<APT_VIDEO_A;j++) {
      if(image==NULL) {
        NewLine.VideoA[j] = 0;
        NewLine.VideoB[j] = 0;
      }
      else {
        pix = ReadTGAPixel(image);
        Rval = GetRedSubPixel(pix);
        Gval = GetGreenSubPixel(pix);
        Bval = GetBlueSubPixel(pix);
        if(DataB!='C') { 
          NewLine.VideoA[j] = Rval*0.302 + Gval*0.59 + Bval*0.11;
        }
        switch(DataB) {
          case 'R': //Red
            NewLine.VideoB[j] = Rval;
            break;
          case 'G': //Green
            NewLine.VideoB[j] = Gval;
            break;
          case 'B': //Blue
            NewLine.VideoB[j] = Bval;
            break;
          case 'Y': //Yb
            NewLine.VideoB[j] = (-38*Rval-74*Gval+112*Bval+128)/256 + 128; 
            break;
          case 'C': //Color
            pixel_r.r = Rval;
            pixel_r.g = Gval;
            pixel_r.b = Bval;
            pixel_a = RgbToApt(pixel_r);
            NewLine.VideoA[j] = pixel_a.h;
            NewLine.VideoB[j] = pixel_a.sv;
            break;  
          case 'N':  
          default : //Negative
            NewLine.VideoB[j] = 255-NewLine.VideoA[j];
        }      
      }
    } 
  }
  else if (AptImageSet == 2) {
    for(j=0;j<APT_VIDEO_A;j++) {
      if(image==NULL) {
        NewLine.VideoA[j] = 0;
      }
      else {
        pix = ReadTGAPixel(image);
        Rval = GetRedSubPixel(pix);
        Gval = GetGreenSubPixel(pix);
        Bval = GetBlueSubPixel(pix);
        NewLine.VideoA[j] = Rval*0.302 + Gval*0.59 + Bval*0.11;
      }  
    }
    for(j=0;j<APT_VIDEO_B;j++) {
      if(secimage==NULL) {
        NewLine.VideoB[j] = 0;
      }
      else { 
        pix = ReadTGAPixel(secimage); 
        Rval = GetRedSubPixel(pix);
        Gval = GetGreenSubPixel(pix);
        Bval = GetBlueSubPixel(pix);
        NewLine.VideoB[j] = Rval*0.302 + Gval*0.59 + Bval*0.11;
      }
    }
  }
  // Marker A and Marker B
  minute = currentline % APT_MARKER_SIZE;
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
//Concat AptLine to one array
AptLineAr ConcatAptLine(AptLine Apt) {
   int i,j=0;
   AptLineAr AptAr;
   for(i=0;i<APT_SYNC_A;i++) {
     AptAr.Value[j] = Apt.SyncA[i];
     j++;
   }
   for(i=0;i<APT_MARKER_A;i++) {
     AptAr.Value[j] = Apt.MarkerA[i];
     j++;
   }  
   for(i=0;i<APT_VIDEO_A;i++) {
     AptAr.Value[j] = Apt.VideoA[i];
     j++;
   }  
   for(i=0;i<APT_TELEMETRY_A;i++) {
     AptAr.Value[j] = Apt.TelemetryA[i];
     j++;
   }  
   for(i=0;i<APT_SYNC_B;i++) {
     AptAr.Value[j] = Apt.SyncB[i];
     j++;
   } 
   for(i=0;i<APT_MARKER_B;i++) {
     AptAr.Value[j] = Apt.MarkerB[i];
     j++;
   }  
   for(i=0;i<APT_VIDEO_B;i++) {
     AptAr.Value[j] = Apt.VideoB[i];
     j++;
   } 
   for(i=0;i<APT_TELEMETRY_B;i++) {
     AptAr.Value[j] = Apt.TelemetryB[i];
     j++;
   } 
   return AptAr;
}

AptLineAr AptTransImageLine(uint8_t frame, uint8_t currentline, TgaImageHead firstHead, TgaImageHead secHead, AptTelemetry telemA, AptTelemetry telemB, uint8_t DataB){
  AptLineAr result;
  AptLine AptTrans;
  
  AptTrans = CreateAptLine(frame, currentline, telemA, telemB, firstHead.File, secHead.File, DataB);			
  result = ConcatAptLine(AptTrans);
  return result;	
}

AptTelemetry CreateTelemetry(uint8_t temp1, uint8_t temp2, uint8_t temp3, uint8_t temp4, uint8_t backscan, uint8_t channel) {
  AptTelemetry result;
  result.Wedge1 = APT_TEL_WEDGE1;
  result.Wedge2 = APT_TEL_WEDGE2;
  result.Wedge3 = APT_TEL_WEDGE3;
  result.Wedge4 = APT_TEL_WEDGE4;
  result.Wedge5 = APT_TEL_WEDGE5;
  result.Wedge6 = APT_TEL_WEDGE6;
  result.Wedge7 = APT_TEL_WEDGE7;
  result.Wedge8 = APT_TEL_WEDGE8;
  result.ZeroModRef = APT_TEL_ZEROMODREF;
  result.Temp1 = temp1;
  result.Temp2 = temp2;
  result.Temp3 = temp3;
  result.Temp4 = temp4;
  result.BackScan = backscan;
  result.ChannelID = channel;
  result.PatchTemp = APT_TEL_PATCHTEMP;
  return result;
}
