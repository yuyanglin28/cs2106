/*************************************
 * Lab 2 Exercise 2
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/

#include <stdio.h>
#include <fcntl.h>      //For stat()
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>   //for waitpid()
#include <unistd.h>     //for fork(), wait()
#include <string.h>     //for string comparison etc
#include <stdlib.h>     //for malloc()

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);

int main() {

    int max_argument = 10;
    int max_length = 19;
    char commandLine[max_argument*(max_length+1)];

  while (1){

    printf("%s","GENIE > " );
    scanf("%s", commandLine);
    int num;
    char ** commandFull = readTokens(max_argument+1, max_length, &num, commandLine);

    char* command = commandFull[0];
    char quit[] = "quit";

    if (num==1 && strcmp(command, quit)==0){
      printf("%s\n", "Goodbye!");
      exit(0);
    }else{

      if (fork() == 0){
        system(commandLine);
        exit(0);
      }else {
        wait(NULL);
      }

    }
  }


}






char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer)
//Tokenize buffer
//Assumptions:
//  - the tokens are separated by " " and ended by newline
//Return: Tokenized substrings as array of strings
//        readTokenNum stores the total number of tokens
//Note:
//  - should use the freeTokenArray to free memory after use!
{
    char** tokenStrArr;
    char* token;
    int i;

    //allocate token array, each element is a char*
    tokenStrArr = (char**) malloc(sizeof(char*) * maxTokenNum);

    //Nullify all entries
    for (int i = 0; i < maxTokenNum; i++) {
        tokenStrArr[i] = NULL;
    }

    token = strtok(buffer, " \n");

    i = 0;
    while (i < maxTokenNum && (token != NULL)) {
         //Allocate space for token string
        tokenStrArr[i] = (char*) malloc(sizeof(char*) * maxTokenSize);

        //Ensure at most 19 + null characters are copied
        strncpy(tokenStrArr[i], token, maxTokenSize - 1);

        //Add NULL terminator in the worst case
        tokenStrArr[i][maxTokenSize-1] = '\0';

        i++;
        token = strtok(NULL, " \n");
    }

    *readTokenNum = i;

    return tokenStrArr;
}

void freeTokenArray(char** tokenStrArr, int size) {
    int i = 0;

    //Free every string stored in tokenStrArr
    for (i = 0; i < size; i++) {
        if (tokenStrArr[i] != NULL) {
            free(tokenStrArr[i]);
            tokenStrArr[i] = NULL;
        }
    }
    //Free entire tokenStrArr
    free(tokenStrArr);

    //Note: Caller still need to set the strArr parameter to NULL
    //      afterwards
}
