#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
class CToolInvoker
{
private:
	CArgumentParser *parser;
	static const char *CH_EXPORT_HEADER_FILE_NAME;
	
	// データ行数.
	int n_line;

	// データ個数.
	int n_count;

	short sh_errorCode;

	void Read();
	void Convert();
	void LineCount();
	void Write();
	bool HelpCheck();
	bool VersionCheck();
	CToolInvoker():sh_errorCode(0){}
public:
	CToolInvoker(CArgumentParser *parser):
		parser(parser),
		n_line(0),
		n_count(0),
		sh_errorCode(0)
		{
	}
	int Invoke();
	short GetErrorCode(){
		return sh_errorCode;
	}

};
#endif // !defined(__tool__ToolInvoker__)
