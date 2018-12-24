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
    printf("Image width is %d px\n",TgaImage.Width);
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
