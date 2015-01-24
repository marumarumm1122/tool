#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
class CToolInvoker
{
private:
	CArgumentParser *m_parser;
	static const char *CH_EXPORT_HEADER_FILE_NAME;
    static const char *WORK_FILE_EXT_CONST_NAME;
    static const char *WORK_FILE_EXT_CONST_VALUE;
	
	// データ行数.
	int m_nLine;

	// データ個数(データ部).
	int m_nCount;
    
	short m_shErrorCode;

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
    const char *GetHeaderFileName();
    void WriteEndLine(FILE *fp,int nLineCnt);
    void GetIncludeHeaderName(char *ch_includeHeaderName,const char *headerFileName);
    void MakeHeaderData(char *ch_header,const char *chHeaderFileName,char *ch_includeHeaderName);
    void MakeFooterData(char *ch_footer,char *ch_includeHeaderName);
    const char *GetEnumName();
	CToolInvoker():m_shErrorCode(0){}
public:
	CToolInvoker(CArgumentParser *parser):
		m_parser(parser),
		m_nLine(0),
		m_nCount(0),
		m_shErrorCode(0)
		{
	}
	int Invoke();
	short GetErrorCode()
    {
		return m_shErrorCode;
	}

};
#endif // !defined(__tool__ToolInvoker__)
