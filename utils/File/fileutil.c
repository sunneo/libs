#include <stdio.h>

#include <sys/stat.h>
#include <errno.h>
#ifdef __linux__
#include <dirent.h>
#include <unistd.h>
#endif
#include "fileutil.h"
#ifdef WIN32
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
#endif
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
#ifdef __linux__
      return (S_ISDIR(st.st_mode)) != 0;
#else
       return (st.st_mode & _S_IFDIR) != 0;
#endif
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

#ifndef __linux__

DWORD TraverseDir(char* dir, void(*entry_fnc)(TCHAR*), int(*filter)(TCHAR*), int onlydir, int recurse)
{
    WIN32_FIND_DATA ffd;
    TCHAR szDir[MAX_PATH];

    size_t length_of_arg;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    // If the directory is not specified as a command-line argument,
    // print usage.



    // Check that the input path plus 3 is not longer than MAX_PATH.
    // Three characters are for the "\*" plus NULL appended below.

    StringCchLength(dir, MAX_PATH, &length_of_arg);
    // Prepare string for use with FindFile functions.  First, copy the
    // string to a buffer, then append '\*' to the directory name.

    StringCchCopy(szDir, MAX_PATH, dir);
    StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

    // Find the first file in the directory.

    hFind = FindFirstFile(szDir, &ffd);

    if (INVALID_HANDLE_VALUE == hFind)
    {
        return dwError;
    }

    // List all the files in the directory with some info about them.

    do
    {
        if (onlydir)
        {
            if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                continue;
            }
        }
        entry_fnc(ffd.cFileName);
        if (recurse &&
            ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && 
            (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)))
        {
            int cond = 1;
            if (filter != NULL){
                cond = filter(ffd.cFileName);
            }
            else{
                cond = _tcscmp(ffd.cFileName, TEXT(".")) != 0
                    && _tcscmp(ffd.cFileName, TEXT("..")) != 0
                    && _tcscmp(ffd.cFileName, TEXT(".git")) != 0
                    && _tcscmp(ffd.cFileName, TEXT(".svn")) != 0;
            }
            if (cond){
                TCHAR fullDir[MAX_PATH];
                StringCchCopy(fullDir, MAX_PATH, dir);
                StringCchCat(fullDir, MAX_PATH, TEXT("\\"));
                StringCchCat(fullDir, MAX_PATH, ffd.cFileName);
                TraverseDir(fullDir, entry_fnc, filter, onlydir, recurse);
            }
        }
    } while (FindNextFile(hFind, &ffd) != 0);
    dwError = GetLastError();
    FindClose(hFind);
    return dwError;
}
#endif
void file_list(const char* path, void(*entry_fnc)(const char*),int onlydir){
#ifdef __linux__
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
#else
    TraverseDir(path, entry_fnc, NULL, 0, 0);
#endif
}

FILE *file_open_memory(void *buf, size_t size, const char *mode)
{
#if defined WIN32 || defined _WIN32 || defined __WIN32__
    char temppath[MAX_PATH - 13];
    if (0 == GetTempPath(sizeof(temppath), temppath)) {
        puts("Can't get temp path");
        return NULL;
    }
    char filename[MAX_PATH + 1];
    if (0 == GetTempFileName(temppath, "IQT", 0, filename)) {
        puts("Can't get file name");
        return NULL;
    }
    printf("file::%s\n", filename);
    /* FILE *f = fopen(filename, "wb");
    if (NULL == f)
    return NULL;
    */
    FILE *f;
    errno_t err;

    if ((err = fopen_s(&f, filename, "wb")) != 0)
        printf("The file '%s' was not opened\n", filename);

    fwrite(buf, size, 1, f);
    fclose(f);



    /* return fopen(filename, mode); */
    FILE *f2 = NULL;
    err = fopen_s(&f2, filename, mode);
    return f2;
#else
    return fmemopen(buf,size,mode);
#endif
}

