//usage : mem_mgr <input file> <mem_pool_size>

#include "comment_removal.h"
#include "mymalloc.c"

#include <stdio.h> 
#include <string.h> 
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

//singly linked list to store value and pointers to allocated memory
typedef struct node {
	int val; 
    void *point;
    struct node * next;
} node_t;

//initialize linked list head to NULL
node_t * head2 = NULL;

//functions
int command_check(char *array[3]);
void add_list(node_t * head2, int val, void *point);
void command_exe_m(char *array[3]);
int command_exe_f(char *array[3]);
void rem_list(node_t *head_ref, int key);

void main(int argc, char *argv[]){
//checking if valid arguments
    if (argc != 3){
        printf("Invalid number of arguments. Valid input \"mem_mgr <input file> <mem_pool_size>\"\n ");
        exit(1);
    }
    if( access( argv[1], F_OK ) == -1 ) {
        printf("Invalid file input. Valid input \"mem_mgr <input file> <mem_pool_size>\"\n");
        exit(1);
    }
    if( ! isdigit(*argv[2]) ){
        printf("Invalid memory pool size. Valid input \"mem_mgr <input file> <mem_pool_size>\"\n ");
        exit(1);
    }

    //removing comments from the provided text file and saving it as mynewfile.txt
    comment(argv[1]);
    int mem_pool = atoi(argv[2]);
    //initializing memory 
    mem_initialize(mem_pool);

    //opening mynewfile.txt
    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    fp = fopen("mynewfile.txt", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    //initialize head
    head2 = malloc(sizeof(node_t));
    head2->val = -1;
    head2->next = NULL;

    //line by line text processing
    while (( getline(&line, &len, fp)) != -1) {
        if( strcmp(line,"\n"  ) != 0 && strcmp(line,"\r\n") != 0 && strcmp(line,"\0"  ) != 0 && 1){
            char output[255];
            //removing extra white spaces
            remove_extra_whitespaces(line,output);
            char * p;
            int i=0;
            char *array[3];
            p = strtok (output," ");
            //tokenizing the string
            while (p != NULL){
                array[i++] = p;
                p = strtok (NULL, " ");
            }
            //if number of tokens is between 2 and 4 then it is valid accespted command
            if( i>=2 && i<=4 ){
                //checks the type of command i.e. 3.print / 1.malloc / 2.free
                int cmd_type = command_check(array);
                //command malloc
                if (cmd_type == 1)
                    command_exe_m(array);
                //command free
                else if ( cmd_type == 2){
                    int temp = command_exe_f(array);
                    if (temp == 0)
                        printf("Free command number not found or already freed at command # %s\n", array[0]); 
                    if (temp == 1 )
                        printf("free command at command # %s sucessful\n",array[0]);
                }
                //command print
                else if (cmd_type==3)
                    print_block();
            }
            else{
                printf("invalid command line : contains more than 3 words\n");
            }
            free(p);
        }
        else{
            continue;
        }
    }
    fclose(fp);
    free(line);
    //cleaning up the temporary temp file
    remove("mynewfile.txt");
//    print_block();
}

//checking the commands if valid or not 
//return 0:error, 1:malloc, 2:free, 3:print
int command_check(char *array[3]){
    if (! isdigit(*array[0])){
        printf("Invalid command number\n");
        return 0;
    }
    if ( ! strcmp( array[1],"malloc") ){
        if ( !isdigit(*array[2]) ) {
            printf("invalid size for malloc @ command %s\n", array[0]);
            return 0;
        }
        else{
            return 1;
        }
    }
    else if ( ! strcmp( array[1],"free" )){
        if ( !isdigit(*array[2]) ) {
            printf("invalid command for free @ command %s\n", array[0]);
            return 0;
        }
        else{
            return 2;
        }
    }
    else if ( ! strncmp( array[1],"print",1)){
        if( strcmp(array[2],"\n"  ) != 0 && strcmp(array[2],"\r\n") != 0 && strcmp(array[2],"\0"  ) != 0 && 1){
            printf("Invalid print command @ command %s\n", array[0]);
            return 0;
        }
        else{
            return 3;
        }
    }
    else{
        printf("invalid command at command %s\n",array[0]);
        return 0;
    }
}

//execute malloc and add value and pointer to the linked list
void command_exe_m(char *array[3]){
    void *p;
    p = malloc_block(atoi( array[2] ));
    if ( ! p ){
        printf("no available blocks of size %d at command %s\n", atoi( array[2] ), array[0] );
        return;
    }
    else{
        printf("malloc successfull at command # %d\n", atoi( array[0] ));
    }
    add_list(head2, atoi( array[0] ), p);
}

//execute free and remove the related entry from the linked list
int command_exe_f(char *array[3]){
    int temp = atoi (array[2]);    
    node_t* current = head2; 
    while (current != NULL)  
    {  
        if (current->val == temp){ 
            free_block(current->point);
            rem_list(head2,temp);
            return 1;
        }
        current = current->next;  
    }
    return 0;
}

//Add pointer and command number to the linked list
void add_list(node_t * head2, int val, void *point) {
    node_t * current = head2;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(node_t));
    current->next->val = val;
    current->next->point = point;
    current->next->next = NULL;
}

// removing pointer and command number from the linked list for the provided value(command number)
void rem_list(node_t *head2, int key) { 
    node_t* temp = head2, *prev; 
    if (temp != NULL && temp->val == key) { 
        head2 = temp->next; 
        free(temp);         
        return; 
    } 
    while (temp != NULL && temp->val != key) { 
        prev = temp; 
        temp = temp->next; 
    } 
    if (temp == NULL) return; 
    prev->next = temp->next; 
    free(temp);  
} 