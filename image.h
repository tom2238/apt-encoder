#ifndef NA_IMAGE_R_H_   
#define NA_IMAGE_R_H_

#include "imgtable.h"
#include "imgtable_apt.h"
#define IMG_TGA_HEAD_SIZE 18

typedef struct {
  uint8_t IDLength;
  uint8_t ColorMapType;
  uint8_t ImageType;
  uint16_t CMapStart;
  uint16_t CMapLength;
  uint8_t CMapDepth;
  uint16_t XOffset;
  uint16_t YOffset;
  uint16_t Width;
  uint16_t Height;
  uint8_t PixelDepth;
  uint8_t ImageDescriptor;
  FILE *File;
}TgaImageHead;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
}RgbColor;

typedef struct {
  uint8_t h;
  uint8_t s;
  uint8_t v;
}HsvColor;
 
typedef struct {
  uint8_t h;  // Hue
  uint8_t sv; // Reduced saturation and value
}AptColor;

//Write test image
int TestTGAImage(FILE *fp);
//Write one 24 bit pixel
int WriteTGAPixel(uint8_t R, uint8_t G, uint8_t B, FILE *fp);
//Read one 24bit pixel
unsigned int ReadTGAPixel(FILE *fp);
//Get colours from pixel
uint8_t GetRedSubPixel(unsigned int pixel);
uint8_t GetGreenSubPixel(unsigned int pixel);
uint8_t GetBlueSubPixel(unsigned int pixel);
//Write integer, little endian
int fwrite_int(int val, char len, FILE *p);
//Read integer, little endian
unsigned int fread_int(char len, FILE *p);
//Open image for transmit
TgaImageHead OpenTgaImage(char *filename);
//Write transmitted image
TgaImageHead WriteTgaImage(char *filename, TgaImageHead WriteImage);
//Convert RGB to HSV color space
HsvColor RgbToHsv(RgbColor rgb);
//Convert HSV to RGB color space
RgbColor HsvToRgb(HsvColor hsv);
//Convert HSV to APT color space
AptColor HsvToApt(HsvColor hsv, uint8_t bits);
//Convert APT to HSV color space
HsvColor AptToHsv(AptColor apt, uint8_t bits);
AptColor RgbToApt(RgbColor rgb);
RgbColor AptToRgb(AptColor apt);
#endif 
