#include "Tool.h"
#include <stdio.h>
#include <string.h>

using namespace tool;
namespace{
    char ch_buff[64];

}
void CTool::Main(){
    if(!ArgumentCheck()){
        return;
    }
    
    
}
bool CTool::ArgumentCheck(){
	if(n_argc<6){
		Error("argument Error");
		return false;
	}
	for (int nCnt = 0;nCnt < n_argc;nCnt++){
		switch(nCnt){
			case 0:
				break;
			case 1:{
				if(strncmp(ch_argv[nCnt],"-f",2)!=0){
					sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
					Error(ch_buff);
                    return false;
				}
				break;
			}
			case 3:{
                if(strncmp(ch_argv[nCnt],"-o",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
			}
			case 5:{
                if(strncmp(ch_argv[nCnt],"-e",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
			}
            case 6:{
                if(strncmp(ch_argv[nCnt],"be",2)!=0 && strncmp(ch_argv[nCnt],"le",2)!=0){
                    sprintf(ch_buff,"Uncknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
            }
			default:
				
				break;
		}
	}
    return true;

}
void CTool::Error(const char *ch_message){
	printf("%s\n",ch_message);
	printf("usage: test -f <file> -o <file> -e be/le [options]\n\n");
	printf("-f 入力ファイル名\n");
	printf("-o 出力ファイル名\n");
	printf("-tb 出力タイプバイナリ（デフォルト）\n");
	printf("-tt 出力タイプテキスト\n");
	printf("-e be ビッグエンディアン指定（デフォルト）");
	printf(" / le リトルエンディアン指定\n");
}
