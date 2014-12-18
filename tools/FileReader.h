#if !defined(__tool__FileReader__)
#define __tool__FileReader__
#include "Tool.h"
using namespace tool;
namespace tool{
	class CFileReader
	{
	private:
		CTool::eFORMAT e_format;
		void *pBuffer;
        const char *ch_filePath;
        bool b_isLoaded;
	public:
		
        CFileReader():
        b_isLoaded(false){}
        
        CFileReader(const char *ch_filePath,CTool::eFORMAT e_format):
        b_isLoaded(false){
            this->ch_filePath = ch_filePath;
            this->e_format = e_format;
        }
        
		//ファイルパスをセットする.
		void SetFilePath(const char *ch_filePath){
            this->ch_filePath = ch_filePath;
        }

		//フォーマットをセット(テキスト、バイナリ).
		void SetFormat(CTool::eFORMAT eformat){
            this->e_format = eformat;
        }
	
		//ファイルサイズをカウントする.
		int GetSize() const{
            if(!b_isLoaded){
                return 0;
            }
            return 0;
        }

		//ファイル行数をカウントする.
		int GetCount() const{
            if(!b_isLoaded){
                return 0;
            }
            return 0;
        }

		//バッファ用ポインタをセットする.
		void SetBuffer(void *pBuffer){
            this->pBuffer = pBuffer;
        }

		//指定されたパスのファイルの中身を指定されたバッファ用ポインタにセットする.
		void Read();


	};
}
#endif // !defined(__tool__FileReader__)
