#include "csapp.h"

int main(void) {

  char *buf, *p;
  char arg1[MAXLINE], arg2[MAXLINE], content[MAXLINE];
  float n1; char *n2;
  char *cur, *number, *currency, *amount;

  /* Extract the two arguments */
  if ((buf = getenv("QUERY_STRING")) != NULL) {
    p = strchr(buf, '&');
    cur = strchr(p, '=');
    currency = cur + sizeof(char);
    *p = '\0';
    strcpy(arg1, buf);
    number = strchr(arg1, '=');
    amount = number + sizeof(char);
    n1 = atof(amount);
    fprintf(stderr, "n1=%f\n", n1);
    n2 = currency;
    fprintf(stderr, "n2=%s\n", n2);
  }

  /* Make the response body */
  char *url; char strC[90];

  url = "http:currencies=&next";

  int x = 16;
  strncpy(strC,url,x);
  strC[x] = '\0';
  strcat(strC,n2);
  strcat(strC,url+x);
  fprintf(stderr, "url %s\n",strC);

  float answer = 10.0;
  sprintf(content, "<html><head><title>Page Title</title></head><body>\
<h1>Kayla and Shira's Currency Converter</h1><p>That is %f in %s! Have a\
 great day!</p></body></html>\n", answer, n2);

  /*    printf("this is what '%s'\n", p);*/
  /* Generate the HTTP response */
  printf("Connection: close\r\n");
  printf("Content-length: %d\r\n", (int)strlen(content));
  printf("Content-type: text/html\r\n\r\n");
  printf("%s", content);
  fflush(stdout);

  exit(0);
}
/* $end adder */
