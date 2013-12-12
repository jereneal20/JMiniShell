//
//  Jobs.h
//  MiniShell2
//
//  Created by Jereneal Kim on 2013. 12. 5..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#ifndef __MiniShell2__Jobs__
#define __MiniShell2__Jobs__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

typedef class Jobs{
//parent only can have this one.
public:
	Jobs(){
		jobNum = 0;
		status = string();
		command = string();
		pid = -1;
		doneChecking = false;
	};
	Jobs(int myJobNum,string myCommand,pid_t myPid){
		jobNum = myJobNum;
		status = "Running";
		command = myCommand;
		pid = myPid;
		doneChecking = false;
	};
	int printCurrentJob(string str);
	int updateStatus(int terminatePID);
	int killProcess();
	static void signal_handler(int signo);
	pid_t getPid();
	int getJobNum();
private:
	int jobNum;
	bool doneChecking;
	pid_t pid;
	string status;
	string command;
	
}Jobs;

#endif /* defined(__MiniShell2__Jobs__) */
