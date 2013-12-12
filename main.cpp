//
//  main.cpp
//  JMiniShell
//
//  Created by Jereneal Kim on 13. 10. 13..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <iostream>
#include <vector>

#include <sys/types.h>
#include <sys/wait.h>
#include <wordexp.h>
#include <signal.h>
#include <fcntl.h>

#include "AliasSet.h"
#include "VariableSet.h"
#include "CmdLineSeparator.h"
#include "Jobs.h"

using namespace std;

int envVariableSetting(string checkStr){
	//setenv
	char setEnvArr[200]={0};
	checkStr.copy(setEnvArr,checkStr.length());

	if(checkStr.length()>=6&&checkStr.compare(0,7,"setenv ")==0){
		char* envTok = strtok(setEnvArr, " ");
		envTok = strtok(NULL, " ");
		char* envTok2 = strtok(NULL, " ");
		if(envTok2[0]=='='){
			printf("invalid operator =\n");
			return -1;
		}
		setenv(envTok, envTok2, 1);
		return 1;
	}
	
	//unsetenv
	if(checkStr.length()>=8&&checkStr.compare(0,9,"unsetenv ")==0){
		char* envTok = strtok(setEnvArr, " ");
		envTok = strtok(NULL, " ");
		unsetenv(envTok);
		return 0;
	}
	
	return -1;//set anything
}
void sigint_handler(int signo){
	printf("\n");
}

int main()
{
	pid_t pid;
	char linestring[1000];
	int lineStrNum = 0;
	AliasSet alSet = AliasSet();
	VariableSet varSet = VariableSet();
	CmdLineSeparator lineSplit = CmdLineSeparator();
	vector<Jobs> jobVector = vector<Jobs>();
	int cmdJobNum=1;
	int pipes_child[2];
	signal(SIGINT, sigint_handler);
	

	while(1) {
		lineSplit.containBackgroundMark = false;
		lineSplit.redirectionInputMark = false;
		if(lineSplit.pipeInputMark==true){
			lineSplit.pipeInputMark=false;
			lineSplit.pipeOutputMark=true;
		}
		
		if(lineSplit.splitIdx==lineStrNum){
			char curPath[256]={0};
			getcwd(curPath, 256);
			printf("%s>",curPath);
			memset(linestring,'\0',1000);
			lineSplit.splitIdx = 0;
			for(lineStrNum=0;lineStrNum<1000;lineStrNum++){
				scanf("%c",&linestring[lineStrNum]);
				if(linestring[lineStrNum]=='\0'){
					lineStrNum--;
					continue;
				}
				if(linestring[lineStrNum]=='\n'){
					break;
				}
			}
		}

		int retIdx = lineSplit.splitOneStatement(linestring);
		if(retIdx==-1){
			//Incorrect
			printf("Invalid statement, check the \',\",;,etc\n");
			continue;
		}else if(retIdx==0){
			//in case of enter
			continue;
		}
		if(lineSplit.pipeInputMark==true){
			if (pipe(pipes_child)==-1){
				perror("파이프 생성 실패");
				exit(1);
			}
		}

		string strPtrStr = lineSplit.cmdString;

		if(envVariableSetting(strPtrStr)!=-1){
			continue; //set or unset
		}
		
		//handling shell variables, if var has '', then add here
		//if(checkStr.length()>=8&&checkStr.compare(0,4,"set ")==0){
		string newStr;
		//replace variables
		int status = varSet.replaceVariables(newStr,strPtrStr);
		if(status==-1){
			continue;//undefined var
		}
		string checkStr = string(strPtrStr);
		strPtrStr = newStr;

		if(checkStr.compare(0,6,"alias ")!=0){
			string newStr2;
			int cmdIdx;
			int alValid = alSet.findCmd(checkStr, cmdIdx);
			if(alValid!=-1){
				newStr2 = alSet.replaceCmd(checkStr,cmdIdx);
				strPtrStr = newStr2;
				printf("%lu\n",strPtrStr.length());
				if(strPtrStr[strPtrStr.length()]!='\0'){
					strPtrStr+='\0';
				}
			}
		}
		
		//before ' " tokenizing
		
		wordexp_t result;
		wordexp(strPtrStr.c_str(), &result, 0);
//		char* tat = result.we_wordv[1];
		
		
		//after ' " tokenizing
		if(strcmp(result.we_wordv[0],"cd")==0){
			chdir(result.we_wordv[1]);
			continue;
		}
		if(strcmp(result.we_wordv[0],"set")==0){
			varSet.setNewCommand(result);
			continue;
		}
		if(strcmp(result.we_wordv[0],"alias")==0){
			alSet.setNewCommand(result);
			continue;
		}
		if(strcmp(result.we_wordv[0],"exit")==0){
			printf("Exit the JMiniShell\n");
			return 0;
		}
		if(strcmp(result.we_wordv[0],"jobs")==0){
			int childPID,childStatus;
			
			for(int iter=0;iter<jobVector.size();iter++){
				childPID = waitpid(jobVector[iter].getPid(), &childStatus, WNOHANG);
				//delete this child
				if(childPID!=0){
					jobVector[iter].updateStatus(childPID);
				}
				jobVector[iter].printCurrentJob("");
			}
			continue;
		}
		if(strcmp(result.we_wordv[0],"kill")==0){
			for(int iter=0;iter<jobVector.size();iter++){
				string jobNum = result.we_wordv[1];
				jobNum.erase(0);

				if(to_string(jobVector[iter].getJobNum()).compare(jobNum)){
					kill(jobVector[iter].getPid(),SIGKILL);
					jobVector[iter].killProcess();
				}
			}
			continue;
		}
		
		
		for(int iter=0;iter<result.we_wordc;iter++){
			for(int iter2=0;result.we_wordv[iter][iter2]!='\0';iter2++){
				if(result.we_wordv[iter][iter2]=='\n'){
					result.we_wordv[iter][iter2]='$';
				}
			}
		}
		if(lineSplit.redirectionInputMark==true){
			while(1){
				int retIdxa = lineSplit.splitOneStatement(linestring);
				if(retIdxa == 0){
					continue;
				}else{
					break;
				}
			}
		}
		/* fork a child process */
		//strptr is needed
		pid = fork();
		
//		printf("%d\n",getpid());

		
		if (pid == 0) { /* child process */
			signal(SIGINT, SIG_DFL);
			int check;
			if(lineSplit.redirectionInputMark==true){
				int file = open(lineSplit.cmdString.c_str(), O_WRONLY | O_CREAT,0644);
				if(file < 0) printf("Couldn't open the file.\n");
				if(dup2(file,1) < 0) printf("Couldn't dup2 to the file 3.\n");
			}else if(lineSplit.pipeInputMark==true){
				if(dup2(pipes_child[1],1) < 0) printf("Couldn't dup2 to the file 1.\n");
				close(pipes_child[0]);
				close(pipes_child[1]);
			}
			//pipe로 결과 받아오기.
			if(lineSplit.pipeOutputMark==true){
				if(dup2(pipes_child[0],0) < 0) printf("Couldn't dup2 to the file 2.\n");
				close(pipes_child[0]);
				close(pipes_child[1]);
				lineSplit.pipeOutputMark = false;
			}

			
			check = execvp(result.we_wordv[0],result.we_wordv);
			if(check==-1){
				printf("-JMiniShell: %s: command not found\n",result.we_wordv[0]);
			}
			
		} else { /* parent process */
			/* parent will wait for the child to complete */
			signal(SIGINT, sigint_handler);
			int childStatus,childPID;

			if(lineSplit.containBackgroundMark==true){
				jobVector.push_back(Jobs(cmdJobNum++,strPtrStr,pid));
				
			}else{
				if(lineSplit.pipeOutputMark==true){
					close(pipes_child[0]);
					close(pipes_child[1]);
					lineSplit.pipeOutputMark=false;
				}
				childPID=waitpid(pid, &childStatus, 0);
				for(int iter=0;iter<jobVector.size();iter++){
					childPID = waitpid(jobVector[iter].getPid(), &childStatus, WNOHANG);
					//delete this child
					if(childPID!=0){
						jobVector[iter].updateStatus(childPID);
						jobVector[iter].printCurrentJob("+");
					}
				}
			}
		}
	}
}