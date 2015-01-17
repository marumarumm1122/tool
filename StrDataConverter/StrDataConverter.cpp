// StrDataConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "Tool.h"

int _tmain(int argc, char* argv[])
{
    CTool theTool(argc,argv);

	short sh_error = theTool.Main();
	if (sh_error > 0){
		printf("error code %d\n", sh_error);
		return sh_error;
	}
	return 0;
}

