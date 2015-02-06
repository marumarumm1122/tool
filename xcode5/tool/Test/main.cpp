#include <stdio.h>
#include <string.h>
#include <iostream>

typedef struct SLinkDataHeader{
public:
    unsigned short GetId() const{
        return ush_id;
    };
    unsigned short GetNo() const{
        return ush_no;
    }
    unsigned short GetNameid() const{
        return ush_nameId;
    }
    unsigned char GetCharaCategory() const{
        return ch_charaCategory;
    }
    unsigned char GetGrowType() const{
        return ch_growType;
    }
    unsigned char GetActionType() const{
        return ch_actionType;
    }
    unsigned int GetHP() const{
        return n_hp;
    }
    unsigned int GetMP() const{
        return n_mp;
    }
    unsigned short GetAttack() const{
        return ush_attack;
    }
    unsigned short GetDefense() const{
        return ush_defense;
    }
    unsigned short GetRapid() const{
        return ush_rapid;
    }
    unsigned short GetInteli() const{
        return ush_inteli;
    }
private:
    
    unsigned short ush_id;
    unsigned short ush_no;
    unsigned short ush_nameId;
    unsigned char ch_charaCategory;
    unsigned char ch_growType;
    unsigned char ch_actionType;
    unsigned char dummy1;
    unsigned char dummy2;
    unsigned char dummy3;
    unsigned int n_hp;
    unsigned int n_mp;
    unsigned short ush_attack;
    unsigned short ush_defense;
    unsigned short ush_rapid;
    unsigned short ush_inteli;
}S_LINK_DATA_HEADER;

int main(int argc, char **argv){
    fpos_t n_fsize ;
    
    FILE *fp;
    fp = fopen("character_data.bin","rb");
    if(fp==nullptr){
        return 1;
    }
    
    fseek(fp, 0, SEEK_END);
    fgetpos(fp, &n_fsize);
    fclose(fp);
    fp = fopen("character_data.bin","rb");
    if(fp==nullptr){
        return 1;
    }
    
    void *p_buff = malloc(n_fsize);
    unsigned short version =0;
    fread( &version,2,1,fp );
    fread( p_buff,n_fsize,1,fp );
    fclose(fp);
    
    S_LINK_DATA_HEADER *s_asHeader = reinterpret_cast<S_LINK_DATA_HEADER*>(p_buff) ;
    printf("sizeof=%lu\n",sizeof(S_LINK_DATA_HEADER));
    for(int nCnt = 0;nCnt < 3;nCnt++){
        printf("id = %d \n",s_asHeader[nCnt].GetId());
        printf("no = %d \n",s_asHeader[nCnt].GetNo());
        printf("nameid = %d \n",s_asHeader[nCnt].GetNameid());
        printf("characategory = %d \n",s_asHeader[nCnt].GetCharaCategory());
        printf("growtype = %d \n",s_asHeader[nCnt].GetGrowType());
        printf("actiontype = %d \n",s_asHeader[nCnt].GetActionType());
        printf("hp = %d \n",s_asHeader[nCnt].GetHP());
        printf("mp = %d \n",s_asHeader[nCnt].GetMP());
        printf("attack = %d \n",s_asHeader[nCnt].GetAttack());
        printf("defense = %d \n",s_asHeader[nCnt].GetDefense());
        printf("rapid = %d \n",s_asHeader[nCnt].GetRapid());
        printf("inteli = %d \n",s_asHeader[nCnt].GetInteli());
    }
    return 0;
}
