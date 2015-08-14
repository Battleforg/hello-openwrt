#ifndef _UPLOAD_H_
#define _UPLOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

/* somewhat unix-specific */
#include <sys/time.h>
#include <unistd.h>

/* curl stuff */
#include <curl/curl.h>

char urls[90];
void seturls();
int upload( const char * zipname);
#endif

