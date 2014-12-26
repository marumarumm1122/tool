#include "Tool.h"
#include <stdio.h>
#include <string.h>
#include "ArgumentParser.h"
#include "ToolInvoker.h"
using namespace tool;
void CTool::Main(){
    CArgumentParser *parser = new CArgumentParser(n_argc,ch_argv);
    if(!parser->Parse()){
        delete parser;
        printf("parsing Error.\n");
        return;
    }

    CToolInvoker *invoker = new CToolInvoker(parser);
    int ret = invoker->Invoke();
    if(ret>0){
        printf("invoke Error.\n");
    }
    delete parser;
    delete invoker;
    
    
}
void CTool::Error(const char *ch_message){
	printf("%s\n",ch_message);
	printf("usage: test -f <file> -o <file> -e be/le -v <バージョン> \n\n");
	printf("-f 入力ファイル名\n");
	printf("-o 出力ファイル名\n");
	printf("-e be ビッグエンディアン指定（デフォルト）");
	printf(" / le リトルエンディアン指定\n");
    printf("-v バージョン\n");
}
