//
//  Jobs.cpp
//  MiniShell2
//
//  Created by Jereneal Kim on 2013. 12. 5..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#include "Jobs.h"

int Jobs::printCurrentJob(string str){
//	status.compare("Done")==0
	if(doneChecking==true){
		return -1;
	}else{
		printf("[%d]%s   %s\t%s\n",jobNum,str.c_str(),status.c_str(),command.c_str());
		if(status.compare("Done")==0||status.compare("Terminated")==0){
			doneChecking=true;
		}
		return 0;
	}
}
int Jobs::updateStatus(int terminatePID){
	if(pid==terminatePID){
		status = "Done";
		return 0;
	}
	return 1;
}

int Jobs::killProcess(){
	status = "Terminated";
	printCurrentJob("");
	return 0;
}
pid_t Jobs::getPid(){
	return pid;
}

int Jobs::getJobNum(){
	return jobNum;
}
void Jobs::signal_handler(int signo){
	printf("Call some signal: %d\n",signo);
	return;
}