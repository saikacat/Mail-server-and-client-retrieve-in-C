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
   ///test
   int i= is_valid_user("john.doe@example.com", NULL);
    printf("%i",i);
    
    char msg[100] = "220 Welcome to local host, SMTP Server \n";
    write(fd, msg, strlen(msg));
   net_buffer_t netbuffer= nb_create(fd, MAX_LINE_LENGTH);
    char data[512];
    while(nb_read_line(netbuffer, data)){
        printf("%s",data);
        // write(fd, data, strlen(data));
        if((strncmp(data,"EHLO",4)==0)||(strncmp(data,"RSET",4)==0)||(strncmp(data,"VRFY",4)==0)||(strncmp(data,"EXPN",4)==0)||(strncmp(data,"HELP",4)==0)){
            char msg[100] = "502 Unsupported Command \n";
            write(fd, msg, strlen(msg));
        }
        if(strncmp(data,"QUIT",4)==0){
            char msg[100] = "221 local host Service closing transmission channel \n";
            write(fd, msg, strlen(msg));
            break;
        }
        ///NOOP Command
        if(strncmp(data,"NOOP",4)==0){
            char msg[100] = "250 OK  \n";
            write(fd, msg, strlen(msg));
        }
        if(strncmp(data,"HELO",4)==0){
            char msg[100] = "250 HELO command recieved! Hello! \n";
            write(fd, msg, strlen(msg));
        }
        if(strncmp(data,"MAIL",4)==0){
            char output[512];
            if(strncmp(data,"MAIL FROM:<",11)==0){
                int x=0;
                // checks for < and >, takes data in between.
                for(int i=0; i<strlen(data);i++){
                    if (data[i]=='<'){
                        x=1;
                    }
                    if (data[i]=='>'){
                        x=0;
                    }
                    if(x==1){
                        //test print first
                       // char msg = data[i];
                        output[i-11]=data[i];
                    }
                }
                char msg[100] = "250 MAIL command recieved from\n";
                write(fd, msg, strlen(msg));
                write(fd, output, strlen(msg));
            }
                else{
                    char msg[100] = "500 Wrong format! Format is MAIL FROM:<....> \n";
                    write(fd, msg, strlen(msg));
                }
            }
        if(strncmp(data,"RCPT",4)==0){
            char output[512];
            if(strncmp(data,"RCPT TO:<",9)==0){
                int x=0;
                char* filename = "/a3_y6o1b/users.txt";
                
            FILE *fp=fopen(filename,"r");
                if(fp==NULL){
                    printf("feelsbad\n");
                }
                // checks for < and >, takes data in between.
                for(int i=0; i<strlen(data);i++){
                    if (data[i]=='<'){
                        x=1;
                    }
                    if (data[i]=='>'){
                        x=0;
                    }
                    if(x==1){
                        //test print first
                        // char msg = data[i];
                        output[i-11]=data[i];
                    }
                    char user[30];
                    int ret=fscanf(fp, "%s",user);
                    printf("user\n");
                     printf("%s",user);
                    
                }
                char msg[100] = "250 OK \n";
                write(fd, msg, strlen(msg));
                write(fd, output, strlen(msg));
            }
            else{
                char msg[100] = "500 Wrong format! Format is RCPT FROM:<....> \n";
                write(fd, msg, strlen(msg));
            }
        }
        if(strlen(data)>MAX_LINE_LENGTH){
            char msg[100] = "500 Line too long! \n";
            write(fd, msg, strlen(msg));
        }
        
     //   read(fd,data,10);
   //nc -c 127.0.0.1 6000
  // TODO To be implemented
    }
}
