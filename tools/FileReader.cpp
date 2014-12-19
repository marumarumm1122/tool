#include "FileReader.h"
#include "Tool.h"
#include <stdio.h>
#include <stdlib.h>
//指定されたパスのファイルの中身を指定されたバッファ用ポインタにセットする.
void CFileReader::Read(){
    if(e_format==CTool::eFORMAT_TEXT){
        TextRead();
        return;
    }
    
    BinaryRead();
    
}
void CFileReader::BinaryRead(){
    FILE *fp;
    fpos_t n_fsize;
    fp = fopen(ch_filePath,"rb");
    if(fp==NULL){
        return ;
    }
    
    fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fclose(fp);
    
    fp = fopen(ch_filePath,"rb");
    if(fp==NULL){
        return ;
    }
    
    pBuffer = malloc(n_fsize);
    fread(pBuffer,n_fsize,1,fp);
    fclose(fp);
}
void CFileReader::TextRead(){
    
}