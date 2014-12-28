#include <stdio.h>
#include <string.h>
#include "ArgumentParser.h"
using namespace tool;

bool CArgumentParser::Parse(){
	if(!ParseArguments()){
		return false;
	}
	if(!ArgumentCheck()){
		return false;
	}
	return true;

}
bool CArgumentParser::ArgumentCheck(){
	if(ch_checkArgs[eARGUMENT_INPUT_FILE_COMMAND]==NULL 
		|| ch_checkArgs[eARGUMENT_OUTPUT_FILE_COMMAND]==NULL 
		|| ch_checkArgs[eARGUMENT_ENDIAN_COMMAND]==NULL 
		|| ch_checkArgs[eARGUMENT_VERSION_COMMAND]==NULL 
		){
		Error("argument invalid count.");
		return false;
	}
	return true;
}
bool CArgumentParser::ParseArguments(){
	for(int nCnt=1;nCnt<n_argc;nCnt++){
    	if(strncmp(ch_argv[nCnt],"-f",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-f'.");
    			return false;
    		}
    	    ch_checkArgs[eARGUMENT_INPUT_FILE_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_INPUT_FILE_DATA] = ch_argv[++nCnt];
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-o",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-o'.");
    			return false;
    		}
    	    ch_checkArgs[eARGUMENT_OUTPUT_FILE_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_OUTPUT_FILE_DATA] = ch_argv[++nCnt];
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-e",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-e'.");
    			return false;
    		}
    		if(strncmp(ch_argv[nCnt+1],"be",2)==0){
                ch_checkArgs[eARGUMENT_ENDIAN_COMMAND] = ch_argv[nCnt];
                ch_checkArgs[eARGUMENT_ENDIAN_DATA] = ch_argv[++nCnt];
                byteOrder = eBYTE_ORDER_BIGENDIAN;
            }else if(strncmp(ch_argv[nCnt+1],"le",2)==0){
                ch_checkArgs[eARGUMENT_ENDIAN_COMMAND] = ch_argv[nCnt];
                ch_checkArgs[eARGUMENT_ENDIAN_DATA] = ch_argv[++nCnt];
                byteOrder = eBYTE_ORDER_LITTLEENDIAN;
            }else{
    			Error("argument endian is not word be/le.");
    			return false;
    		}
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-v",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-v'.");
    			return false;
    		}
    	    ch_checkArgs[eARGUMENT_VERSION_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_VERSION_DATA] = ch_argv[++nCnt];
    	    continue;
        }

    }
	return true;
}

void CArgumentParser::Error(const char *ch_message){
	printf("%s\n",ch_message);
	printf("usage: test -f <file> -o <file> -e be/le -v <バージョン> \n\n");
	printf("-f 入力ファイル名\n");
	printf("-o 出力ファイル名\n");
	printf("-e be ビッグエンディアン指定（デフォルト）");
	printf(" / le リトルエンディアン指定\n");
    printf("-v バージョン\n");
}
