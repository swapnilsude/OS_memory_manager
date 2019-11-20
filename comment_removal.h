#include <stdio.h>
#include <stdlib.h>

// file in which we check for comments and file after removing comments
FILE *fp , *fp2;

// function that handles block comments
void comment_backslash_star(){
char d,e;
    while((d=fgetc(fp))!=EOF){   // comment started do nothing until end of comment is found
        if(d=='*'){    
            e=fgetc(fp);  
            if(e=='/')
                return;
        }
    }
}

// function that handles single line comments
void comment_double_backslash(){
char d,e;
    while((d=fgetc(fp))!=EOF){ // comment started do nothing until newline
        if(d=='\n')
            return;
    }
}

void comment( char file_name[] )
{
    char c;
    fp = fopen (file_name,"r");           // open in read mode
    if (fp == NULL)
        exit(EXIT_FAILURE);
    fp2 = fopen ("mynewfile.txt","w");     // open in write mode
    if (fp2 == NULL)
        exit(EXIT_FAILURE);

    while((c=fgetc(fp))!=EOF){   // read the file character by character and check for comment
        char d;
        if( c == '/'){
            if((d=fgetc(fp))=='*')
                comment_backslash_star();
            else if( d == '/'){         
                comment_double_backslash();
            }
            else{
                // if not a comment add characters in the new file
                fputc(c,fp2);
                fputc(d,fp2);
            }
        }
        // add the character in the new file.
        else
            fputc(c,fp2);
    }
    // close both the files
    fclose(fp);
    fputc(' ',fp2);
    fputc('\n',fp2);
    fclose(fp2);
}

//for removing extra whitespaces from the line
void remove_extra_whitespaces(char* input, char* output){
    int i = 0;
    int j = 0;
    while(input[i] != '\0'){
        output[j] = input[i];
        if(input[i] == ' '){
            // skip extra spaces
            while(input[i + 1] == ' ') {
                i++;
            }
        }
        j++;
        i++;
    }
    // null-terminate output
    output[j] = '\0';
}