#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <sys/times.h>


using namespace std;

struct Record{
  string name;
  double timestamp;
};

void show_history(vector<Record> command_history);
void show_sort_history(vector<Record> command_history);
bool comp(string input, string command);
bool comp2(string input, string command1, string command2);
bool operator< (const Record& a, const Record& b){return a.timestamp < b.timestamp;}

int main(){
  cout << "tinyshell>";//prompt
  string command;
  clock_t start, finish;
  double timestamp;

  vector<Record> command_history;
  Record new_command;
  pid_t pid;

  while(true){
    getline(cin,command);
    if (comp(command, "exit"))
      exit(0);
      //break;//for gdb
    struct tms time1, time2;
    //start=times(&time1);
    start=clock();
    pid=fork();
    if (pid==0){ // the child process
      if (comp(command, "history"))
        show_history(command_history);
      else if (comp2(command, "history ","-sbu"))
        show_sort_history(command_history);
      else
        system(command.c_str());
      exit(0);//exit this child process
    }
    else{ // the parent process
        waitpid(pid, NULL, 0);//when the child process completed //block parent process
        //wait(NULL);
        //finish = times(&time2);
        finish = clock();
        timestamp = (double)(finish-start)/CLOCKS_PER_SEC;
        //long double clktck=sysconf(_SC_CLK_TCK);
        //timestamp = (finish-start)/(double)clktck;
        new_command.name = command;
        new_command.timestamp = timestamp;
        command_history.push_back(new_command);
    }
  }
  //cout << "for gdb test"<<endl;
}

void show_history(vector<Record> command_history){
  vector<Record>::iterator itr;
  if (command_history.size()>=6){
    for (itr=command_history.end()-1; itr!=command_history.end()-6;itr--)
      cout << (*itr).name<< "  "<< (*itr).timestamp <<"s"<<endl;
  }
  else if (command_history.size()>=1){
    for (itr=command_history.end()-1; itr!=command_history.begin(); itr--)
      cout << (*itr).name<< "  "<< (*itr).timestamp <<"s"<<endl;
    cout << (*command_history.begin()).name<< "  "<< (*command_history.begin()).timestamp <<"s"<<endl;
  }
}

void show_sort_history(vector<Record> command_history){
  vector<Record> command_history_copy;
  //command_history_copy.insert(command_history_copy.begin(), command_history.end()-5, command_history.end());
  command_history_copy.insert(command_history_copy.begin(), command_history.begin(), command_history.end());
  sort (command_history_copy.begin(), command_history_copy.end());
  show_history(command_history_copy);
}

bool comp(string input, string command){
  int len_input = input.length();
  int len_command = command.length();
  for (int i=0; i<(len_input-len_command+1); i++){
    if (input[i]==' ')  continue; //don't care initial spaces
    if (input.substr(i, len_command)!=command)  return false;
    if (input.substr(i, len_command)==command){
      for (int j=i+len_command; j<len_input; j++){ //after the substring should be spaces
        if (input[j]!=' ')  return false;
      }
      return true;
    }
  }
  return false;
}

bool comp2(string input, string command1, string command2){
  int len_input = input.length();
  int len_command1 = command1.length();
  int len_command2 = command2.length();
  for (int i=0; i<(len_input-len_command1-len_command2+1); i++){
    if (input[i]==' ')  continue; //don't care initial spaces
    if (input.substr(i, len_command1)!=command1)  return false;
    if (input.substr(i, len_command1)==command1) //remaining substring should satisfies comp()
      return comp(input.substr(i+len_command1), command2);
  }
  return false;
}
