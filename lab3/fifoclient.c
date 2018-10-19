#include <stdio.h> 
#include <stdlib.h> 
#include <sys/stat.h> 
#include <unistd.h> 
#include <linux/stat.h>

#define  FIFO_FILE  "myfifo" 
#define  SZBUF  80 
#define  QUIT  "bye" 

int main(void) 
{ 
    FILE* fp; 
    char readbuf[SZBUF]; 

    /*create the fifo*/ 
    umask(0); 
    mknod(FIFO_FILE, S_IFIFO|0666, 0); 

    printf("Running server... To quit, send via the client the msg 'bye'.\n"); 

    while(1) { 
        fp = fopen(FIFO_FILE,"r"); 
        fgets(readbuf, SZBUF, fp); 

        if (!strcmp(readbuf,QUIT)) break; 

        printf("Received string: %s\n",readbuf); 
        fclose(fp); 
    } 
    return 0; 
} 