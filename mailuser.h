/* mailuser.h
 * Handles users, authentication and mail data
 * Author  : Jonatan Schroeder
 * Modified: Nov 5, 2017
 */

#ifndef _MAILUSER_H_
#define _MAILUSER_H_

#include <stdio.h>

#define MAX_USERNAME_SIZE 255
#define MAX_PASSWORD_SIZE 255


typedef struct user_list *user_list_t;
typedef struct mail_item *mail_item_t;
typedef struct mail_list *mail_list_t;

/**
* Function: is_valid_user
* =======================
* is_valid_user checks if a user with username and pasword exists
* returns: true if user exists, false if user does not exist.
* 
**/
int is_valid_user(const char *username, const char *password);

/**
* Functions: create_user_list
*            add_user_to_list
*            destroy_user_list
* =============================
* create_user_list: create a user list
* returns: created list
*
* add_user_to_list: adds user with username to list
* returns: void
*
* destroy_user_list: destroys user list
* returns: void 
**/
user_list_t create_user_list(void);
void add_user_to_list(user_list_t *list, const char *username);
void destroy_user_list(user_list_t list);

/**
* Functions: save_user_mail
*            load_user_mail
* =============================
* save_user_mail: Saves a new email message into the mail storage for a list of
* users. It assumes the temporary file 
* is in the same file system as the newly created files.
* returns: the created list
*
* load_user_mail: Creates a list of email messages for a username. 
*  The messages load the file names, sizes and are not kept in memory. 
*  returns: The created list of email messages 
*           If the user does not exist or does not have any messages, 
*           an empty list is returned.
**/
void save_user_mail(const char *basefile, user_list_t users);
mail_list_t load_user_mail(const char *username);

/**
* Functions: destroy_mail_list
*            get_mail_count
*            get_mail_item
*            get_mail_list_size
*            reset_mail_list_deleted_flag
* =============================
* destroy_mail_list: Frees all memory used by a list of emails. 
*                    Deletes files marked to be deleted.
* returns: void
*
* get_mail_count: Returns the number of email messages available in a list of
*                 emails, not counting messages marked as deleted.
* returns: Number of non-deleted messages in list.
*
* get_mail_item: Returns the email message object at a position in an email list. 
*                If the mail item is marked as deleted or the position is invalid, returns NULL. 
* returns: mail_item_t object corresponding to the message, or NULL
*           if the position is invalid or the message is marked as
*           deleted.
*
* get_mail_list_size: Returns the amount of bytes in email messages in an email list, 
*                     not counting messages marked as deleted.
* returns: Total size for all non-deleted messages in list.
*
* reset_mail_list_deleted_flag: Marks all deleted messages in a list as no longer deleted.
* returns: Number of recovered messages.
**/
void destroy_mail_list(mail_list_t list);
unsigned int get_mail_count(mail_list_t list);
mail_item_t get_mail_item(mail_list_t list, unsigned int pos);
size_t get_mail_list_size(mail_list_t list);
unsigned int reset_mail_list_deleted_flag(mail_list_t list);

/**
* Functions: get_mail_item_size
*            get_mail_item_filename
*            mark_mail_item_deleted
* =============================
* get_mail_item_size: Returns the total amount of bytes in an email message.
* returns: Size, in bytes, of an email message.
*
* get_mail_item_filename: Returns the name of the file.
* returns: Name of the file containing the email contents.
*
* mark_mail_item_deleted: Marks a message as deleted in the internal email list.
* returns: void
**/
size_t get_mail_item_size(mail_item_t item);
const char *get_mail_item_filename(mail_item_t item);
void mark_mail_item_deleted(mail_item_t item);

#endif
