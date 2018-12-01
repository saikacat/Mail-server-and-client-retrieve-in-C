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
    while((len=nb_read_line(netbuffer, data))){
        
       printf("%i",len);
       // printf("%i",pass);
        int q=is_valid_user("mary.smith@example.com", "maryp");
           printf("\nisvalid??%i\n",q);
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
            char password[256]="";
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
         
            if(pch!=NULL){
            strncpy(password, pch,len-7);
            printf("%s",username);
            printf ("\n%s\n",password);
            password [ strcspn(password, "\r\n") ] = '\0';
            if(is_valid_user(username, password)!=0){
                //pass=1;
                auth=1;
                char msg[100] = "+OK Welcome! \n";
                write(fd, msg, strlen(msg));
            } else {
                printf("user:%s\npass:%s",username,password);
                memset(msg, 0, sizeof msg);
                char msg[100] = "-ERR WRONG PASSWORD. \n";
                write(fd, msg, strlen(msg));
            }
            }else{
            char msg[100] = "-ERR wrong format! Format is 'pass password' in that order. \n";
            write(fd, msg, strlen(msg));
            }
        }
        else if(strncasecmp(data, "noop",4)==0){
            if(user!=1&&auth!=1){
            char msg[100] = "+OK No opperation \n";
            write(fd, msg, strlen(msg));
            }
            else{
                char msg[100] = "-ERR Cannot be used after login \n";
                write(fd, msg, strlen(msg));
            }
        }
        else if(strncasecmp(data, "stat",4)==0){
            if(user==1&&auth==1){
           int j = get_mail_count(load_user_mail(username));
            printf("%i",j);
            char msg[100] = "+OK ";
            write(fd, msg, strlen(msg));
            char c=j+'0';
            send_string(fd, &c);
            unsigned long k=get_mail_list_size(load_user_mail(username));
            send_string(fd, " %lu",k);
            send_string(fd, "\n");
            }
            else{
                char msg[100] = "-ERR Login First! ";
                write(fd, msg, strlen(msg));
            }
           
        }
        else if(strncasecmp(p, "list",4)==0){
              if(user==1&&auth==1){
                 
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
        int x;
          if(pch!=NULL)
            x= atoi(pch);
           // send_string(fd, " x is run ");
            //if we dont want to list all the emails
            if(pch!=NULL){
                mail_list_t maillist= load_user_mail(username);
                x=x-1;
                unsigned long t=get_mail_item_size(get_mail_item(maillist, x));
                x=x+1;
                char q = x + '0';
                send_string(fd,"%s", &q);
                send_string(fd, "st item size is ");
                send_string(fd,"%lu\n", t);
            }
            //list all emails
            else{
                 mail_list_t maillist=load_user_mail(username);
                int j = get_mail_count(load_user_mail(username));
                printf("%i",j);
                char msg[100] = "+OK Mail Count is ";
                write(fd, msg, strlen(msg));
                char c=j+'0';
                send_string(fd, &c);
                send_string(fd, " size of mailbox is ");
              
                
                unsigned long k=get_mail_list_size(maillist);
                send_string(fd, "%lu",k);
                send_string(fd, "\n");
                
                for(int i =0; i < j ; ){
                     unsigned long t=get_mail_item_size(get_mail_item(maillist, i));
                    i=i+1;
                    char q = i + '0';
                    send_string(fd,"%s", &q);
                    send_string(fd, "st item size is ");
                    send_string(fd,"%lu\n", t);
                }
            }
              }
              else{
                  char msg[100] = "-ERR Login first! \n";
                  write(fd, msg, strlen(msg));
              }
        }
        else if(strncasecmp(p, "retr",4)==0){
            char mailname[100];
           // char user[100];
            mail_list_t maillist=load_user_mail(username);
          //  int j = get_mail_count(load_user_mail(username));
            
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
            
            char msg2[100] = "+OK ";
            write(fd, msg2, strlen(msg));
          
          
          //building the mail path directory name
            int x= atoi(pch);
            // 0 indexed to 1 indexed
            x=x-1;
            
            //print the size
           unsigned long k= get_mail_item_size(get_mail_item(maillist, x));
         
            send_string(fd, "%lu",k);
            send_string(fd, " octets \n");

            strcpy(mailname, "./mail.store/");
           
            strcat(mailname, username);
             strcat(mailname, "/");
            char mailname2[100];
            snprintf(mailname2, sizeof mailname2, "%d.mail", x);
            strcat(mailname, mailname2);
            printf("\n%s\n",mailname);
            //open function of mail
            FILE *f;
            char c;
         
            f=fopen(mailname,"rt");
            while((c=fgetc(f))!=EOF){
                send_string(fd,"%c", c);
            }
            
            fclose(f);
            char enddot[100] = ". \n";
            write(fd, enddot, strlen(msg));
          //  return 0;
            //getline(msg, 100, filename);
            char msg[100] = "+OK That is your message \n";
            write(fd, msg, strlen(msg));
           
        }
        else if(strncasecmp(data, "dele",4)==0){
            //getting number of deleted message.
            char* pch = strtok (p," ");
            pch = strtok (NULL, " ");
            
            int x= atoi(pch);
            //-1 for function
            x=x-1;
              mail_list_t maillist=load_user_mail(username);
            mark_mail_item_deleted( get_mail_item(maillist, x));
            //+1 for user to see
            x=x+1;
             char c=x+'0';
          
            
            
            char msg[100] = "+OK message ";
            send_string(fd, msg);
            send_string(fd, &c);
            send_string(fd, " deleted \n");
           
          
        }
       else if(strncasecmp(data, "rset",4)==0){
            mail_list_t maillist=load_user_mail(username);
           reset_mail_list_deleted_flag(maillist);
            char msg[100] = "+OK Undeleted messages \n";
           send_string(fd, msg);
          
        }
       else if(strncasecmp(data, "quit",4)==0){
           mail_list_t maillist=load_user_mail(username);
           reset_mail_list_deleted_flag(maillist);
           destroy_mail_list(maillist);
           char msg[100] = "+OK Deleted messages \n";
           send_string(fd, msg);
           
       }
    }
   
  //QUIT
  // TODO To be implemented
}
