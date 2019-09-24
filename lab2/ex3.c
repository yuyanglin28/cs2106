/*************************************
 * Lab 2 Exercise 3
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
#define MAX_CMDS 10

char** readTokens(int maxTokenNum, int maxTokenSize, int* readTokenNum, char* buffer);
void freeTokenArray(char** strArr, int size);
char** splitCommandChain(int maxArgs, int maxOneCmdSize, int* chainNum, char* input);
void exeOneCmd (int in, int out, char** cmd);
int pipeExe(int chainNum, char** cmdChain);
int checkPath(char * cmd);
void isQuit(char** cmd, int num);


int main ()
{

  int oneCmdSize = MAX_WORD_LEN*(MAX_ARGS+1);
  int cmdChainSize = (oneCmdSize+2)*MAX_CMDS;
  char input[cmdChainSize];

  while (1){
    printf("%s","GENIE > " );
    fgets(input, cmdChainSize, stdin);

    int chainNum;
    char ** cmdChain = (char**) malloc(sizeof(char*) * MAX_CMDS);
    cmdChain = splitCommandChain(MAX_CMDS, oneCmdSize, &chainNum, input);

    if (chainNum == 1){

      int num;
      char** cmdFull = readTokens(MAX_ARGS+1, MAX_WORD_LEN, &num, cmdChain[0]);

      isQuit(cmdFull, num);

      if (checkPath(cmdFull[0]) == 0){
        printf("%s not found\n", cmdFull[0]);
        continue;
      }else {
        if (fork() == 0){
          execv(cmdFull[0], &cmdFull[0]);
          exit(0);
        }else{
          wait(NULL);
        }
      }
    }else if (chainNum > 1){
      pipeExe(chainNum, cmdChain);
    }

    freeTokenArray(cmdChain, chainNum);
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

char** splitCommandChain(int maxArgs, int maxOneCmdSize, int* chainNum, char* input){

  char** result;
  char* token;
  int i;
  result = (char**) malloc(sizeof(char*) * MAX_CMDS);

  for (int i = 0; i < MAX_CMDS; i++) {
      result[i] = NULL;
  }
  token = strtok(input, "|");

  i = 0;
  while (i < MAX_CMDS && (token != NULL)) {
      result[i] = (char*) malloc(sizeof(char*) * maxOneCmdSize);
      strncpy(result[i], token,  maxOneCmdSize-1);
      result[i][maxOneCmdSize-1] = '\0';

      i++;
      token = strtok(NULL, "|");
  }
  *chainNum = i;

  return result;
}

void exeOneCmd (int in, int out, char** cmd)
{
  if (fork() == 0){

      if (in != 0){
          dup2 (in, 0);
          close (in);
      }

      if (out != 1){
          dup2 (out, 1);
          close (out);
      }

      execvp (cmd[0], &cmd[0]);
  }else {
    wait(NULL);
  }

}


void isQuit(char** cmd, int num){

  char quit[] = "quit";

  if (num==1 && strcmp(cmd[0], quit)==0){
    printf("%s\n", "Goodbye!");
    exit(0);
  }

}

int pipeExe(int chainNum, char** cmdChain){
  int i;
  int in, fd [2];
  int num;

  in = 0;

  for (i = 0; i < chainNum - 1; ++i)
    {

      char ** oneCmd = readTokens(MAX_ARGS+1, MAX_WORD_LEN, &num, cmdChain[i]);
      if (checkPath(oneCmd[0]) == 0){
        printf("%s not found\n", oneCmd[0]);
        close(fd[1]);
        close(fd[0]);
        return -1;
      }

      pipe (fd);

      exeOneCmd(in, fd [1], oneCmd);

      close (fd [1]);

      in = fd [0];

      freeTokenArray(oneCmd, num);

    }


  char ** lastCmd = readTokens(MAX_ARGS+1, MAX_WORD_LEN, &num, cmdChain[i]);

   if (fork() == 0){
     close(0);
     close(fd[1]);
     dup2 (in, 0);
      execvp(lastCmd[0], &lastCmd[0]);
    }
    else{
      close(fd[0]);
      close(fd[1]);
      wait(NULL);
    }

    return 1;
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
