#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>
#include <boost/algorithm/string/predicate.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/wait.h>

using namespace std;
string cmd;
string arg;
string command;
fstream history;
string parameter;
string wholeparameter;
string redirection;
string filename;
string historyinfo;
string historydir;
string historypath;

void signal_callback_handler(int signum){
   	history.close();
	cout << endl << "CTRL C has been pressed, exiting." << endl;
	cout << "Here is the history of the commands you have entered: " << endl;
	string readfile = "cat " + historypath;
      	const char* historyfileread = readfile.c_str();
      	system(historyfileread);
   	exit(signum);
}

int main(){
	signal(SIGINT, signal_callback_handler);
	while(1){
		cout << "Brandons-Shell" << get_current_dir_name() << ":~ ";
		ofstream history("history.txt");
		historydir = get_current_dir_name();
		historypath = historydir + "/history.txt";
		getline(cin, cmd);
		stringstream ssobject(cmd);
		ssobject >> command;
		while(ssobject >> parameter){
			if(parameter != ">"){
				wholeparameter.append(parameter);
				wholeparameter.append(" ");
			}
			else{
				redirection = parameter;
				break;
			}
		}
		ssobject >> filename;
		history << cmd << endl;
		while(command != "quit"){
			if(command == "myprocess"){
				cout << "Process ID is " << getpid() << endl;
			}
			else if(command == "allprocesses"){
				system("ps");
			}
			else if(command == "clr"){
				system("clear");
			}
			else if(command == "chgd"){
				if(parameter != ""){
					const char* dir = parameter.c_str();
					struct stat sb;
					if (stat(dir, &sb) == 0){
					chdir(dir);
					cout << "directory has been changed to " <<  get_current_dir_name() << endl;
					}
					else{
						cout << "directory does not exist" << endl;
					}
				}
				else{
					cout << "You must enter another argument for the directory" << endl;
				}
			}
			else if(command == "dir"){
				if(parameter != ""){
					const char* dir = parameter.c_str();
					struct stat sb;
					string find = "ls -al ";
					find.append(parameter);
					const char* list = find.c_str();
                                	if (stat(dir, &sb) == 0){
                                	system(list);
                                	}
                                	else{
                                		cout << "directory does not exist" << endl;
                                	}
				}
				else{
                                	cout << "You must enter another argument for the directory" << endl;
                        	}
			}
			else if(command == "environ"){
				system("env");
			}
			else if(command == "repeat"){
				if(parameter != ""){
					if(redirection == ">"){
						if((filename != "") && (boost::ends_with(filename, ".txt"))){
							fstream inputfile;
							inputfile.open(filename, ios::out);
							inputfile << wholeparameter << endl;
							inputfile.close();
						}
						else{
							cout << "filename needs to end with .txt" << endl;
							filename = "";
							redirection = "";
							wholeparameter = "";
						}
					}
					else{
						cout << wholeparameter << endl;
						wholeparameter = "";
					}
                        	}
                        	else{
                                	cout << "You must enter another argument for the string you want to repeat" << endl;
                        	}
			}
			else if(command =="hiMom"){
					int fd[2];
					pipe(fd);
					pid_t cpid = fork();
        				if (cpid == 0){
            					close(fd[0]);
            					char childmsg[] = "Hi Mom!";
            					write(fd[1], childmsg, sizeof(childmsg));
            					close(fd[1]);
            					exit(0);
        				}
        				else{
            					close(fd[1]);
						char msgfromchild[100];
        					read(fd[0], msgfromchild, sizeof(msgfromchild));
              					cout << "Message recieved from child: " << msgfromchild << endl;
						close(fd[0]);
						wait(NULL);
				}
			}
			else if(command == "help"){
				cout << "Available commands" << endl;
				cout << "1. myprocess - displays the current process id" << endl;
				cout << "2. allprocesses - displays the process id of all processes that are ongoing" << endl;
				cout << "3. chgd <directory> - changes the current directory to the given directory" << endl;
				cout << "4. clr - clears the screen" << endl;
				cout << "5. dir <directory> - lists the contents of the given directory" << endl;
				cout << "6. environ - lists all the environmental strings" << endl;
				cout << "7. quit- Quits the shell" << endl;
				cout << "8. repeat <string> - outputs a string to the console can also be used to send a string to a file" << endl;
				cout << "9. hiMom - sends a message from child to parent through pipes" << endl;
				cout << "10. help - displays the user manual" << endl;
			}
			else{
				cout << "invalid command" << endl;
			}
                	cout << "Brandons-Shell" << get_current_dir_name() << ":~ ";
			getline(cin, cmd);
			stringstream ssobject(cmd);
        		ssobject >> command;
        		while(ssobject >> parameter){
                		if(parameter != ">"){
                        		wholeparameter.append(parameter);
                        		wholeparameter.append(" ");
                		}
                		else{
                        		redirection = parameter;
                        		break;
                		}
        		}
	        	ssobject >> filename;
			history << cmd << endl;
		}
		if (command == "quit"){
			history.close();
			cout <<  "Here is the history of the commands you have entered: " << endl;
			string readfile = "cat " + historypath;
			const char* historyfileread = readfile.c_str();
			system(historyfileread);
			exit(0);
		}
	}
	return EXIT_SUCCESS;
}
