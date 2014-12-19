#include "Converter.h"
#include <stdio.h>
#include <string.h>

using namespace tool;
namespace  {
    char* ch_headerBuff;
    char* ch_bodyBuff;
}
//コンバートしてバイナリ結果を変換先バッファにセットする.
void* CConverter::ToBinary(){
    int nLineCount = LineCount();
    int nHeaderBuffSize = sizeof(short)*2 + nLineCount*sizeof(int);
    ch_headerBuff = reinterpret_cast<char*>( malloc(nHeaderBuffSize));
    ch_bodyBuff = reinterpret_cast<char*>( malloc(npBufferSize));
    
    char *ch_pBuffer = reinterpret_cast<char*>(pBuffer);
    int nCnt = 0;
    int nPrevCnt = 0;
    do{
        
        if(ch_pBuffer[0]!='\n' && ch_pBuffer[0]!='\0'){
            ch_bodyBuff[0]='\0';
            ch_bodyBuff++;
            
            // エンディアン指定
            ch_headerBuff[0]=nCnt%16;
            ch_headerBuff++;
            ch_headerBuff[0]=nCnt/16;
            ch_headerBuff++;
            ch_headerBuff[0]=nCnt/(16*16);
            ch_headerBuff++;
            ch_headerBuff[0]=nCnt/(16*16*16);
            ch_headerBuff++;
            
            nCnt = 0;
            continue;
        }
        ch_bodyBuff[0] = ch_pBuffer[0];
        ch_bodyBuff++;
        nCnt+=sizeof(ch_pBuffer[0]);
    }while(*ch_pBuffer++);

    if(pConvertBuffer!=NULL){
        free(pConvertBuffer);
    }
    
    pConvertBuffer = malloc(nHeaderBuffSize + npBufferSize);

    memcpy(pConvertBuffer,ch_headerBuff,nHeaderBuffSize);

    memcpy(reinterpret_cast<char*>( pConvertBuffer)+npBufferSize,ch_bodyBuff,npBufferSize);
    
    free(ch_headerBuff);
    free(ch_bodyBuff);
    
    return pConvertBuffer;
}

int CConverter::LineCount(){
    int n_line = 0;
    npBufferSize = 0;
    char *ch_pBuffer = reinterpret_cast<char*>(pBuffer);
    do{
        npBufferSize++;
        if(ch_pBuffer[0]!='\n' && ch_pBuffer[0]!='\0'){
            n_line++;
            continue;
        }
    }while(*ch_pBuffer++);
    
    return n_line;
}