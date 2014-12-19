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
        int n_size;
	public:
		
        CFileReader():
        b_isLoaded(false)
        ,pBuffer(NULL)
        ,n_size(0){}
        
        CFileReader(const char *ch_filePath,CTool::eFORMAT e_format):
        b_isLoaded(false)
        ,pBuffer(NULL)
        ,n_size(0){
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

		//指定されたパスのファイルの中身を指定されたバッファ用ポインタで取得する.
		void* Read();

	};
}
#endif // !defined(__tool__FileReader__)
