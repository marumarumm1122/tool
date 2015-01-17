#include <stdio.h>
#include <string.h>
#include <iostream>
#include "ArgumentParser.h"
#include "Error.h"

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
    if(ch_checkArgs[eARGUMENT_VERSION_COMMAND]!=NULL || ch_checkArgs[eARGUMENT_HELP_COMMAND]!=NULL){
        return true;
    }
	if(ch_checkArgs[eARGUMENT_INPUT_FILE_COMMAND]==NULL ){
        sh_errorCode = CError::ERR_ARG_REQUIRED_PARAMETER_INPUT_FILE;
        return false;
    }
    if(ch_checkArgs[eARGUMENT_OUTPUT_FILE_COMMAND]==NULL ){
        sh_errorCode = CError::ERR_ARG_REQUIRED_PARAMETER_OUTPUT_FILE;
        return false;
    }
    if(ch_checkArgs[eARGUMENT_ENDIAN_COMMAND]==NULL ){
        sh_errorCode = CError::ERR_ARG_REQUIRED_PARAMETER_ENDIAN;
        return false;
    }
    if(ch_checkArgs[eARGUMENT_FILEVERSION_COMMAND]==NULL ){
        sh_errorCode = CError::ERR_ARG_REQUIRED_PARAMETER_FILEVERSION;
        return false;
    }
	return true;
}
bool CArgumentParser::ParseArguments(){
	for(int nCnt=1;nCnt<n_argc;nCnt++){
    	if(strncmp(ch_argv[nCnt],"-f",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-f'.");
                sh_errorCode = CError::ERR_ARG_ILLEGAL_PARAMETER_INPUT_FILE_VALUE;
    			return false;
    		}
    	    ch_checkArgs[eARGUMENT_INPUT_FILE_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_INPUT_FILE_DATA] = ch_argv[++nCnt];
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-o",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-o'.");
                sh_errorCode = CError::ERR_ARG_ILLEGAL_PARAMETER_OUTPUT_FILE_VALUE;
    			return false;
    		}
    	    ch_checkArgs[eARGUMENT_OUTPUT_FILE_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_OUTPUT_FILE_DATA] = ch_argv[++nCnt];
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-e",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-e'.");
                sh_errorCode = CError::ERR_ARG_ILLEGAL_PARAMETER_ENDIAN_VALUE;
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
                sh_errorCode = CError::ERR_ARG_UNKNOWN_PARAMETER_ENDIAN_VALUE;
    			return false;
    		}
    	    continue;
        }
    	if(strncmp(ch_argv[nCnt],"-n",2)==0){
    		if(nCnt+1>=n_argc){
    			Error("argument index out ob bounds '-n'.");
                sh_errorCode = CError::ERR_ARG_ILLEGAL_PARAMETER_FILEVERSION_VALUE;
    			return false;
    		}
            if(atoi(ch_argv[nCnt+1])==0){
                Error("argument not digit");
                sh_errorCode = CError::ERR_ARG_NOT_DIGIT_PARAMETER_FILEVERSION_VALUE;
                return false;
            }
    	    ch_checkArgs[eARGUMENT_FILEVERSION_COMMAND] = ch_argv[nCnt];
    	    ch_checkArgs[eARGUMENT_FILEVERSION_DATA] = ch_argv[++nCnt];
    	    continue;
        }
        if(strncmp(ch_argv[nCnt],"-v",2)==0){
            ch_checkArgs[eARGUMENT_VERSION_COMMAND] = ch_argv[nCnt];
            continue;
        }
        if(strncmp(ch_argv[nCnt],"-h",2)==0){
            ch_checkArgs[eARGUMENT_HELP_COMMAND] = ch_argv[nCnt];
            continue;
        }

    }
	return true;
}
void CArgumentParser::Usage(){
#if defined(_WIN32)
    printf( "usage: StrDataConverter.exe -f <input> -o <output> -e <byte order> -n <file version> \n" );
#else   // defined(_WIN32)
    printf("usage: StrDataConverter.bin -f <input> -o <output> -e <byte order> -n <file version> \n" );
#endif  // defined(_WIN32)
    printf( "-f : input filename\n" );
    printf( "-o : output filename\n" );
    printf( "-e : byte order(be or le) be by default\n" );
    printf( "-v : version\n" );
    printf( "-n : file version\n");
    printf( "-h : help\n");
}
void CArgumentParser::Error(const char *ch_message){
	printf("%s\n",ch_message);
    Usage();
}
