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

