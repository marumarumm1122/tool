#include <stdio.h>
#include <string.h>
#include "ToolInvoker.h"
using namespace tool;
namespace{
    char ch_buff[64];
    void *p_vbuffer = NULL;
    void *p_convertBuffer = NULL;
    void *p_headerBuffer = NULL;
    
}
const char *CToolInvoker::CH_EXPORT_HEADER_FILE_NAME = "S_LINK_DATA_STRING_HEADER.h";

int CToolInvoker::Invoke(){
    return 0;
}

void CToolInvoker::Error(const char *ch_message){
	printf("%s\n",ch_message);
	printf("usage: test -f <file> -o <file> -e be/le -v <バージョン> \n\n");
	printf("-f 入力ファイル名\n");
	printf("-o 出力ファイル名\n");
	printf("-e be ビッグエンディアン指定（デフォルト）");
	printf(" / le リトルエンディアン指定\n");
    printf("-v バージョン\n");
}
