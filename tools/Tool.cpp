#include "Tool.h"
#include <stdio.h>
#include <string.h>
#include "ArgumentParser.h"
#include "ToolInvoker.h"
void CTool::Main(){
    CArgumentParser parser(n_argc,ch_argv);
    if(!parser.Parse()){
        printf("parsing Error.\n");
        return;
    }

    CToolInvoker invoker(&parser);
    int ret = invoker.Invoke();
    if(ret>0){
        printf("invoke Error.\n");
    }
    
    
}
