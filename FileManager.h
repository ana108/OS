#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
using namespace std;

class FileManager
{
	public:
	  void launch();
	  bool fileExsist(string*);
	 // bool writeUserToFile(string* , string*);
	 private:
	 static void sigusr2_handler(int);
	 int cpid;
};
#endif