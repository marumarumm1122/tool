#include <stdio.h>
#include <string.h>
#include <iostream>

typedef struct SLinkDataHeader{
public:
    unsigned short GetVersion() const{
        return ush_verion;
    };
    unsigned short GetSize() const{
        return ushDataCount;
        
    }
    int* GetHeaderTop() const{
        return (int*)pHeaderTop;
    }
private:
    unsigned short ush_verion;
    unsigned short ushDataCount;
    int pHeaderTop[1];
}S_LINK_DATA_HEADER;

int main(int argc, char **argv){
    fpos_t n_fsize ;
    
    FILE *fp;
    fp = fopen("string_table.bin","rb");
    if(fp==nullptr){
        return 1;
    }
    
    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &n_fsize);
    fclose(fp);
    fp = fopen("string_table.bin","rb");
    if(fp==nullptr){
        return 1;
    }
    
    void *p_buff = malloc(n_fsize);
    fread( p_buff,n_fsize,1,fp );
    fclose(fp);
    
    S_LINK_DATA_HEADER *s_asHeader = reinterpret_cast<S_LINK_DATA_HEADER*>(p_buff) ;
    printf("version = %d size = %d\n",s_asHeader->GetVersion(),s_asHeader->GetSize());
    return 0;
}

