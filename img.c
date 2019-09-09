#include "main.h"
#include "image.h"
#include "aptcode.h"
#include "imgtable.h"
#include "imgtable_apt.h"
#include <math.h>
#define IMG_COL_STEP 4.2666666
#define IMG_COL_STEP_42 6.071428
#define IMG_COL_STEP_43 5.930232

typedef struct {
  uint8_t x[16];
  uint8_t y[16];
}ColorMap;

typedef struct {
  uint8_t x[16];
  uint8_t y[4];
}ColorMap16;

void CallColorTable(FILE *zf, int16_t luma);

float ctruncate(float value);
void OpenTgaRaw();
void LUTToApt();
RgbColor From2DApt(AptColor apt);
void TestRGB();
void WriteGLine(ColorMap16 cmap, uint8_t B,FILE *zp);
void Write4CLine(ColorMap Square2D, uint8_t Bcol, FILE *zp);


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
   //unsigned int data=0;
   int i,j;
   
   TgaImageHead ReadTga = OpenTgaImage("139.tga");
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
   
   //AptLine AptTrans;
   //uint8_t frame=1;
   //uint32_t currentline = 1;
   //uint8_t APTdata = 0;
   //AptLineAr ConvLine;
   //AptTelemetry TelemetryA = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   //AptTelemetry TelemetryB = {16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 255};
   
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
   uint16_t mod = 197 % 64;
   uint16_t mod_s =  (197 - mod) / 64;
   printf("197 mod 64 = %d, %d\n",mod,mod_s);
   printf("X: %d, Y:%d Z: %d\n",ReadTga.Width,ReadTga.Height,ReadTga.PixelDepth);
   
   uint8_t gray = 0;
   uint8_t Rval;
   uint8_t Gval;
   uint8_t Bval;
   RgbColor pixel,testpix;
   HsvColor hsvcol;
   AptColor aptc;
   uint8_t LUT[2][2] = {
     1,2,
     3,4
   };
   for(i=0;i<2;i++) {
     for(j=0;j<2;j++) {
       printf("%d\n",LUT[i][j]);
     }
   }
   
       
   printf("%d:%d:%d\n",hsvcol.h,hsvcol.s,hsvcol.v);
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
       //gray = Rval/3+Gval/3+Bval/3;
       
       //hsvcol = RgbToHsv(pixel); // To HSV
       //hsvcol.s = 128;
       //hsvcol.v = 255;
       //gray = hsvcol.v;
       aptc = RgbToApt(pixel);
       //pixel = AptToRgb(aptc);
       //hsvcol = AptToHsv(aptc,3);  // To HSV
       //pixel = HsvToRgb(hsvcol); // TO RGB
       
       WriteTGAPixel(aptc.h,aptc.h,aptc.h, APTTga.File);
       WriteTGAPixel(aptc.sv,aptc.sv,aptc.sv, SecTga.File);
       WriteTGAPixel((pixel.r & 0xE0)*255/224,(pixel.g & 0xE0)*255/224 , (pixel.b & 0xC0)*255/192, ThirdTga.File);
       //gray = ((Rval / 32) << 5) + ((Gval / 32) << 2) + (Bval/ 64);
       //gray = (Rval * 123/3125) << 5 + (Gval *1339/20000) << 2 + (Bval * 33/2500);
       //WriteTGAPixel( ((Rval / 32))*((gray)/8) , ((Gval/32))*((gray)/8), ((Bval /64))*((gray)/4), TGAC3.File);
       //WriteTGAPixel(gray, gray, gray, TGAC3.File);
       //WriteTGAPixel( ((Rval / 32) << 5) , ((Gval / 32) << 5), (Bval/ 64) << 6, TGAC2.File);
     }
   }
   
  /*for(i=0;i<ReadTga.Height;i++) {
    CallColorTable(SecTga.File,i*2-255);
    CallColorTable(SecTga.File,i*2-255);
    CallColorTable(SecTga.File,i*2-255);
    for(j=0;j<141;j++) {
      WriteTGAPixel(0,0,0, SecTga.File); 
    }
  }*/
   
   printf("R: %d, G: %d, B: %d, G: %d\n",Rval,Gval,Bval,gray);  
  
   //CallColorTable();
   printf("End\n");
   //TestRGB();
   //fclose(TGAC1.File);
   fclose(APTTga.File);
   //fclose(TGAC2.File);
   //fclose(TGAC3.File);
   //OpenTgaRaw();
   //LUTToApt();
   return 0;
}

void CallColorTable(FILE *zf, int16_t luma) {
  float R=255;
  float G=0;
  float B=0;
  uint16_t radius=0;
  int i;
  for(i=0;i<42;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    G += IMG_COL_STEP_42;
    if(G>255) {G=255;}
    radius++;
  }
  for(i=0;i<43;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    R -= IMG_COL_STEP_43;
    if(R<0) {R=0;}
    radius++;
  }
  for(i=0;i<43;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    B += IMG_COL_STEP_43;
    if(B>255) {B=255;}
    radius++;
  }
  for(i=0;i<42;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    G -= IMG_COL_STEP_42;
    if(G<0) {G=0;}
    radius++;
  }
  for(i=0;i<43;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    R += IMG_COL_STEP_43;
    if(R>255) {R=255;}
    radius++;
  }
  for(i=0;i<43;i++) {
    //printf("%d:%d,%d,%d\n",radius,(uint8_t)R,(uint8_t)G,(uint8_t)B);
    WriteTGAPixel((uint8_t)ctruncate(R+luma)&0xF0,(uint8_t)ctruncate(G+luma)&0xF0,(uint8_t)ctruncate(B+luma)&0xF0, zf);
    B -= IMG_COL_STEP_43;
    if(B<0) {B=0;}
    radius++;
  }

}

float ctruncate(float value) {
    if(value < 0) return 0;
    if(value > 255) return 255;

    return value;
}

void OpenTgaRaw() {
  FILE *fp;
  FILE *zp;
  zp = fopen("test-space-4k-moved.tga","rb");
  fp = fopen("imgtable.h","wt");
  if(fp==NULL) {
    return;
  }
  if(zp==NULL) {
    return;
  }
  int i,j;
  uint8_t Rval,Gval,Bval;
  unsigned int pix, cval;
  fseek(zp,IMG_TGA_HEAD_SIZE,SEEK_SET);
  fputs("#ifndef NA_IMAGE_TABLE_H_ \n", fp);
  fputs("#define NA_IMAGE_TABLE_H_ \n", fp);
  fputs("#define IMG_LUT_WIDTH 64 \n", fp);
  fputs("#define IMG_LUT_HEIGHT 64 \n", fp);
  fputs("static const uint16_t LUTFromApt[64][64] = {\n", fp);
  for(i=0;i<64;i++) {
    for(j=0;j<64;j++) {
      pix = ReadTGAPixel(zp);
      Rval = GetRedSubPixel(pix) & 0xF;
      Gval = GetGreenSubPixel(pix) & 0xF;
      Bval = GetBlueSubPixel(pix) & 0xF;
      cval = (Rval << 8)+(Gval << 4)+Bval;
      if(i==63 && j==63) {
        fprintf(fp,"%d",cval);
      }
      else {
        fprintf(fp,"%d,",cval);
      }
    }
    fputs("\n",fp);
  }
  
  fputs("};\n", fp);
  fputs("#endif\n", fp);
  fclose(zp);
  fclose(fp);
}

void LUTToApt() {
  FILE *fp;
  FILE *zp;
  zp = fopen("test-space-4k-moved.tga","rb");
  fp = fopen("imgtable_apt.h","wt");
  if(fp==NULL) {
    return;
  }
  if(zp==NULL) {
    return;
  }
  int i,j;
  uint8_t Rval,Gval,Bval;
  unsigned int pix, cval,val;
  uint16_t RGBArray[4096];
  uint16_t counter = 0;
  fseek(zp,IMG_TGA_HEAD_SIZE,SEEK_SET);
  fputs("#ifndef NA_IMAGE_RGB_TABLE_H_ \n", fp);
  fputs("#define NA_IMAGE_RGB_TABLE_H_ \n", fp);
  fputs("#define IMG_LUT_RGB_WIDTH 1 \n", fp);
  fputs("#define IMG_LUT_RGB_HEIGHT 4096 \n", fp);
  fputs("static const uint16_t LUTFromRgb[4096] = {\n", fp);
  for(i=0;i<64;i++) {
    for(j=0;j<64;j++) {
      pix = ReadTGAPixel(zp);
      Rval = GetRedSubPixel(pix) & 0xF;
      Gval = GetGreenSubPixel(pix) & 0xF;
      Bval = GetBlueSubPixel(pix) & 0xF;
      cval = (Rval << 8)+(Gval << 4)+Bval;
      RGBArray[cval]=counter;
      counter++;
    }
  }
  
  for(i=0;i<4096;i++) {
    if(i==4095) {
      fprintf(fp,"%d",RGBArray[i]);
    }
    else {
      fprintf(fp,"%d,",RGBArray[i]);
    }
  }

  fputs("\n",fp);
  fputs("};\n", fp);
  fputs("#endif\n", fp);
  fclose(zp);
  fclose(fp);
}

RgbColor From2DApt(AptColor apt) {
  RgbColor rgb;
  uint8_t x = apt.h;
  uint8_t y = apt.sv;
  unsigned int pix;
  //unsigned int pix = LUTFromApt[x][y];
  uint8_t r = (pix >> 16) & 0xFF;
  uint8_t g = (pix >> 8) & 0xFF;
  uint8_t b = pix & 0xFF;
  rgb.r = r;
  rgb.g = g;
  rgb.b = b;
  return rgb;
}

void TestRGB() {
  TgaImageHead test;
  test.IDLength = 0;
  test.ColorMapType = 0;
  test.ImageType = 0x02;
  test.CMapDepth = 0;
  test.CMapLength = 0;
  test.CMapStart = 0;
  test.XOffset = 0;
  test.YOffset = 0;
  test.Width = 64;
  test.Height = 64;
  test.PixelDepth = 0x18;
  test.ImageDescriptor = 0x20;
  //uint8_t R=0,G=0,B=0;
  test = WriteTgaImage("test-space-4k.tga",test);
  uint32_t val = 0;
  int i,j,k;
  ColorMap Square2D[16];

  for(i=0;i<16;i++) {
    for(j=0;j<16;j++) {
      for(k=0;k<16;k++) {
        Square2D[k].x[i]=i;
        Square2D[k].y[j]=j;
      }
    }
  }
  for(i=0;i<16;i++) {
    Write4CLine(Square2D[i], i, test.File);
  }
  

  /*
  for(i=0;i<60;i++) {
    for(j=0;j<64;j++) {
      /*R = (val >> 8) & 0x0F;
      G = (val >> 4) & 0x0F;
      B = (val >> 0) & 0x0F;*/
      
     /* WriteTGAPixel(((i*4) & 0xF0)*255/240,((j*4) & 0xF0)*255/240,255,test.File);
      val++;
    }
  }*/
  fclose(test.File);
}

/*RgbColor AptToRgb(AptColor apt) {
  RgbColor rgb;
  uint16_t val = (apt.sv/4) * 64 + (apt.h/4);
  uint8_t R = (val >> 8) & 0xF;
  uint8_t G = (val >> 4) & 0xF;
  uint8_t B = (val >> 0) & 0xF;
  rgb.r = R << 4; rgb.g = G << 4; rgb.b = B << 4;
  return rgb;
}*/

/*AptColor RgbToApt(RgbColor rgb) {
  AptColor apt;
  uint8_t R = rgb.r >> 4;
  uint8_t G = rgb.g >> 4;
  uint8_t B = rgb.b >> 4;
  uint16_t val = (R << 8) + (G << 4) + B;
  uint16_t x_val = val % 64;
  uint16_t y_val = (val - x_val) / 64;
  apt.h = (x_val & 0x3F) * 4;
  apt.sv = (y_val & 0x3F) * 4; 
  return apt;
}*/

void WriteGLine(ColorMap16 cmap, uint8_t B,FILE *zp) {
  int i,j;
  for(j=0;j<16;j++) {
    for(i=0;i<4;i++) {
      WriteTGAPixel(cmap.y[i],cmap.x[j],B,zp); 
    }
  }
} 

void Write4CLine(ColorMap Square2D, uint8_t Bcol, FILE *zp) {
  int i,j;
  uint8_t R,G,B;
  ColorMap16 MapLine[4];
  for(i=0;i<16;i++) {
    for(j=0;j<16;j++) {
      R = Square2D.x[i] * 255/15;
      G = Square2D.y[j] * 255/15;
      B = Bcol*255/15;
      if((i<4) && (j<=16)) {
        MapLine[0].y[i]=R;
        MapLine[0].x[j]=G;
      }
      if(((i<8) && (i>=4)) && (j<=16)) {
        MapLine[1].y[i-4]=R;
        MapLine[1].x[j]=G;
      }
      if(((i<12) && (i>=8)) && (j<=16)) {
        MapLine[2].y[i-8]=R;
        MapLine[2].x[j]=G;
      }
      if(((i<16) && (i>=12)) && (j<=16)) {
        MapLine[3].y[i-12]=R;
        MapLine[3].x[j]=G;
      }
    }
  }
  WriteGLine(MapLine[0],B,zp);
  WriteGLine(MapLine[1],B,zp);
  WriteGLine(MapLine[2],B,zp);
  WriteGLine(MapLine[3],B,zp);
  

}