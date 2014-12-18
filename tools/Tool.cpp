#include "Tool.h"
#include <stdio.h>
#include <string.h>
#include "FileReader.h"
#include "FileWriter.h"
#include "Converter.h"
#include "HeaderGenerator.h"

using namespace tool;
namespace{
    char ch_buff[64];
    void *p_vbuffer;
    void *p_convertBuffer;
    void *p_headerBuffer;
    
}
const char *CTool::CH_EXPORT_HEADER_FILE_NAME = "S_LINK_DATA_STRING_HEADER.h";
void CTool::Main(){
    if(!ArgumentCheck()){
        return;
    }
    
    CFileReader *reader_instance = new CFileReader(ch_argv[2],CTool::eFORMAT_TEXT);
    reader_instance->SetBuffer(p_vbuffer);
    reader_instance->Read();
    
    CConverter *converter_instance = new CConverter(ch_argv[8]);
    converter_instance->SetBufferFrom(p_vbuffer);
    converter_instance->SetBufferTo(p_convertBuffer);
    converter_instance->ToBinary();
    
    CHeaderGenerator *generator_instance = new CHeaderGenerator();
    converter_instance->SetBufferFrom(p_vbuffer);
    converter_instance->SetBufferTo(p_headerBuffer);
    generator_instance->Generate();
    
    CFileWriter *writer_instance = new CFileWriter(ch_argv[4],CTool::eFORMAT_BINARY);
    writer_instance->SetBuffer(p_convertBuffer);
    writer_instance->Write();
    
    writer_instance->SetBuffer(p_headerBuffer);
    writer_instance->SetFormat(CTool::eFORMAT_TEXT);
    writer_instance->SetFilePath(CH_EXPORT_HEADER_FILE_NAME);
    writer_instance->Write();
    
}
bool CTool::ArgumentCheck(){
	if(n_argc<9){
		Error("argument Error");
		return false;
	}
	for (int nCnt = 0;nCnt < n_argc;nCnt++){
		switch(nCnt){
			case 0:
				break;
			case 1:{
				if(strncmp(ch_argv[nCnt],"-f",2)!=0){
					sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
					Error(ch_buff);
                    return false;
				}
				break;
			}
			case 3:{
                if(strncmp(ch_argv[nCnt],"-o",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
			}
			case 5:{
                if(strncmp(ch_argv[nCnt],"-e",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
			}
            case 6:{
                if(strncmp(ch_argv[nCnt],"be",2)!=0 && strncmp(ch_argv[nCnt],"le",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
            }
            case 7:{
                if(strncmp(ch_argv[nCnt],"-v",2)!=0){
                    sprintf(ch_buff,"Unknown option: %s",ch_argv[nCnt]);
                    Error(ch_buff);
                    return false;
                }
                break;
            }
			default:
				
				break;
		}
	}
    return true;
    
}
void CTool::Error(const char *ch_message){
	printf("%s\n",ch_message);
	printf("usage: test -f <file> -o <file> -e be/le -v <バージョン> \n\n");
	printf("-f 入力ファイル名\n");
	printf("-o 出力ファイル名\n");
	printf("-e be ビッグエンディアン指定（デフォルト）");
	printf(" / le リトルエンディアン指定\n");
    printf("-v バージョン\n");
}
