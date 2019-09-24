/*************************************
 * Lab 2 Exercise 1
 * Name:
 * Student No:
 * Lab Group:
 *************************************
 Warning: Make sure your code works on
 lab machine (Linux on x86)
 *************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //for fork(), wait()
#include <string.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/wait.h>

void readFromFile (char who[50], int fd);
void readOneFile (char who[50], int fd);
void readFromBuffer(char who[50], FILE* fp);
void readOneBuffer(char who[50], FILE* fp);

int main(int argc, char **argv) {
    char* fileName = NULL;
    int nChild = 0;
    int fd = 0;
    char who[50];

    if (argc < 3)
    {
        printf ("Usage: %s <filename> <number_of_processes>\n", argv[0]);
        return 1;
    }

    fileName = argv[1];
    nChild = atoi(argv[2]);

    fd = open( fileName, O_RDONLY );

    sprintf(who, "Parent [%d]", getpid());
    int ppid = getpid();
    readOneFile(who, fd );

    for (int i=0; i<nChild; i++){
      if (fork() == 0){
        sprintf(who, "Child %d[%d]", i+1, getpid());
        break;
      }
    }
    readFromFile(who, fd);

    for (int i=0; i<nChild+1; i++){
      if (getpid() != ppid){
        char strBuf[128];
        sprintf(strBuf, "Parent: %s done.\n", who );
        write(STDOUT_FILENO, strBuf, strlen(strBuf));
        exit(0);
      }
    }

    for (int i=0; i<nChild; i++)
      wait(NULL);

    close (fd);
//-----------------------------------------------------------------
    FILE *fp;
    fp = fopen(fileName, "r");
    sprintf(who, "Parent [%d]", getpid());
    readOneBuffer(who, fp );

    for (int i=0; i<nChild; i++){
      if (fork() == 0){
        sprintf(who, "Child %d[%d]", i+1, getpid());
        break;
      }
    }
    readFromBuffer(who, fp);

    for (int i=0; i<nChild+1; i++){
      if (getpid() != ppid){
        char strBuf[128];
        sprintf(strBuf, "Parent: %s done.\n", who);
        fwrite(&strBuf, strlen(strBuf),1, stdout);
        exit(0);
      }
    }

    for (int i=0; i<nChild; i++)
      wait(NULL);


    printf("%s\n", "Parent: Exiting.");
    fclose (fp);

}

//----------------------------------------------------------
void readFromFile (char who[50], int fd)
{
	ssize_t readBytes = 1;
	char charBuf = 0;
  char strBuf[128];

	while (readBytes > 0) {
		usleep (1000);
		charBuf = 0;
		readBytes = read( fd, &charBuf, 1 );

		if( readBytes != 1 ){
			if( readBytes == 0 ){
				return;
			}
		}

    sprintf(strBuf, "%s: %c\n", who, charBuf);
	  write (STDOUT_FILENO, strBuf, strlen(strBuf));

	}

}

void readOneFile(char who[50], int fd){
  ssize_t readBytes = 1;
	char charBuf = 0;
  char strBuf[128];

		charBuf = 0;
		readBytes = read( fd, &charBuf, 1 );
		if( readBytes != 1 ){
			if( readBytes == 0 ){
				exit(0);
			}
		}
    sprintf(strBuf, "%s: %c\n", who, charBuf);
	  write (STDOUT_FILENO, strBuf, strlen(strBuf));

}

//--------------------------------------------------------------

void readFromBuffer(char who[50], FILE* fp){
  ssize_t readBytes = 1;
	char charBuf = 0;
  char strBuf[128];

	while (readBytes > 0) {
    usleep(1000);
		charBuf = 0;
		readBytes = fread(&charBuf, 1 , 1, fp);

		if( readBytes != 1 ){
			if( readBytes == 0 ){
				return;
			}
		}

    sprintf(strBuf, "%s: %c\n", who, charBuf);
	  fwrite(strBuf, strlen(strBuf),1, stdout);

	}

}

void readOneBuffer(char who[50], FILE* fp){
  ssize_t readBytes = 1;
	char charBuf = 0;
  char strBuf[128];

		charBuf = 0;
		readBytes = fread(&charBuf, 1 , 1, fp);
		if( readBytes != 1 ){
			if( readBytes == 0 ){
				exit(0);
			}
		}
    sprintf(strBuf, "%s: %c\n", who, charBuf);
	  fwrite(strBuf, strlen(strBuf),1, stdout);
}
