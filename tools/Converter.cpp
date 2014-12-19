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

    ch_headerBuff = reinterpret_cast<char*>( malloc(sizeof(short)*2 + LineCount()*sizeof(int)));
    ch_bodyBuff = reinterpret_cast<char*>( malloc(sizeof(pBuffer)));
    
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
    pConvertBuffer = malloc(sizeof(ch_headerBuff) + sizeof(pBuffer));
    memcpy(pConvertBuffer,ch_headerBuff,sizeof(short)*2 + LineCount()*sizeof(int));
    
    memcpy(reinterpret_cast<char*>( pConvertBuffer)+sizeof(pBuffer),ch_bodyBuff,sizeof(pBuffer));
    
    free(ch_headerBuff);
    free(ch_bodyBuff);
    
    return pConvertBuffer;
}

int CConverter::LineCount(){
    int n_line = 0;
    char *ch_pBuffer = reinterpret_cast<char*>(pBuffer);
    do{
        
        if(ch_pBuffer[0]!='\n' && ch_pBuffer[0]!='\0'){
            n_line++;
            continue;
        }
        
    }while(*ch_pBuffer++);
    
    return n_line;
}