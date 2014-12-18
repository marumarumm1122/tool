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
        ush_version(ush_version){
        }
        CConverter(const char *ch_version){
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

		//変換先格納バッファをセットする.
		void SetBufferTo(void *pBuffer){
            this->pConvertBuffer = pBuffer;
        }

		//コンバートしてバイナリ結果を変換先バッファにセットする.
		void ToBinary();

		//コンバートした結果を文字列として変換先バッファにセットする.
		void ToString();

	};
}
#endif // !defined(__tool__Converter__)
