#ifndef BMP_LIB_H_
#  define BMP_LIB_H_

#  ifdef DLLLIB
#    ifdef BUILDING_DLL
#      define DLLIMPORT __declspec(dllexport)
#    else 
#      define DLLIMPORT __declspec(dllimport)
#    endif
#  else
#    define DLLIMPORT
#  endif

#  ifndef BMPCall 
#    define BMPCall __attribute__((__fastcall__))
#  endif

#  ifndef ulong
#    define ulong unsigned long
#  endif

#  ifndef ushort
#    define ushort unsigned short
#  endif

#  ifndef uint
#    define uint unsigned int
#  endif

#pragma pack(push,1)
typedef struct DLLIMPORT BMPSize_t{
   long width,height;
}BMPSize_t;
#pragma pack(pop)

#pragma pack(push,1)
typedef struct DLLIMPORT BMPFileHeader
{
   ushort Header_bm;
   ulong fileSize;
   ushort reserved[2];  
   ulong StartAddress; /* 0x3e for mono,0x76 for color */
   ulong Size;         /* HeaderSize */
   ulong width;
   ulong height;
   ushort Plane;
   ushort Bits;        /* bit per pixel */
   ulong Compression;  /* compression format */
   ulong SizeImage;  /* pixel size( sometimes you have to calc it you self ) */
   ulong XPelsPerMeter;
   ulong YPelsPerMeter;
   ulong ClrUsed;        /* total color, default 0 */
   ulong ClrImportant;   /* used pattle , default 0  */
}BMPFileHeader;
#pragma pack(pop)

typedef union DLLIMPORT Coord_t{
   struct {
      short x,y;
   }XYStruct;
   ulong dwCoord;
}Coord_t;

typedef struct DLLIMPORT BMP{
   struct{
      Coord_t coord;
      ulong color;
   }pen;
   BMPFileHeader header;
   ulong* table;  
}BMP;

#  ifdef __cplusplus
extern "C" {
#  endif

DLLIMPORT BMP*    BMPCall bmp_create(ulong width,ulong height,ulong bkclr);
DLLIMPORT BMP*    BMPCall bmp_create_from_file(const char* name);

DLLIMPORT int     BMPCall bmp_file_is_bmp(const char* fname);
DLLIMPORT ulong*  BMPCall bmp_buf(const BMP* bmp);
DLLIMPORT ulong*  BMPCall bmp_at(const BMP* bmp,long x,long y);

DLLIMPORT Coord_t BMPCall bmp_pen_where(const BMP* bmp);
DLLIMPORT ulong   BMPCall bmp_pen_color(const BMP* bmp);
DLLIMPORT ulong   BMPCall bmp_pos_clr(const BMP* bmp,long x,long y);

DLLIMPORT void    BMPCall bmp_point(BMP* bmp,long x,long y,ulong clr);
DLLIMPORT void    BMPCall bmp_line(BMP* bmp,long x1,long y1,long x2,long y2,ulong clr);
DLLIMPORT void    BMPCall bmp_lineto(BMP* bmp,long x,long y,ulong clr);
DLLIMPORT void    BMPCall bmp_rect(BMP* bmp,long x1,long y1,ulong w,ulong h,ulong clr);
DLLIMPORT void    BMPCall bmp_circle(BMP* bmp,long x,long y,ulong radix,double wR,double hR,ulong clr);
DLLIMPORT void    BMPCall bmp_paint(BMP* bmp,long x,long y,ulong clr);
DLLIMPORT void    BMPCall bmp_clear(BMP* bmp);

DLLIMPORT void    BMPCall bmp_get_region_size(BMPSize_t *dest,const BMP* bmp);
DLLIMPORT ulong   BMPCall bmp_get_image_size(const BMP* bmp);
DLLIMPORT ulong   BMPCall bmp_get_file_size(const BMP* bmp);

DLLIMPORT void    BMPCall bmp_write(const BMP* bmp,const char* name);
DLLIMPORT void    BMPCall bmp_paste(BMP* dst,const ulong* table,ulong x,ulong y,ulong w,ulong h);

DLLIMPORT int     BMPCall bmp_read_by_target(BMP* dest,const char* name);
DLLIMPORT int     BMPCall bmp_read_by_source(BMP* dest,const char* name);
DLLIMPORT void    BMPCall bmp_delete(BMP* bmp);
DLLIMPORT int     BMPCall bmp_img_bsize(const BMP* bmp);

#  ifdef __cplusplus
}
#  endif

#endif
