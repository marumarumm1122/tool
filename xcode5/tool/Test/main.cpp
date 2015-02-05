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
    fread( p_buff,n_fsize,1,fp );
    fclose(fp);
    
    S_LINK_DATA_HEADER *s_asHeader = reinterpret_cast<S_LINK_DATA_HEADER*>(p_buff) ;
    printf("id = %d \n",s_asHeader->GetId());
    printf("no = %d \n",s_asHeader->GetNo());
    printf("nameid = %d \n",s_asHeader->GetNameid());
    printf("characategory = %d \n",s_asHeader->GetCharaCategory());
    printf("growtype = %d \n",s_asHeader->GetGrowType());
    printf("actiontype = %d \n",s_asHeader->GetActionType());
    printf("hp = %d \n",s_asHeader->GetHP());
    printf("mp = %d \n",s_asHeader->GetMP());
    printf("attack = %d \n",s_asHeader->GetAttack());
    printf("defense = %d \n",s_asHeader->GetDefense());
    printf("rapid = %d \n",s_asHeader->GetRapid());
    printf("inteli = %d \n",s_asHeader->GetInteli());
    return 0;
}
