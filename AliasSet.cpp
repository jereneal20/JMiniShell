//
//  aliasSet.cpp
//  MiniShell
//
//  Created by Jereneal Kim on 13. 10. 13..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#include "AliasSet.h"

using namespace std;

int AliasSet::setNewCommand(wordexp_t &result){
	//check this cmd already defined
	int cmdCheck=-1;
	for(int iter=0;iter<numOfElement;iter++){
		if(strcmp(result.we_wordv[1],element[iter].newCommand)==0){
			cmdCheck = iter;
			//cmd exist
		}
	}
	if(cmdCheck==-1){
		//cmd가 겹치지 않는 경우, 그냥 nOE에 추가
		cmdCheck = numOfElement;
		numOfElement++;
	}else{
		//cmd가 겹친경우
		memset(element[cmdCheck].newCommand, '\0', 1000);
		memset(element[cmdCheck].originCmd, '\0', 1000);
	}
	
	strcat(element[cmdCheck].newCommand, result.we_wordv[1]);
	strcat(element[cmdCheck].newCommand, " ");
	for(int iter=2;result.we_wordv[iter]!=NULL;iter++){
		strcat(element[cmdCheck].originCmd, result.we_wordv[iter]);
		//		if(result.we_wordv[iter+1]!=NULL){
		strcat(element[cmdCheck].originCmd, " ");
		//		}
	}
	return cmdCheck;
}
int AliasSet::findCmd(string origin,int& cmdIdx){
	origin+=" ";
	for(int iter=0;iter<numOfElement;iter++){
		size_t strIdx = origin.find(element[iter].newCommand);
		if(strIdx!=string::npos){
			cmdIdx = iter;
			return (int)strIdx;
		}
	}
	return -1;
}
string AliasSet::replaceCmd(string origin,int cmdIdx){
	origin+=" ";
	string retString = string(origin);
	size_t strIdx = origin.find(element[cmdIdx].newCommand);
	if(strIdx!=string::npos){
		string newStr = string(element[cmdIdx].newCommand);
		string originStr = string(element[cmdIdx].originCmd);
		
		retString.replace(strIdx, newStr.length(), originStr);
	}
	return retString;
}
