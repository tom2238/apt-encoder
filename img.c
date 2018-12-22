#include "main.h"
#include "image.h"
#include "aptcode.h"

int main() {
   printf("Start\n");
   FILE *fp;
   FILE *ts;
   FILE *col;
   FILE *col2;
   FILE *APT;
   //fp = fopen("8bit_pal.tga", "rb");
   
   ts = fopen("fileC1.tga", "wb");
   col = fopen("fileC2.tga", "wb");
   col2 = fopen("fileC3.tga", "wb");
   APT = fopen("fileAPT.tga","wb");
   printf("Write\n");
   //TestTGAImage(fp);
   unsigned int data=0;
   int i,j;
   //fwrite_int(1234567890,4,ts);
   
   
     
   
   TgaImageHead ReadTga = OpenTgaImage("ic.tga");
   if(ReadTga.File == NULL) {
     printf("Error\n");
     exit(1);
   }
   // read header   
   /*ReadTga.IDLength = fread_int(1,fp);     //no ID array
   ReadTga.ColorMapType = fread_int(1,fp); //no palete
   ReadTga.ImageType = fread_int(1,fp);    //true color
   ReadTga.CMapStart = fread_int(2,fp);  //zero
   ReadTga.CMapLength = fread_int(2,fp); //zero
   ReadTga.CMapDepth = fread_int(1,fp);   //zero
   ReadTga.XOffset = fread_int(2,fp);    //zero
   ReadTga.YOffset = fread_int(2,fp);   //zero
   ReadTga.Width = fread_int(2,fp);     //256 pixels
   ReadTga.Height = fread_int(2,fp);    //256 pixels
   ReadTga.PixelDepth = fread_int(1,fp);  // 24 bits per pixel
   ReadTga.ImageDescriptor = fread_int(1,fp); //up to down
   */
   fp = ReadTga.File;
   
   TgaImageHead APTTga;
   //write header
   fwrite_int(ReadTga.IDLength,1,APT);
   fwrite_int(ReadTga.ColorMapType,1,APT);
   fwrite_int(ReadTga.ImageType,1,APT);
   fwrite_int(ReadTga.CMapStart,2,APT);
   fwrite_int(ReadTga.CMapLength,2,APT);
   fwrite_int(ReadTga.CMapDepth,1,APT);
   fwrite_int(ReadTga.XOffset,2,APT);
   fwrite_int(ReadTga.YOffset,2,APT);
   fwrite_int(2080,2,APT);
   fwrite_int(ReadTga.Height,2,APT);
   fwrite_int(ReadTga.PixelDepth,1,APT);
   fwrite_int(ReadTga.ImageDescriptor,1,APT);
   
   AptLine AptTrans;
   uint8_t frame=1;
   uint32_t currentline = 1;
   uint8_t APTdata = 0;
   AptTelemetry TelemetryA = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   AptTelemetry TelemetryB = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   
   for(j=0;j<ReadTga.Height;j++) {
   AptTrans = CreateAptLine(frame, currentline, TelemetryA, TelemetryB, fp);
   frame++;
   currentline++;
   if(frame > 128) {
     frame = 1;
   }                
   for(i=0;i<39;i++) {
     APTdata = AptTrans.SyncA[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<47;i++) {
     APTdata = AptTrans.MarkerA[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<909;i++) {
     APTdata = AptTrans.VideoA[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<45;i++) {
     APTdata = AptTrans.TelemetryA[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<39;i++) {
     APTdata = AptTrans.SyncB[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<47;i++) {
     APTdata = AptTrans.MarkerB[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<909;i++) {
     APTdata = AptTrans.VideoB[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   for(i=0;i<45;i++) {
     APTdata = AptTrans.TelemetryB[i];
     WriteTGAPixel(APTdata, APTdata, APTdata, APT);
   }
   
   }
   
   fseek(fp, 18, SEEK_SET );
   //write header
   fwrite_int(ReadTga.IDLength,1,ts);
   fwrite_int(ReadTga.ColorMapType,1,ts);
   fwrite_int(ReadTga.ImageType,1,ts);
   fwrite_int(ReadTga.CMapStart,2,ts);
   fwrite_int(ReadTga.CMapLength,2,ts);
   fwrite_int(ReadTga.CMapDepth,1,ts);
   fwrite_int(ReadTga.XOffset,2,ts);
   fwrite_int(ReadTga.YOffset,2,ts);
   fwrite_int(ReadTga.Width,2,ts);
   fwrite_int(ReadTga.Height,2,ts);
   fwrite_int(ReadTga.PixelDepth,1,ts);
   fwrite_int(ReadTga.ImageDescriptor,1,ts);
   
   fwrite_int(ReadTga.IDLength,1,col);
   fwrite_int(ReadTga.ColorMapType,1,col);
   fwrite_int(ReadTga.ImageType,1,col);
   fwrite_int(ReadTga.CMapStart,2,col);
   fwrite_int(ReadTga.CMapLength,2,col);
   fwrite_int(ReadTga.CMapDepth,1,col);
   fwrite_int(ReadTga.XOffset,2,col);
   fwrite_int(ReadTga.YOffset,2,col);
   fwrite_int(ReadTga.Width,2,col);
   fwrite_int(ReadTga.Height,2,col);
   fwrite_int(ReadTga.PixelDepth,1,col);
   fwrite_int(ReadTga.ImageDescriptor,1,col);
   
   fwrite_int(ReadTga.IDLength,1,col2);
   fwrite_int(ReadTga.ColorMapType,1,col2);
   fwrite_int(ReadTga.ImageType,1,col2);
   fwrite_int(ReadTga.CMapStart,2,col2);
   fwrite_int(ReadTga.CMapLength,2,col2);
   fwrite_int(ReadTga.CMapDepth,1,col2);
   fwrite_int(ReadTga.XOffset,2,col2);
   fwrite_int(ReadTga.YOffset,2,col2);
   fwrite_int(ReadTga.Width,2,col2);
   fwrite_int(ReadTga.Height,2,col2);
   fwrite_int(ReadTga.PixelDepth,1,col2);
   fwrite_int(ReadTga.ImageDescriptor,1,col2);
   
   printf("X: %d, Y:%d Z: %d\n",ReadTga.Width,ReadTga.Height,ReadTga.PixelDepth);
   
   
   uint8_t gray = 0;
   uint8_t Rval;
   uint8_t Gval;
   uint8_t Bval;
   unsigned int pix;
   for(i=0;i<ReadTga.Width;i++) {
     for(j=0;j<ReadTga.Height;j++) {
       pix = ReadTGAPixel(fp);
       Rval = GetRedSubPixel(pix);
       Gval = GetGreenSubPixel(pix);
       Bval = GetBlueSubPixel(pix);
       gray = Rval*0.302 + Gval*0.59 + Bval*0.11;
       WriteTGAPixel(gray, gray, gray, ts);
       gray = ((Rval / 32) << 5) + ((Gval / 32) << 2) + (Bval/ 64);
       //gray = (Rval * 123/3125) << 5 + (Gval *1339/20000) << 2 + (Bval * 33/2500);
       //WriteTGAPixel( ((Rval / 32))*((gray)/8) , ((Gval/32))*((gray)/8), ((Bval /64))*((gray)/4), col2);
       WriteTGAPixel(gray, gray, gray, col2);
       WriteTGAPixel( ((Rval / 32) << 5) , ((Gval / 32) << 5), (Bval/ 64) << 6, col);
     }
   }
   
 
   
   printf("R: %d, G: %d, B: %d, G: %d\n",Rval,Gval,Bval,gray);
   
   /* for(i=0;i<APT_SYNC_A;i++) {
      Rval = AptSyncA(i);
      Gval = AptSyncB(i);
      printf("%d.: A:%d \t B:%d\n",i,Rval,Gval);
    } */
  
  /* fseek(fp, 0, SEEK_SET);
   fwrite_int(ReadTga.IDLength,1,col);
   fwrite_int(ReadTga.ColorMapType,1,col);
   fwrite_int(ReadTga.ImageType,1,col);
   fwrite_int(ReadTga.CMapStart,2,col);
   fwrite_int(ReadTga.CMapLength,2,col);
   fwrite_int(ReadTga.CMapDepth,1,col);
   fwrite_int(ReadTga.XOffset,2,col);
   fwrite_int(ReadTga.YOffset,2,col);
   fwrite_int(ReadTga.Width,2,col);
   fwrite_int(ReadTga.Height,2,col);
   fwrite_int(ReadTga.PixelDepth,1,col);
   fwrite_int(ReadTga.ImageDescriptor,1,col);
   // Test image
   for(i=0;i<ReadTga.Width;i++) {
     for(j=0;j<ReadTga.Height;j++) {
       pix = ReadTGAPixel(fp);
       Rval = GetRedSubPixel(pix);
       Gval = GetGreenSubPixel(pix);
       Bval = GetBlueSubPixel(pix);
       //gray = (Rval * 7 / 255) << 5 + (Gval * 7 / 255) << 2 + (Bval * 3 / 255);
       gray = Rval*0.302 + Gval*0.59 + Bval*0.11;
       WriteTGAPixel(gray, gray, gray, col);
     }
   } */
   
   printf("End\n");
   fclose(ts);
   fclose(fp);
   fclose(col);
   fclose(col2);
   return 0;
}
