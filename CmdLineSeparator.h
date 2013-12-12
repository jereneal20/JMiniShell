//
//  CmdLineSeparator.h
//  MiniShell2
//
//  Created by Jereneal Kim on 2013. 11. 26..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#ifndef __MiniShell2__CmdLineSeparator__
#define __MiniShell2__CmdLineSeparator__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

using namespace std;

typedef class CmdLineSeparator{
public:
	CmdLineSeparator(){
		splitIdx=0;
		containBackgroundMark = false;
		redirectionInputMark = false;
		pipeInputMark = false;
		pipeOutputMark = false;
	};
	int splitOneStatement(const char* originState);
	int splitIdx;
	string cmdString;
	bool containBackgroundMark;
	bool redirectionInputMark;
	bool pipeInputMark;
	bool pipeOutputMark;
}CmdLineSeparator;


#endif /* defined(__MiniShell2__CmdLineSeparator__) */
