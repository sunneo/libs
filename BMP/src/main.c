#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bmp.h"
#define WIDTH 800
#define HEIGHT 600
int main(int argc, char *argv[])
{
   BMP* bmp = bmp_create(WIDTH,HEIGHT,0xffffffff);
   char fname[ 256 ];
   int i = 0;
   ulong x,y;
   srand(time(NULL));
   x = rand()%WIDTH;
   y = rand()%HEIGHT;
   if( !opendir("bmpResult") )
      mkdir("bmpResult");
   chdir("bmpResult");
   snprintf(fname,256,"test_%02d_%02d.bmp",x,y);
   bmp_circle(bmp,x,y,50,1,1,rand());
   bmp_paint(bmp,0,HEIGHT,rand()|rand()<<15);
   bmp_write(bmp,fname);
   bmp_delete(bmp);
   system("PAUSE");	
   return 0;
}
