#include <stdio.h>
#include <stdlib.h>
#define  FIFO_FILE  "myfifo"

int main(int argc, char* argv[])
{
  FILE* fp;
  if (argc != 2) {
    fprintf(stderr,"USAGE %s [string]\n",argv[0]);
    return 1;
  }

  if ( (fp = fopen(FIFO_FILE,"w")) == NULL ) {
    perror("fopen");
    return 1;
  }

  fprintf(fp,"%s",argv[1]);
  fclose(fp);
  return 0;
}