#include "main.h"
#include "image.h"
#include "aptcode.h"

int main() {
   printf("Start\n");
   //FILE *fp;
   //FILE *ts;
   //FILE *col;
   //FILE *col2;
   //FILE *APT;
   
   //fp = fopen("8bit_pal.tga", "rb");   
   //ts = fopen("fileC1.tga", "wb");
   //col = fopen("fileC2.tga", "wb");
   //col2 = fopen("fileC3.tga", "wb");
   //APT = fopen("fileAPT.tga","wb");
   printf("Write\n");
   //TestTGAImage(fp);
   unsigned int data=0;
   int i,j;
   
   TgaImageHead ReadTga = OpenTgaImage("169049.tga");
   if(ReadTga.File == NULL) {
     printf("Error\n");
     exit(1);
   }
   
   TgaImageHead APTTga;
   APTTga = ReadTga;
   APTTga.Width = APT_LINE_SIZE;
   APTTga = WriteTgaImage("fileAPT.tga", APTTga);
   if(APTTga.File==NULL) {printf("Error\n"); exit(1);}
      
   AptLine AptTrans;
   uint8_t frame=1;
   uint32_t currentline = 1;
   uint8_t APTdata = 0;
   AptLineAr ConvLine;
   AptTelemetry TelemetryA = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   AptTelemetry TelemetryB = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   
   for(j=0;j<ReadTga.Height;j++) {
     AptTrans = CreateAptLine(frame, currentline, TelemetryA, TelemetryB, ReadTga.File);
     frame++;
     currentline++;
     if(frame > APT_FRAME_SIZE) {
       frame = 1;
     }                
     ConvLine = ConcatAptLine(AptTrans);
     for(i=0;i<APT_LINE_SIZE;i++) {
       APTdata = ConvLine.Value[i];
       WriteTGAPixel(APTdata, APTdata, APTdata, APTTga.File);
     }
   }
   
   
   fseek(ReadTga.File, IMG_TGA_HEAD_SIZE, SEEK_SET);
   //write header
   //ts = fopen("fileC1.tga", "wb");
   TgaImageHead TGAC1, TGAC2, TGAC3;
   TGAC1 = ReadTga;
   TGAC2 = ReadTga;
   TGAC3 = ReadTga;
   TGAC1 = WriteTgaImage("fileC1.tga", TGAC1);
   if(TGAC1.File==NULL) {printf("ErrorC1\n"); exit(1);}
   TGAC2 = WriteTgaImage("fileC2.tga", TGAC2);
   if(TGAC2.File==NULL) {printf("ErrorC2\n"); exit(1);}
   TGAC3 = WriteTgaImage("fileC3.tga", TGAC3);
   if(TGAC3.File==NULL) {printf("ErrorC3\n"); exit(1);}
  
   printf("X: %d, Y:%d Z: %d\n",ReadTga.Width,ReadTga.Height,ReadTga.PixelDepth);
   
   
   uint8_t gray = 0;
   uint8_t Rval;
   uint8_t Gval;
   uint8_t Bval;
   unsigned int pix;
   for(i=0;i<ReadTga.Width;i++) {
     for(j=0;j<ReadTga.Height;j++) {
       pix = ReadTGAPixel(ReadTga.File);
       Rval = GetRedSubPixel(pix);
       Gval = GetGreenSubPixel(pix);
       Bval = GetBlueSubPixel(pix);
       gray = Rval*0.302 + Gval*0.59 + Bval*0.11;
       WriteTGAPixel(gray, gray, gray, TGAC1.File);
       gray = ((Rval / 32) << 5) + ((Gval / 32) << 2) + (Bval/ 64);
       //gray = (Rval * 123/3125) << 5 + (Gval *1339/20000) << 2 + (Bval * 33/2500);
       //WriteTGAPixel( ((Rval / 32))*((gray)/8) , ((Gval/32))*((gray)/8), ((Bval /64))*((gray)/4), TGAC3.File);
       WriteTGAPixel(gray, gray, gray, TGAC3.File);
       WriteTGAPixel( ((Rval / 32) << 5) , ((Gval / 32) << 5), (Bval/ 64) << 6, TGAC2.File);
     }
   }
   
 
   
   printf("R: %d, G: %d, B: %d, G: %d\n",Rval,Gval,Bval,gray);   
  
  
   printf("End\n");
   
   fclose(TGAC1.File);
   fclose(APTTga.File);
   fclose(TGAC2.File);
   fclose(TGAC3.File);
   return 0;
}
