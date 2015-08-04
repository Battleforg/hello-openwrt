#include "upload.h"

void upload(void){
  CURL *curl;
  CURLcode res;
  FILE *in;
  struct stat file_info;
  char *data;
  int len;

  stat("inc.zip", &file_info);
  off_t uploadsize = file_info.st_size;

  in = fopen("inc.zip", "rb");
  fseek(in,0,SEEK_END); 
  len = ftell(in); 
  fseek(in,0,SEEK_SET);
  data = (char*)malloc(sizeof(char)*(len+1));
  fread(data,sizeof(char),len,in);

  /* In windows, this will init the winsock stuff */
  curl_global_init(CURL_GLOBAL_ALL);

  /* get a curl handle */
  curl = curl_easy_init();
  if(curl) {

    /* upload please */
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
    /* First set the URL that is about to receive our POST. This URL can
       just as well be a https:// URL if that is what should receive the
       data. */
    curl_easy_setopt(curl, CURLOPT_URL, "http://jxuao.me/upload?user=liuyk5&filename=inc.zip");
    /* Now specify the POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS,data);

    /* now specify which pointer to pass to our callback */
    curl_easy_setopt(curl, CURLOPT_READDATA, in);

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
  }
  curl_global_cleanup();
}