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
    
    char msg[100] = "+OK Welcome to local host, POP3 Server \n";
    
    write(fd, msg, strlen(msg));
    net_buffer_t netbuffer= nb_create(fd, 200);
    char data[512];
    //define checks
    int auth=0;
    //user is username check
    int user=0;
    //pass is password check
    int len=0;
    char username[200];
    while(len=nb_read_line(netbuffer, data)){
       printf("%i",len);
       // printf("%i",pass);
  //      int q=is_valid_user("anna", "anime");
      //     printf("\nisvalid??%i\n",q);
        char buffer[strlen(data)];
        memcpy(buffer, data, strlen(data));
        char* p;
        p=buffer;
        
         if(strncasecmp(data, "quit",4)==0){
            char msg[100] = "+OK local host Service closing transmission channel \n";
            write(fd, msg, strlen(msg));
            break;
        }
        
        else if(strncasecmp(p, "user",4)==0){
          
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
           
            if(pch!=NULL){
                
            strncpy(username, pch,len-7);
                
            printf ("\n%s\n",username);
                
            if(is_valid_user(username, NULL)!=0){
                user=1;
                char msg[100] = "+OK Enter password next! \n";
                write(fd, msg, strlen(msg));
            } else {
                char msg[100] = "-ERR User not in list! \n";
                write(fd, msg, strlen(msg));
            }
            }else {
            char msg[100] = "-ERR wrong format! Format is 'user email' in that order. \n";
            write(fd, msg, strlen(msg));
            }
        }
        
        else if(strncasecmp(p, "pass",4)==0){
            char password[len-12];
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
            
            if(pch!=NULL){
                
            strncpy(password, pch,len-7);
            printf("%s",username);
            printf ("\n%s\n",password);
            if(is_valid_user(username, password)!=0){
                //pass=1;
                char msg[100] = "+OK Welcome! \n";
                write(fd, msg, strlen(msg));
            } else {
                printf("user:%s\npass:%s",username,password);
                char msg[100] = "-ERR WRONG PASSWORD. \n";
                write(fd, msg, strlen(msg));
            }
            }else{
            char msg[100] = "-ERR wrong format! Format is 'pass password' in that order. \n";
            write(fd, msg, strlen(msg));
            }
        }
        else if(strncasecmp(data, "noop",4)==0){
            char msg[100] = "+OK No opperation \n";
            write(fd, msg, strlen(msg));
           
        }
        else if(strncasecmp(data, "stat",4)==0){
            
           int j = get_mail_count(load_user_mail(username));
            printf("%i",j);
            char msg[100] = "+OK Mail Count is \n";
            write(fd, msg, strlen(msg));
            char c=j+'0';
        //   char c[100];
           // strcpy(c, j);
            write(fd, &c, strlen(msg));
           
        }
        else if(strncasecmp(p, "list",4)==0){
            
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
           // int x;
            //strcpy(x, pch);
            //if we dont want to list all the emails
            if(pch!=NULL){
                mail_list_t maillist= load_user_mail(username);
                size_t t=get_mail_item_size(get_mail_item(maillist, pch));
                write(fd, &t, 100);
                char msg[100] = "+OK 220 \n";
                write(fd, msg, strlen(msg));
              
            }
            //list all emails
            else{
                 mail_list_t maillist= load_user_mail(username);
                char msg[100] = "+OK count is ";
                write(fd, msg, strlen(msg));
                int p=get_mail_count(load_user_mail(username));
                char c=p+'0';
                write(fd, &c, strlen(msg));
                
                size_t k=get_mail_list_size(maillist);
                 write(fd, &k, strlen(msg));
                int l=get_mail_count(maillist);
                for(int i =0; i < l ; i++){
                     write(fd, &i, strlen(msg));
                     size_t t=get_mail_item_size(get_mail_item(maillist, i));
                    write(fd, &t, 100);
                }
            }
         
        }
        else if(strncasecmp(data, "retr",4)==0){
            char msg[100] = "221 local host Service closing transmission channel \n";
            write(fd, msg, strlen(msg));
           
        }
        else if(strncasecmp(data, "dele",4)==0){
            char msg[100] = "221 local host Service closing transmission channel \n";
            write(fd, msg, strlen(msg));
          
        }
       else if(strncasecmp(data, "rset",4)==0){
            char msg[100] = "221 local host Service closing transmission channel \n";
            write(fd, msg, strlen(msg));
          
        }
    }
   
  //QUIT
  // TODO To be implemented
}
