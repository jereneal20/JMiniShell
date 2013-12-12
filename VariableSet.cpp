//
//  VariableSet.cpp
//  MiniShell
//
//  Created by Jereneal Kim on 13. 10. 13..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#include "VariableSet.h"

using namespace std;

int VariableSet::setNewCommand(wordexp_t &result){
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
	strcat(element[cmdCheck].originCmd, result.we_wordv[3]);
	return cmdCheck;
}

int VariableSet::findCmd(string origin,int& cmdIdx){

	for(int iter=0;iter<numOfElement;iter++){
		char tmp2[1000] = {0};
		tmp2[0]=' ';
		origin.copy(tmp2+1, origin.length());
		char* tok1 = strtok(tmp2, "$");
		tok1 = strtok(NULL, "$");
		strtok(tok1, "\'\"} \0");
		if(tok1[0]=='{'){
			if(strcmp(tok1+1, element[iter].newCommand)==0){
				cmdIdx = iter;
				return (int)(tok1-tmp2-1);
			}
		}else{
			if(strcmp(tok1, element[iter].newCommand)==0){
				cmdIdx = iter;
				return (int)(tok1-tmp2-1);
			}
		}

	}
	return -1;
}
string VariableSet::replaceCmd(string origin,int cmdIdx,int dollarPosition){
	string retString = string(origin);
	if(dollarPosition>0&&retString.at(dollarPosition-1)=='\\'){
		return retString;
	}
	
	if(cmdIdx==-1){//env var이나 없는 변수인 경우
		char tmpArr[1000]={0};
		origin.copy(tmpArr, origin.length());
		char* tok = strtok(tmpArr+dollarPosition+1, " \0");
		strtok(tok, "\"\'\0 ");
		char* tmp2Arr = getenv(tok);
		
		if(tok[0]!='{'&&tmp2Arr==NULL){
			printf("%s: Undefined variable.\n",tok);
			return string();
		}
	}else{

		string newStr = string(element[cmdIdx].newCommand);
		string originStr = string(element[cmdIdx].originCmd);
		if(retString.at(dollarPosition+1)=='{'){
			retString.replace(dollarPosition, newStr.length()+3, originStr);
		}else{
			retString.replace(dollarPosition, newStr.length()+1, originStr);
		}
	}
	
	return retString;
}
int VariableSet::replaceVariables(string& newStr,string checkStr){
	
	if(checkStr.length()>0){
		newStr = string(checkStr);
		size_t found = newStr.find('$');
		while(found!=string::npos){
			
			int varIdx;
			int check = findCmd(newStr, varIdx);
			if(check==-1){//env var
				newStr = replaceCmd(newStr,-1,(int)found);
				if(newStr.empty()){
					return -1;//undefined var
				}
			}else{
				newStr = replaceCmd(newStr,varIdx,(int)found);
			}
			found = newStr.find('$',found+1);
		}

		newStr+='\0';
	}
	return 0;
}
