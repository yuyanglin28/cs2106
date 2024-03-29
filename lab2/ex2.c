/*************************************
* Lab 2 Exercise 2
* Name: Lin Yuyang
* Student No: A0207526H
* Lab Group: 09
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
#include <unistd.h>

#define MAX_ARGS 10
#define MAX_WORD_LEN 19

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);
int checkPath(char * cmd);
void isQuit(char** cmd, int num);


int main ()
{

  int oneCmdSize = MAX_WORD_LEN*(MAX_ARGS+1);

  char input[oneCmdSize];

  while (1){
    printf("%s","GENIE > " );
    fgets(input, oneCmdSize, stdin);


      int num;
      char** cmdFull = readTokens(MAX_ARGS+1, MAX_WORD_LEN, &num, input);

      isQuit(cmdFull, num);

      if (checkPath(cmdFull[0]) == 0){
        printf("%s not found\n", cmdFull[0]);
        continue;
      }else {
        if (fork() == 0){
          execvp(cmdFull[0], &cmdFull[0]);
          exit(0);
        }else{
          wait(NULL);
        }
      }
      freeTokenArray(cmdFull, num);
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


void isQuit(char** cmd, int num){

  char quit[] = "quit";

  if (num==1 && strcmp(cmd[0], quit)==0){
    printf("%s\n", "Goodbye!");
    exit(0);
  }

}


int checkPath(char * cmd){

  struct stat cStat;
  char path[strlen("/bin/")+MAX_WORD_LEN];

  if (stat(cmd, &cStat)<0){

    if (cmd[0] != '/'){
      strcpy(path, "/bin/");
      strcat(path, cmd);
    }
  }else{
    strcpy(path, cmd);
  }

  strcpy(cmd, path);

  if (stat(path, &cStat) < 0)
    return 0;

  return 1;

}
