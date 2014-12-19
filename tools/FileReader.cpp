#include "FileReader.h"
#include "Tool.h"
#include <stdio.h>
#include <stdlib.h>
//指定されたパスのファイルの中身を指定されたバッファ用ポインタにセットする.
void* CFileReader::Read(){
    FILE *fp;
    fpos_t n_fsize;
    printf("file open\n");
    fp = fopen(ch_filePath,"rb");
    if(fp==NULL){
        printf("file error\n");
        return NULL;
    }
    
    printf("file seek\n");
    fpos_t fsizeb = fseek(fp,0,SEEK_END);
    fgetpos(fp, &n_fsize);
    fseek(fp,fsizeb,SEEK_SET);
    
    if(pBuffer!=NULL){
        free(pBuffer);
    }
    
    pBuffer = malloc(n_fsize);
    printf("size = %lld\n",n_fsize);
    printf("file read\n");
    fread(pBuffer,n_fsize,1,fp);
    
    printf("file close\n");
    fclose(fp);
    
    return pBuffer;
    
}
