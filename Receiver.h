#ifndef RECEIVER_H
#define RECIEVER_H
#include <sys/socket.h>
#include <sys/types.h>
#include<netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdio.h>

#include <iostream>
using namespace std;
class Receiver
{
  public:
  void error(string);
    bool authenticateConnection(string);
	int openConnectionListener();
  private:
    int writeToFile(int);
};
#endif