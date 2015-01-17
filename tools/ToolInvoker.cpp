#include <stdio.h>
#include "Error.h"
#include "Tool.h"
#include <string.h>
#include "ToolInvoker.h"
#include <stdlib.h>
#if defined(_WIN32)
#include <WinSock2.h>
#endif  // defined(_WIN32)

namespace{
    void *p_vbuffer = NULL;
    void *p_convertBuffer = NULL;
    int *n_headerList;
    int *n_startHeaderList;
    // インプットファイル用.
    void *p_vibuffer = NULL;
    // ワークファイル用.
    void *p_nbuffer = NULL;
}
const char *CToolInvoker::CH_EXPORT_HEADER_FILE_NAME = "S_LINK_DATA_STRING_HEADER.h";
const char *CToolInvoker::WORK_FILE_EXT_CONST_NAME = ".constn";
const char *CToolInvoker::WORK_FILE_EXT_CONST_VALUE = ".constv";

int CToolInvoker::Invoke(){
    // バージョンチェック.
    if(VersionCheck()){
        return 0;
    }
    
    // ヘルプチェック.
    if(HelpCheck()){
        return 0;
    }
    
    // 入力ファイルをオープンして二つの一時ファイルに振り分ける.
    if(!WriteWorkFile()){
        return sh_errorCode;
    }
    
	// 振り分けた一時ファイル二つ目をオープン.
    if(!Read()){
        return sh_errorCode;
    }
    
	// 振り分けた一時ファイル二つ目を変換.
    if(!Convert()){
        return sh_errorCode;
    }
    
    // 出力ファイルに書き込む.
    if(!Write()){
        return sh_errorCode;
    }
    
    // 振り分けた一時ファイル一つ目をもとに定数用ヘッダファイルを書き出す
    if(!WriteHeaderFile()){
        return sh_errorCode;
    }
    
    return 0;
}
bool CToolInvoker::WriteHeaderFile(){
    if(!WorkFileRead()){
        return false;
    }
    const char *ch_header = "//\n//  S_LINK_DATA_STRING_HEADER.h\n//\n// これはツールで自動出力されるものです.\n// 手動でいじるのは禁止です.\n#if !defined(__amida__S_LINK_DATA_STRING_HEADER__)\n#define __amida__S_LINK_DATA_STRING_HEADER__\n";
    const char *ch_data = "#define S_LINK_DATA_STRING_HEADER_";
    const char *ch_footer = "#endif // !defined(__amida__S_LINK_DATA_STRING_HEADER__)\n";
    
    FILE *fp;
    fp = fopen(CH_EXPORT_HEADER_FILE_NAME,"wb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",CH_EXPORT_HEADER_FILE_NAME);
        sh_errorCode = CError::ERR_INV_OUTPUT_FILE_NOT_OPEN;
        return false;
    }
    
    fwrite(ch_header, strlen(ch_header),1,fp);

    char *ch_pBuffer = reinterpret_cast<char*>(p_nbuffer);
    fwrite(ch_data,strlen(ch_data),1,fp);
    int nCnt = 0;
    do{
        if(ch_pBuffer[0]=='\n' || ch_pBuffer[0]=='\0'){
            fprintf(fp," %d\n",nCnt);
            nCnt++;
            if(nCnt >= n_line){
                break;
            }
            fwrite(ch_data,strlen(ch_data),1,fp);
            continue;
        }
        fwrite(ch_pBuffer,sizeof(char),1,fp);

    }while(*ch_pBuffer++);

    fwrite(ch_footer, strlen(ch_footer),1,fp);
    
    fclose(fp);
    return true;
}
bool CToolInvoker::WriteWorkFile(){
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
bool CToolInvoker::WorkFileRead(){
	char **ch_args = parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    char *workn = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workn,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workn,WORK_FILE_EXT_CONST_NAME);
    fp = fopen(workn,"rb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",workn);
        sh_errorCode = CError::ERR_INV_WORK_FILE_NOT_OPEN;
        return false;
    }
    free(workn);
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_nbuffer = malloc(n_fsize);
    fread(p_nbuffer,n_fsize,1,fp);
    
    fclose(fp);
    
    return true;
}

bool CToolInvoker::InputFileRead(){
    char **ch_args = parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    fp = fopen(ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA],"rb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA]);
        sh_errorCode = CError::ERR_INV_INPUT_FILE_NOT_OPEN;
        return false;
    }
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_vibuffer = malloc(n_fsize);
    fread(p_vibuffer,n_fsize,1,fp);
    
    fclose(fp);
    
    return true;
}
bool CToolInvoker::WriteTemporaryFile(){
    char **ch_args = parser->GetParseArgs();
    char *workn = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workn,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workn,WORK_FILE_EXT_CONST_NAME);
    char *workv = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workv,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workv,WORK_FILE_EXT_CONST_VALUE);
    
    FILE *fpc;
    fpc = fopen(workn,"wb");
    if(fpc==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",workn);
        sh_errorCode = CError::ERR_INV_WORK_FILE_NOT_OPEN;
        return false;
    }

    FILE *fpv;
    fpv = fopen(workv,"wb");
    if(fpv==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",workv);
        sh_errorCode = CError::ERR_INV_WORK_FILE_NOT_OPEN;
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
bool CToolInvoker::HelpCheck(){
    char **ch_args = parser->GetParseArgs();
    if(ch_args[CArgumentParser::eARGUMENT_HELP_COMMAND]!=NULL){
        parser->Usage();
        return true;
    }
    return false;
}

bool CToolInvoker::VersionCheck(){
    char **ch_args = parser->GetParseArgs();
    if(ch_args[CArgumentParser::eARGUMENT_VERSION_COMMAND]!=NULL){
        printf("tool version %s\n",CTool::TOOL_VERSION);
        return true;
    }
    return false;
}

bool CToolInvoker::Read(){
	char **ch_args = parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    char *workv = reinterpret_cast<char*>(malloc(sizeof(char)*100));
    strcpy(workv,ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
    strcat(workv,WORK_FILE_EXT_CONST_VALUE);
    fp = fopen(workv,"rb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",workv);
        sh_errorCode = CError::ERR_INV_WORK_FILE_NOT_OPEN;
        return false;
    }
    free(workv);
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_vbuffer = malloc(n_fsize);
    fread(p_vbuffer,n_fsize,1,fp);
    
    fclose(fp);
    p_convertBuffer = malloc(n_fsize+1);
    n_count = n_fsize;
    
    return true;
}
bool CToolInvoker::Convert(){
	// 行数カウント.
	return LineCount();
    
    
}
bool CToolInvoker::LineCount(){
    n_line = 0;
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
        
        n_headerList = reinterpret_cast<int*>(realloc(n_headerList,sizeof(int)*n_line));
        n_headerList[n_line] = n_bufferCount;
        
        printf("col count:%d\n",n_headerList[n_line]);
        n_line++;
        
    }while(*ch_pBuffer++);
    printf("count:%d\n",n_line);
    
    // 開始位置を入れる.
    n_startHeaderList = reinterpret_cast<int*>(malloc(sizeof(int)));
    n_startHeaderList[0]=0;
    for(int nCnt=1;nCnt<n_line;nCnt++){
        int n_headerTmp = n_headerList[nCnt-1];
        if(parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
	    	printf("convert BIG ENDIAN\n");
            n_headerTmp = htonl(n_headerTmp);
        }
        n_startHeaderList[nCnt] = n_headerTmp;
    }
    
    return true;
}
bool CToolInvoker::Write(){
    char **ch_args = parser->GetParseArgs();
    FILE *fp;
    fp = fopen(ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA],"wb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
        sh_errorCode = CError::ERR_INV_OUTPUT_FILE_NOT_OPEN;
        return false;
    }
    
    // write version.
    short *sh_version = reinterpret_cast<short*>(malloc(sizeof(short)));
    short n_versionTmp = atoi(ch_args[CArgumentParser::eARGUMENT_FILEVERSION_DATA]);
    if(parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
    	printf("convert BIG ENDIAN\n");
        n_versionTmp = htons(n_versionTmp);
    }else{
        //n_versionTmp = ntohs(n_versionTmp);
    }
    sh_version[0] = n_versionTmp;
    fwrite(sh_version,sizeof(short),1,fp);
    
    // write size.
    short *sh_size = reinterpret_cast<short*>(malloc(sizeof(short)));
    short n_sizeTmp = n_line;
    if(parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_BIGENDIAN){
    	printf("convert BIG ENDIAN\n");
        n_sizeTmp = htons(n_sizeTmp);
    }else if(parser->GetByteOrder()==CArgumentParser::eBYTE_ORDER_LITTLEENDIAN){
    	printf("convert LITTLE ENDIAN\n");
        //n_sizeTmp = ntohs(n_sizeTmp);
    }else{
        printf("error[%d]\n",parser->GetByteOrder());
    }
    sh_size[0] = n_sizeTmp;
    fwrite(sh_size,sizeof(short),1,fp);
    
    // write header.
    fwrite(n_startHeaderList,n_line*sizeof(int),1,fp);
    
    // write data.
    fwrite(p_convertBuffer, n_count,1,fp);
    
    fclose(fp);
    
    return true;
}