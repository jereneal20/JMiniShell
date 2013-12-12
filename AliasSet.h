//
//  aliasSet.h
//  MiniShell
//
//  Created by Jereneal Kim on 13. 10. 13..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#ifndef __MiniShell__aliasSet__
#define __MiniShell__aliasSet__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <wordexp.h>

using namespace std;

typedef class AliasSet{
public:
	AliasSet(){
		numOfElement=0;
		for(int iter=0;iter<100;iter++){
			element[iter] = AliasElement();
		}
	}
	int setNewCommand(wordexp_t& result);
	int findCmd(string origin,int& cmdIdx);
	string replaceCmd(string origin,int cmdIdx);
	typedef class AliasElement{
	public:
		AliasElement(){
			memset(newCommand, '\0', 1000);
			memset(originCmd, '\0', 1000);
		}
		char newCommand[1000];
		char originCmd[1000];
	}AliasElement;
	
	AliasElement element[100];
	int numOfElement;
}AliasSet;

#endif /* defined(__MiniShell__aliasSet__) */
