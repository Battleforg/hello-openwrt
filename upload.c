#include "upload.h"

void upload( const char * zipname){
  CURL *curl;
  CURLcode res;
  FILE *in;
  struct stat file_info;
  char *data;
  int len;

  stat(zipname, &file_info);
  off_t uploadsize = file_info.st_size;

  in = fopen(zipname, "rb");
  data = (char*)malloc(sizeof(char)*(uploadsize));
  fread(data,sizeof(char),uploadsize,in);

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {

     curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    // /* upload please */
    // curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://jxuao.me/upload?user=liuyk5&filename=data.zip");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,data);

    //  now specify which pointer to pass to our callback 
    // curl_easy_setopt(curl, CURLOPT_READDATA, in);

    /* Set the size of the file to upload */
    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, uploadsize);

    /* Perform the request, res will get the return code */
    res = curl_easy_perform(curl);
    /* Check for errors */
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    /* always cleanup */
    curl_easy_cleanup(curl);
    free(data);
    fclose(in);
  }
  curl_global_cleanup();
}

// int main(int argc, char const *argv[])
// {
// 	upload("README.zip");
// 	return 0;
// }
