#include "main.h"
#include "image.h"
#include "aptcode.h"

#define IMG_COL_STEP 4.2666666

void CallColorTable();

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
   
   TgaImageHead ReadTga = OpenTgaImage("duha.tga");
   if(ReadTga.File == NULL) {
     printf("Error\n");
     exit(1);
   }
   
   TgaImageHead APTTga,SecTga,ThirdTga;
   APTTga = ReadTga;  
   SecTga = ReadTga;
   ThirdTga = ReadTga;
   APTTga = WriteTgaImage("file_col_1.tga", APTTga);
   if(APTTga.File==NULL) {printf("Error\n"); exit(1);}
   SecTga = WriteTgaImage("file_col_2.tga", SecTga);
   if(SecTga.File==NULL) {printf("Error\n"); exit(1);}
   ThirdTga = WriteTgaImage("file_col_3.tga", ThirdTga);
   if(ThirdTga.File==NULL) {printf("Error\n"); exit(1);}
      
   AptLine AptTrans;
   uint8_t frame=1;
   uint32_t currentline = 1;
   uint8_t APTdata = 0;
   AptLineAr ConvLine;
   AptTelemetry TelemetryA = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   AptTelemetry TelemetryB = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   
   /*for(j=0;j<ReadTga.Height;j++) {
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
   }*/
   
   
   //fseek(ReadTga.File, IMG_TGA_HEAD_SIZE, SEEK_SET);
   //write header
   //ts = fopen("fileC1.tga", "wb");
   /*TgaImageHead TGAC1, TGAC2, TGAC3;
   TGAC1 = ReadTga;
   TGAC2 = ReadTga;
   TGAC3 = ReadTga;
   TGAC1 = WriteTgaImage("fileC1.tga", TGAC1);
   if(TGAC1.File==NULL) {printf("ErrorC1\n"); exit(1);}
   TGAC2 = WriteTgaImage("fileC2.tga", TGAC2);
   if(TGAC2.File==NULL) {printf("ErrorC2\n"); exit(1);}
   TGAC3 = WriteTgaImage("fileC3.tga", TGAC3);
   if(TGAC3.File==NULL) {printf("ErrorC3\n"); exit(1);}*/
  
   printf("X: %d, Y:%d Z: %d\n",ReadTga.Width,ReadTga.Height,ReadTga.PixelDepth);
   
   uint8_t gray = 0;
   uint8_t color = 0;
   uint8_t Rval;
   uint8_t Gval;
   uint8_t Bval;
   RgbColor pixel;
   HsvColor hsvcol;
   AptColor aptc;
   unsigned int pix;
   for(i=0;i<ReadTga.Width;i++) {
     for(j=0;j<ReadTga.Height;j++) {
       pix = ReadTGAPixel(ReadTga.File);
       Rval = GetRedSubPixel(pix)/1;
       Gval = GetGreenSubPixel(pix)/1;
       Bval = GetBlueSubPixel(pix)/1;
       pixel.r = Rval;
       pixel.g = Gval;
       pixel.b = Bval;
       gray = Rval*0.302 + Gval*0.59 + Bval*0.11;
       color = Rval/4+Gval/2+Bval/4;
       
       hsvcol = RgbToHsv(pixel); // To HSV
       aptc = HsvToApt(hsvcol,3);  // To APT
       hsvcol = AptToHsv(aptc,3);  // To HSV
       pixel = HsvToRgb(hsvcol); // TO RGB
       
       WriteTGAPixel(hsvcol.h,hsvcol.h,hsvcol.h, APTTga.File);
       WriteTGAPixel(aptc.sv,aptc.sv,aptc.sv, SecTga.File);
       WriteTGAPixel(pixel.r,pixel.g,pixel.b, ThirdTga.File);
       //gray = ((Rval / 32) << 5) + ((Gval / 32) << 2) + (Bval/ 64);
       //gray = (Rval * 123/3125) << 5 + (Gval *1339/20000) << 2 + (Bval * 33/2500);
       //WriteTGAPixel( ((Rval / 32))*((gray)/8) , ((Gval/32))*((gray)/8), ((Bval /64))*((gray)/4), TGAC3.File);
       //WriteTGAPixel(gray, gray, gray, TGAC3.File);
       //WriteTGAPixel( ((Rval / 32) << 5) , ((Gval / 32) << 5), (Bval/ 64) << 6, TGAC2.File);
     }
   }
   
 
   
   printf("R: %d, G: %d, B: %d, G: %d\n",Rval,Gval,Bval,gray);  
  
   //CallColorTable();
   printf("End\n");
   
   //fclose(TGAC1.File);
   fclose(APTTga.File);
   //fclose(TGAC2.File);
   //fclose(TGAC3.File);
   return 0;
}

void CallColorTable() {
  float R=255;
  float G=0;
  float B=0;
  uint16_t radius=0;
  int i;
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    G += IMG_COL_STEP;
    if(G>255) {G=255;}
    radius++;
  }
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    R -= IMG_COL_STEP;
    if(R<0) {R=0;}
    radius++;
  }
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    B += IMG_COL_STEP;
    if(B>255) {B=255;}
    radius++;
  }
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    G -= IMG_COL_STEP;
    if(G<0) {G=0;}
    radius++;
  }
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    R += IMG_COL_STEP;
    if(R>255) {R=255;}
    radius++;
  }
  for(i=0;i<60;i++) {
    printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    B -= IMG_COL_STEP;
    if(B<0) {B=0;}
    radius++;
  }

}

