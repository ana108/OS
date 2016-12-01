#include "FileManager.h"

static volatile sig_atomic_t sig_cault = 0;
void FileManager::launch(){
	 pid_t child_pid = fork();
	 cpid = child_pid;
	//get parent process id and send to server
	string val = std::to_string(getppid());
	const char * ppid =  val.c_str();
	if(child_pid == -1) 
		cout<<"Failed to launch Server!";
	else if(child_pid == 0){
      	cout<<"\n\nServer Launching..." <<endl;
		 char buf[1024];
		int siz = readlink("/proc/self/exe", buf, 1024);
		buf[siz-4] = '\0'; //send exe file is size 4
		string pathName = buf;
		strcat(buf, "Comp3000HTTPServer/server.js");
		execlp("node", "node",buf, ppid, NULL);
		fprintf(stderr,"failded %s\n",strerror(errno));
      	cout<<"\n\n\nLaunching failed" <<endl;
	}
    else{
		cout<<"Waiting for server response" <<endl ;
		signal(SIGUSR2,sigusr2_handler);
		signal(SIGCONT,sigusr2_handler);
		pause();
		return;
	}
}
bool FileManager::fileExsist(string* path){
  //ifstream file(path);
//  return file;
return false;
}
//bool FileManager::writeToFile(string* path


void FileManager::sigusr2_handler(int signal_number){
	if(signal_number == SIGCONT)
	  cout<<"server is ready"<<endl;
    else if(signal_number == SIGUSR2) {
		wait(NULL);
		//make a sleeping thread that will try again ?
	    cout<<"Need to restart server"<<endl;
	}
	return;
}