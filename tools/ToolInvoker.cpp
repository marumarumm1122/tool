#include <stdio.h>
#include "Error.h"
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
    
}
const char *CToolInvoker::CH_EXPORT_HEADER_FILE_NAME = "S_LINK_DATA_STRING_HEADER.h";

int CToolInvoker::Invoke(){
    // バージョンチェック.
    if(VersionCheck()){
        return 0;
    }

    // ヘルプチェック.
    if(HelpCheck()){
        return 0;
    }

	// 入力ファイルをオープン.
	Read();

	// 入力データを変換.
	Convert();

    // 出力ファイルに書き込む.
    Write();

    return 0;
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
        printf("tool version 1.0\n");
        return true;
    }
    return false;
}

void CToolInvoker::Read(){
	char **ch_args = parser->GetParseArgs();
	FILE *fp;
    fpos_t n_fsize;
    fp = fopen(ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA],"rb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",ch_args[CArgumentParser::eARGUMENT_INPUT_FILE_DATA]);
        sh_errorCode = CError::ERR_INV_INPUT_FILE_NOT_OPEN;
        return ;
    }
    
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    p_vbuffer = malloc(n_fsize);
    fread(p_vbuffer,n_fsize,1,fp);
    
    fclose(fp);
    p_convertBuffer = malloc(n_fsize+1);
    n_count = n_fsize;
}
void CToolInvoker::Convert(){
	// 行数カウント.
	LineCount();


}
void CToolInvoker::LineCount(){
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
}
void CToolInvoker::Write(){
    char **ch_args = parser->GetParseArgs();
    FILE *fp;
    fp = fopen(ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA],"wb");
    if(fp==NULL){
        printf("ファイルエラーが発生しました:開こうとしたファイル[%s]\n",ch_args[CArgumentParser::eARGUMENT_OUTPUT_FILE_DATA]);
        sh_errorCode = CError::ERR_INV_OUTPUT_FILE_NOT_OPEN;
        return ;
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

}
