#if !defined(__tool__Converter__)
#define __tool__Converter__
#include <stdlib.h>

namespace tool{
	class CConverter
	{
	private:
		unsigned short ush_version;
		void *pBuffer;
		void *pConvertBuffer;
	public:
        
        CConverter(){}
        
        CConverter(unsigned short ush_version):
        ush_version(ush_version)
        ,pBuffer(NULL)
        ,pConvertBuffer(NULL){
        }
        CConverter(const char *ch_version):
        pBuffer(NULL)
        ,pConvertBuffer(NULL){
            this->ush_version = atoi(ch_version);
        }
        
		//バージョンをセットする.
		void SetVersion(unsigned short ush_version){
            this->ush_version = ush_version;
        }

		//変換もと文字列バッファをセットする.
		void SetBufferFrom(void *pBuffer){
            this->pBuffer = pBuffer;
        }

		//コンバートしてバイナリ結果を取得する.
		void* ToBinary();
    private:
        
        // 行数カウント
        int LineCount();
	};
}
#endif // !defined(__tool__Converter__)
