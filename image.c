#include "main.h"
#include "image.h"
#include "aptcode.h"

int TestTGAImage(FILE *fp){
   TgaImageHead TgaHead;
   // 24 bpp, true color without alfa
   TgaHead.IDLength = 0x00;     //no ID array
   TgaHead.ColorMapType = 0x00; //no palete
   TgaHead.ImageType = 0x02;    //true color
   TgaHead.CMapStart = 0x0000;  //zero
   TgaHead.CMapLength = 0x0000; //zero
   TgaHead.CMapDepth = 0x00;    //zero
   TgaHead.XOffset = 0x0000;    //zero
   TgaHead.YOffset = 0x0000;    //zero
   TgaHead.Width = 0x0100;      //256 pixels
   TgaHead.Height = 0x0100;     //256 pixels
   TgaHead.PixelDepth = 0x18;   // 24 bits per pixel
   TgaHead.ImageDescriptor = 0x20; //up to down
   //write header
   fwrite_int(TgaHead.IDLength,1,fp);
   fwrite_int(TgaHead.ColorMapType,1,fp);
   fwrite_int(TgaHead.ImageType,1,fp);
   fwrite_int(TgaHead.CMapStart,2,fp);
   fwrite_int(TgaHead.CMapLength,2,fp);
   fwrite_int(TgaHead.CMapDepth,1,fp);
   fwrite_int(TgaHead.XOffset,2,fp);
   fwrite_int(TgaHead.YOffset,2,fp);
   fwrite_int(TgaHead.Width,2,fp);
   fwrite_int(TgaHead.Height,2,fp);
   fwrite_int(TgaHead.PixelDepth,1,fp);
   fwrite_int(TgaHead.ImageDescriptor,1,fp);
   // Test image
   int i,j;
   for(i=0;i<256;i++) {
     for(j=0;j<256;j++) {
       WriteTGAPixel(i, i, j, fp);
     }
   }
   return 0;
}

int WriteTGAPixel(uint8_t R, uint8_t G, uint8_t B, FILE *fp){
   uint32_t RR = R << 16; //8 bit
   uint32_t GG = G << 8; //8 bit
   uint32_t BB = B; //8 bit
   uint32_t data = BB + GG + RR;
   return fwrite_int(data,3,fp);
}

unsigned int ReadTGAPixel(FILE *fp){
   return fread_int(3,fp);
}

uint8_t GetRedSubPixel(unsigned int pixel) {
   unsigned int pix = pixel & 0x00FF0000;
   return pix >> 16;
}

uint8_t GetGreenSubPixel(unsigned int pixel) {
   unsigned int pix = pixel & 0x0000FF00;
   return pix >> 8;
}

uint8_t GetBlueSubPixel(unsigned int pixel) {
   unsigned int pix = pixel & 0x000000FF;
   return pix >> 0;
}

int fwrite_int(int val, char len, FILE *p){
    unsigned int byte;
    while (len-- > 0) {
        byte = val & 0xFF;
        fwrite(&byte, 1, 1, p);
        val >>= 8;
    }
    return 0;
}

unsigned int fread_int(char len, FILE *p){
    unsigned int byte=0;
    unsigned int val=0;
    unsigned int pc=0;
    while (len-- > 0) {
        fread(&byte, 1, 1, p);
        val = val + (byte << 8*pc);
        pc++;      
    }
    return val;
}

TgaImageHead OpenTgaImage(char *filename) {
  FILE *filein=NULL;
  char msg[255];
  TgaImageHead TgaImage = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL};
  if((filein = fopen(filename, "rb")) == NULL){
    strcpy(msg, "Error opening ");
    strcat(msg, filename);
    perror(msg);
    return TgaImage;
  }
  // read header   
  TgaImage.IDLength = fread_int(1,filein);     
  TgaImage.ColorMapType = fread_int(1,filein); 
  TgaImage.ImageType = fread_int(1,filein);    
  TgaImage.CMapStart = fread_int(2,filein); 
  TgaImage.CMapLength = fread_int(2,filein); 
  TgaImage.CMapDepth = fread_int(1,filein);   
  TgaImage.XOffset = fread_int(2,filein);   
  TgaImage.YOffset = fread_int(2,filein);   
  TgaImage.Width = fread_int(2,filein);     
  TgaImage.Height = fread_int(2,filein);    
  TgaImage.PixelDepth = fread_int(1,filein);  
  TgaImage.ImageDescriptor = fread_int(1,filein); 
  if (TgaImage.Width != APT_VIDEO_A)  {
    printf("%s : Image width is %d px\n",filename,TgaImage.Width);
    printf("Only %d px width images are supported\n",APT_VIDEO_A);
    fclose(filein);
    return TgaImage;
  }
  TgaImage.File = filein;
  //printf("X: %d, Y:%d Z: %d\n",TgaImage.Width,TgaImage.Height,TgaImage.PixelDepth);
  return TgaImage;  //now ready to read image data
}

TgaImageHead WriteTgaImage(char *filename, TgaImageHead WriteImage) {
  FILE *fileout=NULL;
  char msg[255]; 
  WriteImage.File = NULL;
  if((fileout = fopen(filename, "wb")) == NULL){
    strcpy(msg, "Error opening ");
    strcat(msg, filename);
    perror(msg);
    return WriteImage;
  }
  WriteImage.File = fileout;
  fwrite_int(WriteImage.IDLength,1,fileout);
  fwrite_int(WriteImage.ColorMapType,1,fileout);
  fwrite_int(WriteImage.ImageType,1,fileout);
  fwrite_int(WriteImage.CMapStart,2,fileout);
  fwrite_int(WriteImage.CMapLength,2,fileout);
  fwrite_int(WriteImage.CMapDepth,1,fileout);
  fwrite_int(WriteImage.XOffset,2,fileout);
  fwrite_int(WriteImage.YOffset,2,fileout);
  fwrite_int(WriteImage.Width,2,fileout);
  fwrite_int(WriteImage.Height,2,fileout);
  fwrite_int(WriteImage.PixelDepth,1,fileout);
  fwrite_int(WriteImage.ImageDescriptor,1,fileout);
  return WriteImage;  //now ready to write image data
}

HsvColor RgbToHsv(RgbColor rgb) {
  HsvColor hsv;
  uint8_t rgbMin, rgbMax;
  // Get min/max
  rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
  rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);
  // Value
  hsv.v = rgbMax;
  if (hsv.v == 0) {
    hsv.h = 0;
    hsv.s = 0;
    return hsv;
  }
  // Saturation
  hsv.s = 255 * (long)(rgbMax - rgbMin) / hsv.v;
  if (hsv.s == 0) {
    hsv.h = 0;
    return hsv;
  }
  // Hue
  if (rgbMax == rgb.r) {
    hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
  }
  else if (rgbMax == rgb.g) {
    hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
  }
  else {
    hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);
  }
  return hsv;
}

RgbColor HsvToRgb(HsvColor hsv) {
  RgbColor rgb;
  uint8_t region, remainder, p, q, t;
  // Grayscale
  if (hsv.s == 0) {
    rgb.r = hsv.v;
    rgb.g = hsv.v;
    rgb.b = hsv.v;
    return rgb;
  }
  // 60 degrees region
  region = hsv.h / 43;
  remainder = (hsv.h - (region * 43)) * 6; 

  p = (hsv.v * (255 - hsv.s)) >> 8;
  q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
  t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

  switch (region) {
    case 0:
      rgb.r = hsv.v; rgb.g = t; rgb.b = p;
      break;
    case 1:
      rgb.r = q; rgb.g = hsv.v; rgb.b = p;
      break;
    case 2:
      rgb.r = p; rgb.g = hsv.v; rgb.b = t;
      break;
    case 3:
      rgb.r = p; rgb.g = q; rgb.b = hsv.v;
      break;
    case 4:
      rgb.r = t; rgb.g = p; rgb.b = hsv.v;
      break;
    default:
      rgb.r = hsv.v; rgb.g = p; rgb.b = q;
      break;
  }

  return rgb;
}

AptColor HsvToApt(HsvColor hsv, uint8_t bits) {
  AptColor apt;
  apt.h = hsv.h;
  switch (bits) {
    case 0:
      apt.sv = (hsv.s & 0x00)+(hsv.v >> 0);
      break;
    case 1:
      apt.sv = (hsv.s & 0x80)+(hsv.v >> 1);
      break;
    case 2:
      apt.sv = (hsv.s & 0xC0)+(hsv.v >> 2);
      break;  
    case 4:
      apt.sv = (hsv.s & 0xF0)+(hsv.v >> 4);
      break;
    case 5:
      apt.sv = (hsv.s & 0xF8)+(hsv.v >> 5);
      break;  
    case 6:
      apt.sv = (hsv.s & 0xFC)+(hsv.v >> 6);
      break; 
    case 7:
      apt.sv = (hsv.s & 0xFE)+(hsv.v >> 7);
      break;  
    case 8:
      apt.sv = (hsv.s & 0xFF)+(hsv.v >> 8);
      break; 
    case 3: 
    default:
      apt.sv = (hsv.s & 0xE0)+(hsv.v >> 3);
      break;  
  }
  
  return apt;
}

HsvColor AptToHsv(AptColor apt, uint8_t bits) {
  HsvColor hsv;
  hsv.h = apt.h;
  switch (bits) {
    case 0:
      hsv.s = (apt.sv & 0x00);
      hsv.v = (apt.sv & 0xFF) << 0;
      break;
    case 1:
      hsv.s = (apt.sv & 0x80);
      hsv.v = (apt.sv & 0x7F) << 1; 
      break;
    case 2:
      hsv.s = (apt.sv & 0xC0);
      hsv.v = (apt.sv & 0x3F) << 2;
      break;  
    case 4:
      hsv.s = (apt.sv & 0xF0);
      hsv.v = (apt.sv & 0x0F) << 4;
      break;
    case 5:
      hsv.s = (apt.sv & 0xF8);
      hsv.v = (apt.sv & 0x07) << 5;
      break;
    case 6:
      hsv.s = (apt.sv & 0xFC);
      hsv.v = (apt.sv & 0x03) << 6;
      break;  
    case 7:
      hsv.s = (apt.sv & 0xFE);
      hsv.v = (apt.sv & 0x01) << 7;
      break;  
    case 8:
      hsv.s = (apt.sv & 0xFF);
      hsv.v = (apt.sv & 0x00) << 8;
      break;  
    case 3:     
    default: 
      hsv.s = (apt.sv & 0xE0);
      hsv.v = (apt.sv & 0x1F) << 3;
      break; 
  }
  
  return hsv;
}

AptColor RgbToApt(RgbColor rgb) {
  AptColor apt;
  uint8_t R = (rgb.r >> 4) & 0xF;
  uint8_t G = (rgb.g >> 4) & 0xF;
  uint8_t B = (rgb.b >> 4) & 0xF;
  uint16_t val = (R << 8) + (G << 4) + B;
  uint16_t pos = LUTFromRgb[val];
  uint16_t y_val = pos % 64;
  uint16_t x_val = (pos - y_val) / 64;
  apt.h = (x_val & 0x3F) * 4;
  apt.sv = (y_val & 0x3F) * 4; 
  return apt;
}

RgbColor AptToRgb(AptColor apt) {
  RgbColor rgb;
  uint8_t x_val = (apt.h+2)/4;
  uint8_t y_val = (apt.sv+2)/4;
  if(x_val > 255) {x_val = 255;}
  if(y_val > 255) {y_val = 255;}
  uint16_t val = LUTFromApt[x_val][y_val];
  uint8_t R = (val >> 8) & 0xF;
  uint8_t G = (val >> 4) & 0xF;
  uint8_t B = (val >> 0) & 0xF;
  rgb.r = R * 17; rgb.g = G * 17; rgb.b = B * 17;
  return rgb;
}