
#ifndef FILE_IPHONECORE_H_INCLUDED
#define FILE_IPHONECORE_H_INCLUDED

#include <stdio.h>

FILE* LoadFile_iPhone(const char* filename, const char* search, const char* ext);
void GetFilePath(char*, const char*);
void GetHomePath(char*, const char*);

#endif