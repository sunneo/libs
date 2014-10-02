#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "fileutil.h"

off_t file_size(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return st.st_size;
   }
   return 0;
}

int file_exists(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return 1;
   }
   return 0;
}

int file_isdir(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return (S_ISDIR(st.st_mode)) != 0;
   }
   return 0;
}

int file_makedir(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) != 0 && errno == ENOENT){
      if(errno == ENOENT){
         return mkdir(filepath,0755);
      }
   }
   return 0;  
}

time_t file_atime(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return st.st_atime;
   }
   return 0;
  
}

time_t file_mtime(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return st.st_mtime;
   }
   return 0;
}

time_t file_ctime(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return st.st_ctime;
   }
   return 0;
}

int file_changed(const char* filepath){
   struct stat st;
   if(stat(filepath, &st) == 0){
      return (st.st_atime != st.st_mtime) || (st.st_atime != st.st_ctime);
   }
   return 0;
  
}


static int file_write_append_help(const char* fpath,int append, const void* byte, size_t len){
   FILE* fp;   
   size_t chunks = len / 4096;
   fp  = fopen(fpath,append?"ab":"wb");
   if(!fp){
      return 0;
   }
   fwrite(byte,4096,chunks,fp);
   if(len % 4096 > 0){
      fwrite(((char*)byte)+chunks*4096,1,len%4096,fp);
   }
   fflush(fp);
   fclose(fp);
   return len;
}

int file_append(const char* fpath,const void* byte,size_t len){
   return file_write_append_help(fpath,1,byte,len);
}
int file_write(const char* fpath, const void* byte, size_t len){
   return file_write_append_help(fpath,0,byte,len);
}

int file_read(const char* fpath, void* inbyte,size_t len){
   FILE* fp;   
   off_t fsize = file_size(fpath);
   size_t chunks;
   if(fsize > len){
      fsize = len;
   }
   chunks = fsize / 4096;
   fp  = fopen(fpath,"rb");
   if(!fp){
      perror("at file_read");
      return 0;
   }
   fread(inbyte,4096,chunks,fp);
   if(fsize % 4096 > 0){
      fread(((char*)inbyte)+(chunks*4096),1,fsize%4096,fp);
   }
   fflush(fp);
   fclose(fp);
   return fsize;
}

int file_delete(const char* fpath){
   return remove(fpath) == 0;
}

void file_list(const char* path, void(*entry_fnc)(const char*),int onlydir){
   DIR* dir;
   struct dirent* entry;
   if(!entry_fnc){ 
      return;
   }
   dir = opendir(path);
   if(!dir){
      return;
   }
   while((entry=readdir(dir)) != NULL){
      if(onlydir){
         if(entry->d_type != DT_DIR){
            continue;
         }
      }
      entry_fnc(entry->d_name);
   }
   closedir(dir);
}

