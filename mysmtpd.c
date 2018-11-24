#include "netbuffer.h"
#include "mailuser.h"
#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024

static void handle_client(int fd);

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    fprintf(stderr, "Invalid arguments. Expected: %s <port>\n", argv[0]);
    return 1;
  }
  
  run_server(argv[1], handle_client);
  
  return 0;
}

void handle_client(int fd) {
    char msg[100] = "220 Welcome to local host, SMTP Server \n";
    write(fd, msg, strlen(msg));
    
    char data[512];
    while(read(fd,data,512))
    {
        
        if(strncmp(data,"HELO",4)==0){
            char msg[100] = "HELO command recieved! Hello! \n";
            write(fd, msg, strlen(msg));
        }
        if(strncmp(data,"MAIL",4)==0){
            char msg[100] = "MAIL command recieved! Hello! \n";
            write(fd, msg, strlen(msg));
            
        }
    }
   
    
  // TODO To be implemented
}
