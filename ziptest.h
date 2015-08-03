#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include <zip.h>

#define DBG_SEQ 0
#define LOGI printf
#define LOGD printf
#define LOGE printf

void search_dir ( const char * name );
void compress(const char *folderpath,const char *destzipname);