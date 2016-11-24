#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include "menu.h"
#include "unistd.h"
#include <dirent.h>
#include <sys/stat.h>
#include "pwd.h"

char* name;

void addAllUsers(){
  createDesktopFile("Ben");
}
void addToMenu(){
  char cwd[500];
  getcwd(cwd, 500);
  char configPath[400];
  strcpy(configPath, "/home/");
  strcat(configPath, name);
  strcat(configPath, "/.config/lxsession/Lubuntu/autostart");
  FILE *fp;
  fp = fopen(configPath, "a");

  fprintf(fp,"%s/share \n", getcwd(cwd,500));
  fclose(fp);

}
void createUserFile(){
  
   char filePath[300];
   strcpy(filePath, "/home/");
   strcat(filePath, name);
   strcat(filePath, "/Documents/COMP3000/info.txt");
   FILE *fp;
   fp = fopen(filePath, "a");
   fprintf(fp, "username:?;password:?;\n<users>\nsamplename:\n</users>");
   fclose(fp);
}

int createFile(){
   char location[150];
   strcpy(location, "/home/");
   strcat(location, name);
   strcat(location, "/Documents/COMP3000");
 
   DIR* dir = opendir(location);
   if(dir){
        createUserFile();
   }
   else if(ENOENT == errno){
	int result = mkdir(location, 0777);
	if(result < 0){
		printf("%s\n", strerror(errno));
	}
	createUserFile(); 
   }
   else{
   	printf("Permission denied\n");
	return -1;
   }
	return 0;
}

int listen(){
  // is it possible to listen to network connections
  // AND to listen to changes to a file?
  printf("Listening here. Function to create background process is here");
}

int main ()
{
  //get username in order to construct home dir
  struct passwd *p = getpwuid(getuid());
  name = p->pw_name;
  // check for domain file's existance
  // create if none exists
  char filePath[500];
  strcpy(filePath, "/home/");
  strcat(filePath, name);
  strcat(filePath, "/Documents/COMP3000/info.txt");
  if(access(filePath, F_OK) != -1){
        printf("%s\n", "File exists. Read contents.");
        // port cloud credentials
        //manage the desktop files for all users.
        // create a background process and exit.
  }
  else{
        createFile();
	// add to startup menu
        addToMenu();
  }
  return 0;
}

