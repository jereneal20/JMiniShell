//
//  VariableSet.h
//  MiniShell
//
//  Created by Jereneal Kim on 13. 10. 13..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#ifndef __MiniShell__VariableSet__
#define __MiniShell__VariableSet__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <wordexp.h>

using namespace std;

typedef class VariableSet{
public:
	VariableSet(){
		numOfElement=0;
		for(int iter=0;iter<100;iter++){
			element[iter] = VariableElement();
		}
	}
	int setNewCommand(wordexp_t& result);
	int findCmd(string origin,int& cmdIdx);
	string replaceCmd(string origin,int cmdIdxint,int dollarPosition);
	int replaceVariables(string& newStr,string checkStr);

	typedef class VariableElement{
	public:
		VariableElement(){
			memset(newCommand, '\0', 1000);
			memset(originCmd, '\0', 1000);
		}
		char newCommand[1000];
		char originCmd[1000];
	}VariableElement;
	
	VariableElement element[100];
	int numOfElement;
}VariableSet;

#endif /* defined(__MiniShell__VariableSet__) */
