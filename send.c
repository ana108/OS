#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "FileManager.h"

int sendFile();
int main (int argc, char* argv[])
{
  printf("The number of arguments passed are: %d\n",argc);
  for(int i = 1; i < argc; i++){
    printf("%d argument is: %s\n", i, argv[i]);
  }
  char st[10];
  scanf("%s",st);
  printf("Result: %s\n",st);
  sendFile();
  return 0;
}
int sendFile(){

	//initilize server
	FileManager file;
    file.launch();
	printf("Server was launched...");
}
