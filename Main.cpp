#include "tools/Tool.h"

using namespace tool;

int main(int argc, char **argv){
	CTool *tool = new CTool(argc,argv);
	
    tool->Main();
    delete tool;
    return 0;
}

