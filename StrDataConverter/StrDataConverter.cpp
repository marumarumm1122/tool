// StrDataConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Tool.h"

int _tmain(int argc, char* argv[])
{
    tool::CTool theTool(argc,argv);	
    theTool.Main();

	return 0;
}

