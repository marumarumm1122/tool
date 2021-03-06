#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
class CToolInvoker
{
private:
	CArgumentParser *m_parser;
	
    enum E_COLUMN_TYPE{
		eCOLUMN_TYPE_START = -1,

        eCOLUMN_TYPE_NONE = eCOLUMN_TYPE_START,
        eCOLUMN_TYPE_DUMMY ,
        eCOLUMN_TYPE_USHORT,
        eCOLUMN_TYPE_BYTE,
        eCOLUMN_TYPE_UINT,
        eCOLUMN_TYPE_FLOAT,
        eCOLUMN_TYPE_BOOL,
        
		eCOLUMN_TYPE_NUM,
		eCOLUMN_TYPE_MAX = eCOLUMN_TYPE_NUM - 1,
    };

	// データ行数.
	int m_nLine;

	// データ個数(データ部).
	int m_nCount;
    
    // 入力データ行数.
    int m_nInputLine;
    
    // 入力データ.
    void *vInputBuff;

    // １レコードのサイズ.
    int m_nRecordSize;
    
    // １レコードのカラム数.
    int m_nRecordColumnSize;
    
    // １レコードの各バイト数.
    E_COLUMN_TYPE m_nBytesOfColumns[64];

    // 出力header用のデータサイズ.
    //int m_nHeaderSize;
    
	short m_shErrorCode;

    bool MakeStringHeader();
    bool MakeStringIdList();
	bool Convert();
	bool LineCount();
	bool Write();
	bool HelpCheck();
	bool VersionCheck();
    bool InputFileRead();
    bool WriteHeaderFile();
    bool WorkFileRead();
    const char *GetHeaderFileName();
    void WriteEndLine(FILE *fp,int nLineCnt);
    void GetIncludeHeaderName(char *ch_includeHeaderName,const char *headerFileName);
    void MakeHeaderData(char *ch_header,const char *chHeaderFileName,char *ch_includeHeaderName);
    void MakeFooterData(char *ch_footer,char *ch_includeHeaderName);
    const char *GetEnumName();
    
    bool WriteInputBuffer();
    bool LoadInputFile();
    bool MakeSchema();
    void SplitCanma(char *ch_in,int startByte);
    bool LoadMemory();

	CToolInvoker():
    m_nInputLine(0),
	vInputBuff(NULL),
    m_nRecordSize(0),
    m_nRecordColumnSize(0),
    //m_nHeaderSize(0),
    m_shErrorCode(0){}
public:
	CToolInvoker(CArgumentParser *parser):
		m_parser(parser),
		m_nLine(0),
		m_nCount(0),
    m_nInputLine(0),
	vInputBuff(NULL),
	m_nRecordSize(0),
    m_nRecordColumnSize(0),
    //m_nHeaderSize(0),
		m_shErrorCode(0)
		{
            for(int nCnt=0;nCnt<64;nCnt++){
                m_nBytesOfColumns[nCnt] = eCOLUMN_TYPE_NONE;
            }

	}
	int Invoke();
	short GetErrorCode()
    {
		return m_shErrorCode;
	}

};
#endif // !defined(__tool__ToolInvoker__)
