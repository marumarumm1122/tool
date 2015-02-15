#include <stdio.h>
#include "Error.h"
#include "Tool.h"
#include <string.h>
#include "ToolInvoker.h"
#include <stdlib.h>
#if defined(_WIN32)
#include <WinSock2.h>
#endif  // defined(_WIN32)
#include "Commons.h"

namespace{
    void *p_vbuffer = NULL;
    void *p_convertBuffer = NULL;
    int *n_headerList;
    int *n_startHeaderList;
    // インプットファイル用.
    void *p_vibuffer = NULL;
    // ワークファイル用.
    void *p_nbuffer = NULL;
    
    int *n_stringIdList;
    char *p_stringConstList;
    char *p_stringDataList;
}
const char *CToolInvoker::CH_EXPORT_HEADER_FILE_NAME = "S_LINK_DATA_STRING_HEADER.h";
const char *CToolInvoker::WORK_FILE_EXT_CONST_NAME = ".constn";
const char *CToolInvoker::WORK_FILE_EXT_CONST_VALUE = ".constv";

int CToolInvoker::Invoke()
{
    // バージョンチェック.
    if(VersionCheck()){
        return 0;
    }
    
    // ヘルプチェック.
    if(HelpCheck()){
        return 0;
    }
    
    // 入力ファイルをオープンしてvoid*に格納していく.
    if(!LoadInputFile()){
        return m_shErrorCode;
    }
    
    // 格納したバッファをファイルに書き出す.
    if(!WriteInputBuffer()){
        return m_shErrorCode;
    }

    // string header関連処理.
    if(!MakeStringHeader()){
        return m_shErrorCode;
    }
    
    
    return 0;
}
bool CToolInvoker::MakeStringHeader()
{
    char **ch_args = m_parser->GetParseArgs();
    
    if(ch_args[CArgumentParser::eARGUMENT_OUTPUT_HEADER_NAME_COMMAND]==NULL){
        // ヘッダファイル指定がある場合のみ作る.
        return true;
    }
    
    // string入力ファイルをオープンして名前IDと位置のintリストを作る.
    if(!MakeStringIdList()){
        return false;
    }
    
    
    // 入力ファイルをオープンして二つの一時ファイルに振り分ける.
    //if(!WriteWorkFile()){
    //    return m_shErrorCode;
    //}
    
	// 振り分けた一時ファイル二つ目をオープン.
    //if(!Read()){
    //    return m_shErrorCode;
    //}
    
	// 振り分けた一時ファイル二つ目を変換.
    if(!Convert()){
        return m_shErrorCode;
    }
    
    // 出力ファイルに書き込む.
    if(!Write()){
        return m_shErrorCode;
    }
    
    // 振り分けた一時ファイル一つ目をもとに定数用ヘッダファイルを書き出す.
    if(!WriteHeaderFile()){
        return m_shErrorCode;
    }
    
    return true;
}
bool CToolInvoker::MakeStringIdList()
{
    char **ch_args = m_parser->GetParseArgs();
    
    FILE *fp;
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_FILE_STRING_DATA], "rb");
    char ch_read[1024];
    n_stringIdList = reinterpret_cast<int*>(malloc(sizeof(int)));
    p_stringConstList = reinterpret_cast<char*>(malloc(sizeof(char)*1024));
    p_stringDataList = reinterpret_cast<char*>(malloc(sizeof(char)*1024));
    enum eCOLUMN_COUNTER{
        eCOLUMN_COUNTER_CONST,
        eCOLUMN_COUNTER_ID,
        eCOLUMN_COUNTER_DATA,
        eCOLUMN_COUNTER_END,
    };
    int nLastConstIdx = 0;
    int nLastDataIdx = 0;
    int nColumnCounter = 0;
    while ( fgets(ch_read, 1024, fp) != NULL ) {
        int nCnt = 0;
        int nLastCnt = 0;
        eCOLUMN_COUNTER ec = eCOLUMN_COUNTER_CONST;
        while(nCnt < 1024){
            if(ch_read[nCnt]==','||ch_read[nCnt]=='\0'){
                switch (ec) {
                    case eCOLUMN_COUNTER_CONST:
                    {
                        int nL = nLastConstIdx % 1024;
                        int nC = nLastConstIdx / 1024;
                        if(nL + nCnt+2 >= 1024){
                            p_stringConstList = reinterpret_cast<char*>(realloc(p_stringConstList, sizeof(char)*1024*(nC+1)));
                            
                        }
                        memcpy(p_stringConstList+nLastConstIdx, ch_read, nCnt);
                        nLastConstIdx += nCnt;
                        p_stringConstList[nLastConstIdx] = '\n';
                        p_stringConstList[nLastConstIdx+1] = '\0';
                        nLastConstIdx++;
                        nCnt++;
                        nLastCnt = nCnt;
                        ec = eCOLUMN_COUNTER_ID;
                        break;
                    }
                    case eCOLUMN_COUNTER_ID:
                    {
                        n_stringIdList = reinterpret_cast<int*>(realloc(n_stringIdList, sizeof(int)*(nColumnCounter+2)));
                        n_stringIdList[nColumnCounter] = atoi(ch_read+nLastCnt);
                        nColumnCounter++;
                        nCnt++;
                        nLastCnt = nCnt;
                        ec = eCOLUMN_COUNTER_DATA;
                        break;
                    }
                    case eCOLUMN_COUNTER_DATA:
                    {
                        int nL = nLastDataIdx % 1024;
                        int nC = nLastDataIdx / 1024;
                        if(nL + nCnt+2 >= 1024){
                            p_stringDataList = reinterpret_cast<char*>(realloc(p_stringDataList, sizeof(char)*1024*(nC+1)));
                            
                        }
                        memcpy(p_stringDataList+nLastDataIdx, ch_read+nLastCnt, nCnt-nLastCnt);
                        nLastDataIdx += nCnt-nLastCnt;
                        p_stringDataList[nLastDataIdx+1] = '\0';
                        ec = eCOLUMN_COUNTER_END;
                        break;
                    }
                    default:
                        break;
                }
            }
            if(ec==eCOLUMN_COUNTER_END){
                break;
            }
            nCnt++;
        }
        
    }
    fclose(fp);
    
    // 一時ファイル一つ目のメモリにstringデータをコピーする.
    p_nbuffer = malloc(nLastConstIdx);
    memcpy(p_nbuffer, p_stringConstList, nLastConstIdx);
    
    // 一時ファイル二つ目のメモリにstringデータをコピーする.
    p_vbuffer = malloc(nLastDataIdx);
    memcpy(p_vbuffer, p_stringDataList, nLastDataIdx);
    
    p_convertBuffer = malloc(nLastDataIdx+1);
    m_nCount = nLastDataIdx;
    
    return true;
}
const char *CToolInvoker::GetHeaderFileName()
{
    char **ch_args = m_parser->GetParseArgs();
   
    return ch_args[CArgumentParser::eARGUMENT_OUTPUT_HEADER_NAME_DATA];
}
void CToolInvoker::WriteEndLine(FILE *fp,int nLineCnt)
{
    if(nLineCnt==0){
        const char *ch_enumName = GetEnumName();
        fprintf(fp," = %s_START,\n",ch_enumName);
        return;
    }
    fprintf(fp," = %d ,\n",n_stringIdList[nLineCnt]);
}
void CToolInvoker::GetIncludeHeaderName(char *ch_includeHeaderName,const char *headerFileName)
{
    {
        size_t nMax = strlen(headerFileName);
        int nCnt = (int)nMax-1;
        int nLCnt = 0;
        while(nCnt>=0){
            nCnt--;
            if(headerFileName[nCnt]=='.'){
                nLCnt = nCnt;
                break;
            }
            
        }
        while(nCnt>=0){
            nCnt--;
            if(headerFileName[nCnt]=='/'
//               ||headerFileName[nCnt]=='\¥'
               ||headerFileName[nCnt]=='\\'){
                nCnt++;
                break;
            }
        }

        if(nCnt<0){
            ch_includeHeaderName = NULL;
        }
        strncpy(ch_includeHeaderName,headerFileName+nCnt,nLCnt - nCnt);
        ch_includeHeaderName[nLCnt-nCnt] = '\0';
    }
}
void CToolInvoker::MakeHeaderData(char *ch_header,const char *chHeaderFileName,char *ch_includeHeaderName)
{
    const char *ch_enumName = GetEnumName();
    sprintf(ch_header,"//\n//  %s\n//\n// This file was generated by Tool.\n// It is possibe to change only the enum name.\n#if !defined(__%s__)\n#define __%s__\n\nenum %s{\n\t%s_START = %d,\n\n"
            ,chHeaderFileName
            ,ch_includeHeaderName
            ,ch_includeHeaderName
            ,ch_enumName
            ,ch_enumName
            ,n_stringIdList[0]
            );
}
void CToolInvoker::MakeFooterData(char *ch_footer,char *ch_includeHeaderName)
{
    const char *ch_enumName = GetEnumName();

    sprintf(ch_footer,"\n\t%s_NUM = %d,\n\t%s_MAX = %d,\n};\n\n#endif // !defined(__%s__)\n"
            ,ch_enumName
            ,m_nLine
            ,ch_enumName
            ,n_stringIdList[m_nLine-1]
            ,ch_includeHeaderName
            );
}
const char *CToolInvoker::GetEnumName()
{
    char **ch_args = m_parser->GetParseArgs();
    if(ch_args[CArgumentParser::eARGUMENT_OUTPUT_ENUM_NAME_DATA]==NULL){
        return "eLINK_DATA_STRING_HEADER";
    }
    return ch_args[CArgumentParser::eARGUMENT_OUTPUT_ENUM_NAME_DATA];
}
bool CToolInvoker::WriteHeaderFile()
{
	char **ch_args = m_parser->GetParseArgs();
	// オプションで-Hが指定されてなかったらtrue返して何もしない.
	if (ch_args[CArgumentParser::eARGUMENT_OUTPUT_HEADER_NAME_DATA] == NULL){
		return true;
	}

    //if(!WorkFileRead()){
    //    return false;
    //}
    const char *chHeaderFileName = GetHeaderFileName();
    const char *ch_enumName = GetEnumName();

    char ch_includeHeaderName[64];
    GetIncludeHeaderName(ch_includeHeaderName,chHeaderFileName);

    char ch_header[1024];
    MakeHeaderData(ch_header,chHeaderFileName,ch_includeHeaderName);
    
    char ch_footer[1024];
    MakeFooterData(ch_footer,ch_includeHeaderName);
    
    FILE *fp;
	FILEOPENANDERROR(fp, chHeaderFileName, "wb");
    
    fwrite(ch_header, strlen(ch_header),1,fp);

    char *ch_pBuffer = reinterpret_cast<char*>(p_nbuffer);
    fprintf(fp,"\t%s_",ch_enumName);
    int nLineCnt = 0;
    do{
        if(ch_pBuffer[0]=='\n' || ch_pBuffer[0]=='\0'){
            WriteEndLine(fp,nLineCnt);
            
            nLineCnt++;
            if(nLineCnt >= m_nLine){
                break;
            }
            fprintf(fp,"\t%s_",ch_enumName);
            continue;
        }
        fwrite(ch_pBuffer,sizeof(char),1,fp);
    }while(*ch_pBuffer++);

    fwrite(ch_footer, strlen(ch_footer),1,fp);
    
    fclose(fp);
    return true;
}
bool CToolInvoker::WriteWorkFile()
{
    // 入力ファイルを読み込む
    if(!InputFileRead()){
        return false;
    }
    
    // ワークファイルに書き込む
    if(!WriteTemporaryFile()){
        return false;
    }
    return true;
}
bool CToolInvoker::WorkFileRead()
{
	char **ch_args = m_parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    char *workn = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workn,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workn,WORK_FILE_EXT_CONST_NAME);
	FILEOPENANDERROR(fp, workn, "rb");
    free(workn);
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_nbuffer = malloc(n_fsize);
    fread(p_nbuffer,n_fsize,1,fp);
    
    fclose(fp);
    
    return true;
}

bool CToolInvoker::InputFileRead()
{
    char **ch_args = m_parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA], "rb");
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_vibuffer = malloc(n_fsize);
    fread(p_vibuffer,n_fsize,1,fp);
    
    fclose(fp);
    
    return true;
}
bool CToolInvoker::WriteTemporaryFile()
{
    char **ch_args = m_parser->GetParseArgs();
    char *workn = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workn,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workn,WORK_FILE_EXT_CONST_NAME);
    char *workv = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workv,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workv,WORK_FILE_EXT_CONST_VALUE);
    
    FILE *fpc;
	FILEOPENANDERROR(fpc, workn, "wb");

    FILE *fpv;
	fpv = fopen(workv, "wb");
    if(fpv==NULL){
        printf("File open error[%s]\n",workv);
        m_shErrorCode = ERR_INV_WORK_FILE_NOT_OPEN;
        fclose(fpc);
        return false;
    }

    free(workn);
    free(workv);
    
    char *ch_pBuffer = reinterpret_cast<char*>(p_vibuffer);
    bool b_constFlag = true;
    do{
        if(ch_pBuffer[0]==','){
            b_constFlag = false;
            ch_pBuffer[0] = '\n';
            fwrite(ch_pBuffer,sizeof(char),1,fpc);
            continue;
        }
        if(b_constFlag){
            // write work1.
            fwrite(ch_pBuffer,sizeof(char),1,fpc);
            
        }else{
            // write work2.
            fwrite(ch_pBuffer,sizeof(char),1,fpv);
            
        }
        if(ch_pBuffer[0]=='\n'){
            b_constFlag = true;
        }
        
    }while(*ch_pBuffer++);
    
    fclose(fpc);
    fclose(fpv);
    
    return true;
}
bool CToolInvoker::HelpCheck()
{
    char **ch_args = m_parser->GetParseArgs();
    if(ch_args[CArgumentParser::eARGUMENT_HELP_COMMAND]!=NULL){
        m_parser->Usage();
        return true;
    }
    return false;
}

bool CToolInvoker::VersionCheck()
{
    char **ch_args = m_parser->GetParseArgs();
    if(ch_args[CArgumentParser::eARGUMENT_VERSION_COMMAND]!=NULL){
        printf("tool version %s\n",CTool::TOOL_VERSION);
        return true;
    }
    return false;
}

bool CToolInvoker::Read()
{
	char **ch_args = m_parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    char *workv = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workv,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workv,WORK_FILE_EXT_CONST_VALUE);
	FILEOPENANDERROR(fp, workv, "rb");
    free(workv);
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_vbuffer = malloc(n_fsize);
    fread(p_vbuffer,n_fsize,1,fp);
    
    fclose(fp);
    p_convertBuffer = malloc(n_fsize+1);
    m_nCount = (int)n_fsize;
    
    return true;
}
bool CToolInvoker::Convert()
{
	// 行数カウント.
	return LineCount();
    
    
}
bool CToolInvoker::LineCount()
{
    m_nLine = 0;
    int n_bufferCount = 0;
    n_headerList = reinterpret_cast<int*>(malloc(sizeof(int)));
    char *ch_pBuffer = reinterpret_cast<char*>(p_vbuffer);
    char *ch_pConvertBuffer = reinterpret_cast<char*>(p_convertBuffer);
    do{
        
        n_bufferCount++;
        
        if(ch_pBuffer[0]!='\n' ){
            *ch_pConvertBuffer = *ch_pBuffer;
            ch_pConvertBuffer = ch_pConvertBuffer+1;
            continue;
        }
        
        *ch_pConvertBuffer = '\0';
        ch_pConvertBuffer = ch_pConvertBuffer+1;
        if(ch_pBuffer[0]=='\0'){
            break;
        }
        
        n_headerList = reinterpret_cast<int*>(realloc(n_headerList,sizeof(int)*(m_nLine+1)));
        n_headerList[m_nLine] = n_bufferCount;
        
        printf("col count:%d\n",n_headerList[m_nLine]);
        m_nLine++;
        
    }while(*ch_pBuffer++);
    printf("count:%d\n",m_nLine);
    
    // 開始位置を入れる.
    n_startHeaderList = reinterpret_cast<int*>(malloc(sizeof(int)));
    n_startHeaderList[0]=0;
    for(int nCnt=1;nCnt<m_nLine;nCnt++){
        int n_headerTmp = n_headerList[nCnt-1];
        if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
	    	printf("convert BIG ENDIAN\n");
            n_headerTmp = htonl(n_headerTmp);
        }
        n_startHeaderList[nCnt] = n_headerTmp;
    }
    
    return true;
}
bool CToolInvoker::Write()
{
    char **ch_args = m_parser->GetParseArgs();
    FILE *fp;
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA], "wb");
    
    // write version.
    short *sh_version = reinterpret_cast<short*>(malloc(sizeof(short)));
    short n_versionTmp = atoi(ch_args[CArgumentParser::eARGUMENT_FILEVERSION_DATA]);
    if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
    	printf("convert BIG ENDIAN\n");
        n_versionTmp = htons(n_versionTmp);
    }else{
        //n_versionTmp = ntohs(n_versionTmp);
    }
    sh_version[0] = n_versionTmp;
    fwrite(sh_version,sizeof(short),1,fp);
    
    // write size.
    short *sh_size = reinterpret_cast<short*>(malloc(sizeof(short)));
    short n_sizeTmp = m_nLine;
    if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
    	printf("convert BIG ENDIAN\n");
        n_sizeTmp = htons(n_sizeTmp);
    }else if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_LITTLEENDIAN){
    	printf("convert LITTLE ENDIAN\n");
        //n_sizeTmp = ntohs(n_sizeTmp);
    }else{
        printf("error[%d]\n",m_parser->GetByteOrder());
    }
    sh_size[0] = n_sizeTmp;
    fwrite(sh_size,sizeof(short),1,fp);
    
    // write header.
    fwrite(n_startHeaderList,m_nLine*sizeof(int),1,fp);
    
    // write data.
    fwrite(p_convertBuffer, m_nCount,1,fp);
    
    fclose(fp);
    
    return true;
}


bool CToolInvoker::WriteInputBuffer()
{
    char **ch_args = m_parser->GetParseArgs();
    FILE *fp;
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA], "wb");
    
    // write version.
    short *sh_version = reinterpret_cast<short*>(malloc(sizeof(short)));
    short n_versionTmp = atoi(ch_args[CArgumentParser::eARGUMENT_FILEVERSION_DATA]);
    if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
    	printf("convert BIG ENDIAN\n");
        n_versionTmp = htons(n_versionTmp);
    }else{
        //n_versionTmp = ntohs(n_versionTmp);
    }
    sh_version[0] = n_versionTmp;
    fwrite(sh_version,sizeof(short),1,fp);
    
    // write buffer.
    fwrite(vInputBuff,m_nInputLine*m_nRecordSize,1,fp);
    
    fclose(fp);
    return true;
}
bool CToolInvoker::MakeSchema()
{
    FILE *fp;
    //fpos_t n_fsize;
    char ch_read[64];
    int n_columnIdx = 0;
    
    char **ch_args = m_parser->GetParseArgs();
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_FILE_SCHEMA_DATA], "rb");
    eCOLUMN_TYPE n_size = eCOLUMN_TYPE_NONE;
    while ( fgets(ch_read, 64, fp) != NULL ) {
        if(strncmp(ch_read,"USHORT",6)==0){
            n_size = eCOLUMN_TYPE_USHORT;
            m_nRecordSize+=2;
        }else if(strncmp(ch_read,"BYTE",4)==0){
            n_size = eCOLUMN_TYPE_BYTE;
            m_nRecordSize+=1;
        }else if(strncmp(ch_read,"DUMMY",5)==0){
            n_size = eCOLUMN_TYPE_DUMMY;
            m_nRecordSize+=1;
        }else if(strncmp(ch_read,"UINT",4)==0){
            n_size = eCOLUMN_TYPE_UINT;
            m_nRecordSize+=4;
        }else{
            break;
        }
        m_nBytesOfColumns[n_columnIdx++] = n_size;
        
    }
    fclose(fp);
    m_nRecordColumnSize = n_columnIdx;
    return true;
}
void CToolInvoker::SplitCanma(char *ch_in,int startByte)
{
    char ch_temp[64];
    char *ch_buff = reinterpret_cast<char*>(vInputBuff);
    int nSCnt = 0;
    int nAppend = startByte;
    for(int nColumnIdx = 0;nColumnIdx < m_nRecordColumnSize;nColumnIdx++){
        int nCnt = 0;
        while(ch_in[nSCnt]){
            if(ch_in[nSCnt]=='"' || ch_in[nSCnt]==' '|| ch_in[nSCnt]=='\t'|| ch_in[nSCnt]=='\n'|| ch_in[nSCnt]=='\r'){
                nSCnt++;
                continue;
            }
            if(ch_in[nSCnt]==','){
                ch_temp[nCnt++] = '\0';
                nSCnt++;
                break;
            }
            ch_temp[nCnt++] = ch_in[nSCnt];
            nSCnt++;
        }
        
        eCOLUMN_TYPE nSize = m_nBytesOfColumns[nColumnIdx];
        switch((int)nSize){
            case eCOLUMN_TYPE_USHORT:
            {
                unsigned short ush = (unsigned short)atoi(ch_temp);
                if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
                    ush = htons(ush);
                }
                memcpy(ch_buff+nAppend,&ush,2);
                nAppend+=2;
                break;
            }
            case eCOLUMN_TYPE_BYTE:
            {
                char ch = (char)atoi(ch_temp);
                memcpy(ch_buff+nAppend,&ch,1);
                nAppend+=1;
                break;
            }
            case eCOLUMN_TYPE_DUMMY:
            {
                char ch = 0;
                memcpy(ch_buff+nAppend,&ch,1);
                nAppend+=1;
                break;
            }
            case eCOLUMN_TYPE_UINT:
            {
                unsigned int un = (unsigned int)atoi(ch_temp);
                if(m_parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
                    un = htonl(un);
                }
                memcpy(ch_buff+nAppend,&un,4);
                nAppend+=4;
                break;
            }
            case eCOLUMN_TYPE_NONE:
                break;
        }
    }
    
}
bool CToolInvoker::LoadMemory()
{
    FILE *fp;
    //fpos_t n_fsize;
    char ch_read[256];
    
    char **ch_args = m_parser->GetParseArgs();
	FILEOPENANDERROR(fp, ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA], "rb");
    while ( fgets(ch_read, 256, fp) != NULL ) {
        if(vInputBuff==NULL){
            vInputBuff = malloc(m_nRecordSize);
        }else{
			vInputBuff = realloc(vInputBuff, (m_nInputLine + 1)*m_nRecordSize);

        }
        SplitCanma(ch_read,m_nInputLine*m_nRecordSize);
        m_nInputLine++;
    }
    fclose(fp);
    return true;
}
bool CToolInvoker::LoadInputFile()
{
    // カラム定義に応じたメモリマップを作る.
    if(!MakeSchema()){
        return false;
    }
    
    // 入力ファイルをインプットに溜め込む.
    if(!LoadMemory()){
        return false;
    }
    
    return true;
    
}