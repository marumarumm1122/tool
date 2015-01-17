#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
class CToolInvoker
{
private:
	CArgumentParser *parser;
	static const char *CH_EXPORT_HEADER_FILE_NAME;
    static const char *WORK_FILE_EXT_CONST_NAME;
    static const char *WORK_FILE_EXT_CONST_VALUE;
	
	// データ行数.
	int n_line;

	// データ個数(データ部).
	int n_count;
    
    // データ個数(ヘッダ部).
    int n_nCount;
    
	short sh_errorCode;

	bool Read();
	bool Convert();
	bool LineCount();
	bool Write();
	bool HelpCheck();
	bool VersionCheck();
    bool WriteWorkFile();
    bool InputFileRead();
    bool WriteTemporaryFile();
    bool WriteHeaderFile();
    bool WorkFileRead();
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
