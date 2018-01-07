
/* <DESC>
* Shows how the write callback function can be used to download data into a
 * chunk of memory instead of storing it in a file.
 * </DESC>
 */
#include "csapp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
  char *memory;
  size_t size;
};

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

/*create a function that concatenates three strings to make it easier later on
I got the code from https://stackoverflow.com/questions/8465006/how-do-i-concatenate-two-strings-in-c"*/

char* concat(const char *s1, const char *s2, const char *s3){
  /*malloc so we know we have enough space*/
  char *result = malloc(strlen(s1) + strlen(s2) + strlen(s3) + 1); /*for the null */
  strcpy(result, s1);
  strcat(result,s2);
  strcat(result,s3);
  return result;
}
		       
int main(void)
{
  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);  /* will be grown as needed by the realloc above */
  chunk.size = 0;    /* no data at this point */

  curl_global_init(CURL_GLOBAL_ALL);

  /* init the curl session */
  curl_handle = curl_easy_init();

  /* specify URL to get */
  /*take the parameters that the client inputed into the web page and add them to the first part of the api address, then add the second part to the api address, use concat function*/
  char* countryCode = "ILS";
    char* APIservice = concat("http://apilayer.net/api/live?access_key=948cdf688cfa7116991ee380962b067b&currencies=", countryCode,"&format=1");
    /*char* linkToService = "http://apilayer.net/api/live?access_key=948cdf688cfa7116991ee380962b067b&currencies=ILS \
      &format=1";*/
  curl_easy_setopt(curl_handle,CURLOPT_URL, APIservice);
  free(APIservice);

  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  /* we pass our 'chunk' struct to the callback function */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);

  /* some servers don't like requests that are made without a user-agent
     field, so we provide one */
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

  /* get it! */
  res = curl_easy_perform(curl_handle);

  /* check for errors */
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
	    curl_easy_strerror(res));
  }
  else {
    char* byte_array = chunk.memory;
    char *p, *q, *r,*s,*t,*u,*v, *w,*x,*y;
    int lenChar = sizeof(char);
   
    p = strchr(byte_array, ':');
    /*clean up after yourself*/
    byte_array = '\0';
    q = strchr((p+lenChar),':');
    p = '\0';
    r = strchr((q+lenChar),':');
    q = '\0';
    s = strchr((r+lenChar),':');
    r = '\0';
    t = strchr((s+lenChar),':');
    s = '\0';
    u = strchr((t+lenChar),':');
    t = '\0';
    v = strchr((u+lenChar), ':');
    u = '\0';
    w= strchr((v+lenChar), ':');
    v = '\0';
    x = strchr((w+lenChar), ':');
    w = '\0';
    
    char *token;
    char *search = "\n";
    /* fprintf(stderr,"%s hi \n",x);*/
    /*take on the number from the return statment from the api, since the data looks like this :##.##, get rid of the ':' and then change split up the the entire string so only the number remains, then assign it to float and change it to atoof value*/
    token = strtok(x, search);
    float theCurrent = (atof)(token+lenChar);
    fprintf(stderr,"this is the most recent currency value in a float: %f\n",theCurrent);
   
  }

  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  /* we're done with libcurl, so clean it up */
  curl_global_cleanup();

  return 0;
}

