//
//  CmdLineSeparator.cpp
//  MiniShell2
//
//  Created by Jereneal Kim on 2013. 11. 26..
//  Copyright (c) 2013년 Jereneal Kim. All rights reserved.
//

#include "CmdLineSeparator.h"

int CmdLineSeparator::splitOneStatement(const char* originState){
	
	bool singleQuoteInner = false;
	bool doubleQuoteInner = false;
	int retIdx=0;
	char retStr[1000] = {0};
	//처음에 ;같은게 있으면 아무것도 없는 문장이라 리턴시키고 다시 들어 올 것임.
	//spiltIdx는 문장을 새로받지 않는 이상 보존되도록 해둠.
	//retIdx는 이번 문장에서 실제로 쓰이는 문자 개수가 들어감.
	if(originState[splitIdx]==';'||originState[splitIdx]==' '){
		splitIdx++;
		return 0;
	}
	
	for(;splitIdx<1000;splitIdx++,retIdx++){
		
		if(originState[splitIdx]=='$'&&singleQuoteInner==true){
			retStr[retIdx] = '\n';
			//'일때 $표시를 \n로 바꿔서 환경변수가 아닌 글자로 인식되도록 한다.
			//\n는 문장내에 존재하지않으므로 끝에서 다시 바꿔주면됨.^^
		}else{
			retStr[retIdx]=originState[splitIdx];
		}
		if(originState[splitIdx]=='"'){
			doubleQuoteInner = doubleQuoteInner==true?false:true;
		}else if(originState[splitIdx]==39){//'
			singleQuoteInner = singleQuoteInner==true?false:true;
		}
		if(doubleQuoteInner==false&&singleQuoteInner==false){
			//둘중 하나가 true이면 이를 나중에 처리해줘야..
			if(originState[splitIdx]==';'){
				splitIdx++;
				break;
			}else if(originState[splitIdx]=='>'){
				redirectionInputMark = true;
				splitIdx++;
				break;
			}else if(originState[splitIdx]=='|'){
				pipeInputMark = true;
				splitIdx++;
				break;
			}else if(originState[splitIdx]=='\n'){
				break;
			}
		}else{
			if(originState[splitIdx]=='\n'){
				//incorrect statement
				return -1;
			}
		}
	}
	
	for(int i=retIdx;i>0;i--){
		//마지막에 이런 것들이 있는 경우 없애줘야 명령어 정상 작동
		if(retStr[i]==' '||retStr[i]=='\n'||retStr[i]==';'||retStr[i]=='>'||retStr[i]=='|'){
			retStr[i]='\0';
		}else if(retStr[i]=='&'){
			containBackgroundMark = true;
			retStr[i]='\0';
		}else{
			retIdx=i;
			break;
		}
	}
	
	
	cmdString = string(retStr);
	return retIdx;
}
