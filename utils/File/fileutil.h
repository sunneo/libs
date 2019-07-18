#ifndef __FILEUTIL_H
#  define __FILEUTIL_H
#include <sys/types.h>
off_t file_size(const char* filepath);
int   file_exists(const char* filepath);
int   file_isdir(const char* filepath);
int   file_makedir(const char* fpath);
time_t file_atime(const char* fpath);
time_t file_mtime(const char* fpath);
time_t file_ctime(const char* fpath);
int    file_changed(const char* fpath);
int   file_write(const char* fpath, const void* byte, size_t len);
int   file_append(const char* fpath,const void* byte,size_t len);
int   file_read(const char* fpath, void* inbyte,size_t len);
int  file_delete(const char* fpath);
void file_list(const char* path, void(*entry_fnc)(const char*),int onlydir);
FILE *file_open_memory(void *buf, size_t size, const char *mode);
#endif
