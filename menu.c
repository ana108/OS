#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
const char* defaultPath = "/usr/share/applications/";

void createDesktopFile(char* name){
// create an empty file
// populate the file with relevant data
// restart desktop
// /usr/share/applications
  char* menuItem = "COMP3000";
  int totalSize = sizeof(menuItem) + sizeof(name);
  char desktopFileName[totalSize+5];
  
  strcpy(desktopFileName, menuItem);
  strcat(desktopFileName, name);
  strcat(desktopFileName, ".desktop");
  FILE * fp;
  char* line = NULL;
  char wholeString[2500];
  char templateLines[6][2500];
  int pointer = 0;
  fp = fopen("template.desktop", "r");
  if (fp == NULL)
      printf("Could not read template.desktop");

  while(fgets(wholeString, 2499, fp) != NULL) {
      strcpy(templateLines[pointer], wholeString); 
      if(templateLines[pointer][strlen(templateLines[pointer])-1] == '\n'){
         templateLines[pointer][strlen(templateLines[pointer])-1] = '\0';
	}
      pointer++;
  }


  char myDir[1024];
  if(getcwd(myDir, sizeof(myDir)) != NULL){
     strcat(myDir, "/share");
     printf("Current Directory is: %s\n", myDir);
  }

  templateLines[1][strlen(templateLines[1])-2] = '\0';
  strcat(templateLines[1],"Ana");

  templateLines[2][strlen(templateLines[2])-2] = '\0';
  strcat(templateLines[2], myDir);
 
  templateLines[3][strlen(templateLines[3])-2] = '\0';
  strcat(templateLines[3],"cloud.png");

  fclose(fp);
  // write to file the contents of the array
  char desktopPath[2024];
  strcpy(desktopPath, defaultPath);
  strcat(desktopPath, desktopFileName);
  FILE * fpnew;
  fpnew  = fopen(desktopPath, "w");
  if(fpnew == NULL){
    printf("%s\n", strerror(errno));
    printf("%s\n", "Could not write to file");
  }
  for(int i = 0; i < 6; i++){
      printf("%d %s\n",i, templateLines[i]);
     if(i != 5){
        strcat(templateLines[i], "\n");
     }
      fprintf(fpnew, "%s", templateLines[i]); 
  }

  fclose(fpnew);
}
// delete all desktop files or per name?
void deleteDesktopFile(){

}
// main program will get placed in a different file
int main ()
{
  createDesktopFile("Ana");
  return 0;
}
