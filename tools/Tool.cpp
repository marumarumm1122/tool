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
