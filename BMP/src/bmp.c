#include "bmp.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "queue.h"
#ifndef __fastcall
#  define __fastcall __attribute__((__fastcall__))
#endif
__inline static BMP*  __fastcall bmp_create2(long width,long height,ulong bkclr,int bIgnoreInitTbl);
__inline static void  __fastcall bmp_init_header(BMP *bmp,long width,long height);
__inline static void  __fastcall bmp_init_table(BMP* bmp,ulong bkclr,int bIgnore);
__inline static void  __fastcall set_table(ulong* dest,ulong ch, uint size);
__inline static void  __fastcall zero_memory(unsigned char* dest,uint cnt);
__inline static long __fastcall bmp_offset(const BMP* rect,long x,long y);
__inline static void  __fastcall bmp_header_change_size(BMP* bmp,long width,long height);
__inline static long __fastcall ulmax(long a,long b);
__inline static long __fastcall ulmin(long a,long b);
__inline static long __fastcall region_offset(long h,long x,long y);
__inline static long __fastcall bmp_to_range(const BMP* bmp,long val,int bXY);
__inline static long  __fastcall _labs(long l);
__inline static long  __fastcall _lmin(long a,long b);
__inline static long  __fastcall _lmax(long a,long b);
__inline static float __fastcall _fabs(float f);
__inline static float __fastcall _fmax(float a,float b);
__inline static int   __fastcall bmp_out_of_range(const BMP* bmp,long x,long y);
__inline static int   __fastcall bmp_paint_help(BMP* bmp,Queue* queue,ulong clr,ulong target_clr);
__inline static long __fastcall bmp_offset_no_check(const BMP* bmp,long x,long y);
__inline static void __fastcall bmp_point_no_check(BMP* bmp,long x,long y,ulong clr);
__inline static ulong __fastcall bmp_pos_clr_no_check(const BMP* bmp,long x,long y);
static int __fastcall bmp_paint_has_next(BMP* bmp,Coord_t coord,ulong target_clr,int Dir,int bXY);

__inline static void __fastcall 
set_table(ulong* dest,ulong ch, uint size){
   while( size-- ){
      *dest++ = ch;
   }
}

__inline static void __fastcall 
zero_memory(unsigned char* dest,uint cnt)
{   
   while(cnt >= 4) {
      *(ulong*)dest++ = 0UL;    
      cnt -= 4;
   }
   while(cnt--) {
      *dest++ = '\0';
   }
}

int BMPCall bmp_file_is_bmp(const char* fname){
   FILE* fp = fopen(fname,"rb");
   short bm;
   fread(&bm,1,sizeof(short),fp);
   fclose(fp);
   return bm == 0x4d42;
}

ulong*   BMPCall 
bmp_buf(const BMP* bmp){
   return bmp?bmp->table:0;
}

ulong*  BMPCall 
bmp_at(const BMP* bmp,long x,long y){
   if(!bmp) return 0;
   return bmp_buf(bmp) + bmp_offset_no_check(bmp,x,y);
}

__inline static void __fastcall
bmp_header_change_size(BMP* bmp,long width,long height){
   bmp->header.SizeImage = width * height * 4;
   bmp->header.fileSize = bmp->header.SizeImage + sizeof(BMPFileHeader);
   bmp->header.width = width;
   bmp->header.height = height;
}

ulong BMPCall 
bmp_pos_clr(const BMP* bmp,long x,long y){
   return bmp->table[ bmp_offset(bmp,x,y)-1 ];
}

__inline static void __fastcall 
bmp_init_header(BMP *bmp,long width,long height){
   bmp->header.Bits = 32;
   bmp->header.Header_bm = 0x4d42;
   bmp_header_change_size(bmp,width,height);
   bmp->header.reserved[ 0 ] = bmp->header.reserved[ 1 ] = 0;
   bmp->header.StartAddress = sizeof(BMPFileHeader);
   bmp->header.Size = 40;  
   bmp->header.Plane = 1;  
   bmp->header.Compression = 0;
   bmp->header.XPelsPerMeter = 0;
   bmp->header.YPelsPerMeter = 0;
   bmp->header.ClrUsed = 0;
   bmp->header.ClrImportant = 0;
}

__inline static void __fastcall
bmp_init_table(BMP* bmp,ulong bkclr,int bIgnore){
   bmp->table = (ulong*)malloc(bmp->header.SizeImage);
   if(!bIgnore) set_table(bmp->table,bkclr,bmp->header.SizeImage / 4 );
   bmp->pen.coord.dwCoord = 0;
   bmp->pen.color = bkclr;
}

__inline static BMP* __fastcall
bmp_create2(long width,long height,ulong bkclr,int bIgnoreInitTbl){
   BMP* bmp = (BMP*)malloc(sizeof(BMP));
   bmp_init_header(bmp,width,height);
   bmp_init_table(bmp,bkclr,bIgnoreInitTbl);
   return bmp;
}


BMP* BMPCall 
bmp_create(ulong width,ulong height,ulong bkclr){
   return bmp_create2(width,height,bkclr,0);
}

BMP* BMPCall 
bmp_create_from_file(const char* name){
   BMP* bmp;
   FILE* fp;
   if(!bmp_file_is_bmp(name)) return 0;
   fp = fopen(name,"rb");
   if(!fp){
      fclose(fp);
      return 0;
   }
   bmp = (BMP*)calloc(1,sizeof(BMP));
   fread(&bmp->header,1,sizeof(BMPFileHeader),fp);
   bmp->header.SizeImage = bmp->header.fileSize - sizeof(BMPFileHeader);
   bmp->table = (ulong*)malloc(bmp->header.SizeImage);
   fread(bmp->table,1,bmp->header.SizeImage,fp);
   fclose(fp);
   return bmp;
}

Coord_t BMPCall 
bmp_pen_where(const BMP* bmp){
   return bmp->pen.coord;
}

ulong BMPCall 
bmp_pen_color(const BMP* bmp){
   return bmp->pen.color;
}


__inline static long __fastcall
region_offset(long h,long x,long y){
   return y * h + x;
}


__inline static long __fastcall
lmin(long a,long b){
   return a>b?b:a;
}
__inline static long __fastcall
lmax(long a,long b){
   return a>b?a:b;
}

__inline static long __fastcall
bmp_offset(const BMP* bmp,long x,long y){
   x = bmp_to_range(bmp,x,0);
   y = bmp_to_range(bmp,y,1);
   return y * bmp->header.width + x;
}

__inline static int __fastcall
bmp_out_of_range(const BMP* bmp,long x,long y){
  return(x >= bmp->header.width || x < 0 || y >= bmp->header.height || y < 0 );
}

__inline static long __fastcall
bmp_offset_no_check(const BMP* bmp,long x,long y){
   return y * bmp->header.width + x;
}

__inline static void __fastcall
bmp_point_no_check(BMP* bmp,long x,long y,ulong clr){
   bmp->table[ lmax( bmp_offset_no_check(bmp,x,y) -1,0) ] = clr;    
}    

__inline static ulong __fastcall 
bmp_pos_clr_no_check(const BMP* bmp,long x,long y){
   return bmp->table[ lmax(bmp_offset_no_check(bmp,x,y)-1,0) ];
}

void BMPCall 
bmp_point(BMP* bmp,long x,long y,ulong clr){
  bmp->pen.coord.XYStruct.x = x;
  bmp->pen.coord.XYStruct.y = y;
  if(!bmp_out_of_range(bmp,x,y))
    bmp_point_no_check(bmp,x,y,clr);
}

void BMPCall 
bmp_line(BMP* bmp,long x1,long y1,long x2,long y2,ulong clr){
   bmp_point(bmp,x1,y1,clr);
   bmp_lineto(bmp,x2,y2,clr);
}

__inline static long __fastcall 
bmp_to_range(const BMP* bmp,long val,int bXY){
   if(val < 0) return 0;
   return ulmin(val,bXY ? bmp->header.height : bmp->header.width);
}

__inline static long __fastcall
_labs(long l){
   return l<0?-l:l;
}

__inline static float __fastcall
_fabs(float f){
   return f<0?-f:f;
}


__inline static float __fastcall
_fmax(float a,float b){
   return a>b?a:b;
}


void BMPCall 
bmp_lineto(BMP* bmp,long X2,long Y2,unsigned long clr){
   float X1 = bmp->pen.coord.XYStruct.x ;
   float Y1 = bmp->pen.coord.XYStruct.y ;
   float DX = X2 - X1;
   float DY = Y2 - Y1;
   long STEPS = ulmax(_fabs(DX),_fabs(DY)) + 1;
   float DX_STEP = DX / STEPS;
   float DY_STEP = DY / STEPS;
   float X =  0.5f + X1;
   float Y =  0.5f + Y1;
   ulong U;
   for(U = 0; U < STEPS; ++U, X+=DX_STEP, Y+=DY_STEP){   
      bmp_point(bmp,X,Y,clr);
   }
   bmp_point(bmp,X2,Y2,clr); 
}

void BMPCall 
bmp_clear(BMP* bmp){
   set_table(bmp->table,0xffffffff,bmp->header.SizeImage/4);
}

void BMPCall bmp_get_region_size(BMPSize_t *dest,const BMP* bmp){
   dest->width = bmp->header.width;
   dest->height = bmp->header.height;
}

ulong BMPCall 
bmp_get_image_size(const BMP* bmp){
   return bmp->header.SizeImage;
}

ulong BMPCall 
bmp_get_file_size(const BMP* bmp){
   return bmp->header.fileSize;
}

void BMPCall 
bmp_write(const BMP* bmp,const char* name){
   FILE* fp = fopen(name,"wb");
   char buf[ 4096 ];
   setvbuf(fp,buf,_IOFBF,4096);
   fwrite(&bmp->header,1,sizeof(BMPFileHeader),fp);
   fwrite(bmp->table,1,bmp->header.SizeImage,fp);
   fflush(fp);
   fclose(fp);
}

__inline static long __fastcall
ulmax(long a,long b){
   return a>b?a:b;
}

__inline static long __fastcall
ulmin(long a,long b){
   return a>b?b:a;
}

#define BY_SOURCE 1
#define BY_TARGET 0
__inline static int __fastcall
read_img(BMP* dest,const char* name,int bByDstOrSrc){
   BMPFileHeader header;
   FILE* fp;
   void* emptyBuf;
   ulong x_remain_size,xmax,line;
   fp = fopen(name,"rb");
   fread(&header,1,sizeof(BMPFileHeader),fp);
   if(!fp)  return 0;
   switch(bByDstOrSrc){
     case BY_SOURCE:{
        if(dest->table)  free(dest->table);
        bmp_header_change_size(dest,header.width,header.height);
        dest->table = (ulong*)malloc(header.SizeImage);
        fread(dest->table,1,header.SizeImage,fp);
     }
     break;
      
     case BY_TARGET:{
        line = 0;
        xmax = ulmax(dest->header.width,header.width);
        x_remain_size = header.width - xmax;
        emptyBuf = malloc(x_remain_size);
        while(line++ < dest->header.height){
           fread(dest->table,1,xmax,fp);
           fread(emptyBuf,x_remain_size,1,fp);
        }
        free(emptyBuf);
     }
     break;
   }
   return 1;
}

int BMPCall 
read_by_target(BMP* dest,const char* name){
   return read_img(dest,name,BY_TARGET);
}

int BMPCall 
read_by_source(BMP* dest,const char* name){
   return read_img(dest,name,BY_SOURCE);
}

void BMPCall 
bmp_delete(BMP* bmp){
   free(bmp->table);
   free(bmp);
}

void BMPCall 
bmp_paste(BMP* dst,const ulong* table,ulong x,ulong y,ulong w,ulong h){
   ulong i,j;
   for(j = y; j< y+h; ++j){
      for(i = x; i< x+w; ++i){
         bmp_point(dst,x,y,table[ (j-y)*w + i ]);
      }
   }
}

void BMPCall 
bmp_rect(BMP* bmp,long x1,long y1,ulong w,ulong h,ulong clr){
   bmp_point(bmp,x1,y1,clr);
   bmp_lineto(bmp,x1+w,y1  ,clr);
   bmp_lineto(bmp,x1+w,y1+h,clr);
   bmp_lineto(bmp,x1  ,y1+h,clr);
   bmp_lineto(bmp,x1  ,y1  ,clr);
}

void BMPCall 
bmp_circle(BMP* bmp,long x,long y,ulong radix,double wR,double hR,ulong clr){  
#define PI 3.1415926535897932384626433832795
#define PIx2 6.283185307179586476925286766559
#define ANGLE 0.17453292519943295769236907684886
   double i = 0;
   Coord_t coord = bmp->pen.coord;
   bmp_point(bmp,x+radix,y,clr);
   for(i = 0; i<=PIx2; i+= ANGLE){
      bmp_lineto(bmp,x + wR * radix *  cos(i),y + hR * radix *  sin(i),clr);
   }
   bmp->pen.coord = coord;
}

static int __fastcall
bmp_paint_has_next(BMP* bmp,Coord_t coord,ulong target_clr,int Dir,int bXY){
   switch(bXY){
      case 0:
         return (!bmp_out_of_range(bmp,coord.XYStruct.x+Dir,coord.XYStruct.y) 
           &&(bmp_pos_clr_no_check(bmp,coord.XYStruct.x+Dir,coord.XYStruct.y) == target_clr));
      default:
         return (!bmp_out_of_range(bmp,coord.XYStruct.x,coord.XYStruct.y+Dir) 
           &&(bmp_pos_clr_no_check(bmp,coord.XYStruct.x,coord.XYStruct.y+Dir) == target_clr));
   }
}                        

__inline static void __fastcall
bmp_paint_draw_help_to_LR(BMP* bmp,Queue* queue,Coord_t coord,ulong target_clr,void(__fastcall *fn)(BMP* bmp,Queue* queue,Coord_t coord)){
  Coord_t tmp = coord; 
  if( bmp_paint_has_next(bmp,coord,target_clr,1,0 ) ){
     tmp.XYStruct.x = coord.XYStruct.x+1;
     fn(bmp,queue,tmp);
  }
  if(bmp_paint_has_next(bmp,coord,target_clr,-1,0) ){
     tmp.XYStruct.x = coord.XYStruct.x-1;
     fn(bmp,queue,tmp);
  }
}

static void __fastcall 
bmp_paint_draw_help_push_queue(BMP* bmp,Queue* queue,Coord_t coord){
   queue_push(queue,&coord);
}

static void __fastcall 
bmp_paint_draw_help_point(BMP* bmp,Queue* queue,Coord_t coord){
   bmp_point_no_check(bmp,coord.XYStruct.x,coord.XYStruct.y,bmp->pen.color);
}

__inline static void __fastcall
bmp_paint_draw_help(BMP* bmp,Queue* queue,ulong clr,ulong target_clr,int yDir,Coord_t coord){
   bmp_point(bmp,coord.XYStruct.x,coord.XYStruct.y,clr);
   if( bmp_paint_has_next(bmp,coord,target_clr,yDir,1)   )
      bmp_paint_draw_help_to_LR(bmp,queue,coord,target_clr,bmp_paint_draw_help_push_queue);
   while( bmp_paint_has_next(bmp,coord,target_clr,yDir,1)   ){
      coord.XYStruct.y += yDir;
      bmp_point_no_check(bmp,coord.XYStruct.x,coord.XYStruct.y,clr);
      bmp_paint_draw_help_to_LR(bmp,queue,coord,target_clr,bmp_paint_draw_help_push_queue);
      bmp_paint_draw_help_to_LR(bmp,queue,coord,target_clr,bmp_paint_draw_help_point);       
   }
}

__inline static int __fastcall
bmp_paint_help(BMP* bmp,Queue* queue,ulong clr,ulong target_clr){
   Coord_t coord;
   if(!queue_empty(queue)){
      coord = *(Coord_t*)queue_front(queue);
      queue_pop(queue);  
      bmp_paint_draw_help_to_LR(bmp,queue,coord,target_clr,bmp_paint_draw_help_push_queue);
      if( bmp_pos_clr_no_check(bmp,coord.XYStruct.x,coord.XYStruct.y) == target_clr ){
         bmp_paint_draw_help(bmp,queue,clr,target_clr,-1,coord);
         bmp_paint_draw_help(bmp,queue,clr,target_clr,1,coord);
      }
   }
   return !queue_empty(queue);
}

void BMPCall 
bmp_paint(BMP* bmp,long x,long y,ulong clr){
   Coord_t coord = { {x,y} };
   ulong target_clr = bmp_pos_clr(bmp,x,y);
   Queue* queue = queue_create(sizeof(Coord_t));
   bmp->pen.color = clr;
   bmp_point(bmp,x,y,clr);
   if(target_clr != clr){
      queue_push(queue,&coord);
      while( bmp_paint_help(bmp,queue,clr,target_clr) );
   }
   queue_delete(queue);
}

int BMPCall
bmp_img_bsize(const BMP* bmp){
   return bmp?bmp->header.fileSize-sizeof(BMPFileHeader):0;
}
