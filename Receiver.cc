#include "Receiver.h"

//This reads the incoming IP6 Address and checks it to the userlist file that the server stores in tmp folder
bool Receiver::authenticateConnection(string clientIP){
    std::ifstream usersAndIP("./Comp3000HTTPServer/tmp/usersList.txt");
	string userName, ip;
	while(usersAndIP >> userName >> ip){ 
		cout<<"File USRNME: " << userName <<" IP: " <<ip << endl;
		if(clientIP == ip) {
			cout<<"User sending File: " << userName << " From the IP Address: " << ip << endl;
			return true;
		}
	}
	error("BAD CONNECT ATTEMP FROM: ");
	cout<<clientIP <<endl;
	return false;
}
//display error in console
void Receiver::error(string msg)
{
    cout<< msg <<endl;
}
//open on thread to prevent blocking? This will run forever once started
int Receiver::openConnectionListener(){
     //prevents zombie processes from the children prcess created
	 signal(SIGCHLD,SIG_IGN);
	//creates a listening signal
	 int sockfd, newsockfd, portno, sockfdv6;
     socklen_t clilen;
     char buffer[256];
     sockaddr_in6 recevierAddress, cli_addr;
     string val = std::to_string(5000);
	 const char * port =  val.c_str();
	 
	cout<<"Staring Listening for incoming request"<<endl;
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &recevierAddress, sizeof(recevierAddress));
     portno = atoi(port);
     recevierAddress.sin6_family = AF_INET6;
     recevierAddress.sin6_addr = in6addr_any;
     recevierAddress.sin6_port = htons(portno);
     
	 if (bind(sockfd, (sockaddr *) &recevierAddress,
              sizeof(recevierAddress)) < 0) 
              error("ERROR on binding");
	 cout<<"Listening for incoming request on"<< sockfd<<endl;
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
	 //waits for incoming connections and creates a child process to deal with it
	 while(1){
		 
		 newsockfd = accept(sockfd, 
                 (sockaddr *) &cli_addr, 
                 &clilen);
		if (newsockfd < 0) 
			error("ERROR on accept");
        //refuse connection if clientIP is bad 		
		char clientIP[INET6_ADDRSTRLEN];
		inet_ntop(AF_INET6,&cli_addr.sin6_addr,clientIP,INET6_ADDRSTRLEN) ;
		if(!authenticateConnection(clientIP)){
		  pid_t child_pid = fork();
			if(child_pid < 0)
				error("Failed to fork");
			else if(child_pid == 0){
				close(sockfd);
				exit(0);
			}
			else close(newsockfd);
		}//otherwise proceed as normal creats chlid that exits once done and 
		else{ 
		    pid_t child_pid = fork();
			if(child_pid < 0)
				error("Failed to fork");
			else if(child_pid == 0){
				close(sockfd); 
				writeToFile(newsockfd);
				exit(0);
			}
			else close(newsockfd);
		}
     }
	 close(sockfd);
	 return 0; 
}
//processes the connection
int Receiver::writeToFile(int socket){
		int bufferSize = 8192;
	    char buffer[bufferSize];
		bzero(buffer,bufferSize);
		string val = "./";
		int n = read(socket,buffer,bufferSize-1);
		int x = 0;
		 //get file name from first msg on socket
		while(x <=bufferSize-1){
		  if(buffer[x] == '\0' || buffer[x] == '\n')break;
		  cout<<buffer[x];
		  val+=buffer[x];
		  x++;
		}
		if(n < 0) error("ERROR reading from socket");
		cout <<endl << "File Name" << val <<endl;
		n = send(socket,"File name received",17,0);
		if (n < 0) error("ERROR writing to socket");
		char largeBuf[65535];
		bzero(largeBuf,65535);
		const char * fileName =  val.c_str();
		//create a new file the all read write privliges
	    FILE * file =  fopen(fileName, "w+");
		//reads the socket now for the the incoming file transfer and writes it to the newly created file
		while(n = read(socket,largeBuf,65534))
			fwrite(largeBuf,1,n,file);
		if (n < 0) error("ERROR reading from socket");
		fclose(file);
		//sends message to client letting then know transfer is complete and closes the socket
		n =send(socket,"Completed File Transfer",18,0);
		if (n < 0) error("ERROR writing to socket");
		
}

