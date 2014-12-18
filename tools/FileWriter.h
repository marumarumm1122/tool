#if !defined(__tool__FileWriter__)
#define __tool__FileWriter__

#include "Tool.h"

namespace tool{
	class CFileWriter
	{
	private:
		const char *ch_filepath;
		CTool::eFORMAT eformat;
		void *pBuffer;
	public:
        
        CFileWriter(){};
        
        CFileWriter(const char *filePath,CTool::eFORMAT eformat):
        ch_filepath(filePath),
        eformat(eformat){
            
        }
		//ファイルパスをセットする.
		void SetFilePath(const char *filePath){
            this->ch_filepath = filePath;
        }

		//フォーマットをセット(テキスト、バイナリ).
		void SetFormat(CTool::eFORMAT eformat){
            this->eformat = eformat;
        }
	
		//バッファ用ポインタをセットする.
		void SetBuffer(void *pBuffer){
            this->pBuffer = pBuffer;
        }

		//指定されたポインタの中身を指定されたパスに書き込む.
		void Write();


	};
}
#endif // !defined(__tool__FileWriter__)
