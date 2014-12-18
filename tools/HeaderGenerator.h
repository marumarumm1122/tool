#if !defined(__tool__HeaderGenerator__)
#define __tool__HeaderGenerator__

namespace tool{
	class CHeaderGenerator
	{
	private:
		void *pBuffer;
		void *pConvertBuffer;
	public:
        
        CHeaderGenerator(){}
        
		//変換もと文字列バッファをセットする.
		void SetBufferFrom(void *pBuffer){
            this->pBuffer = pBuffer;
        }

		//変換先格納バッファをセットする.
		void SetBufferTo(void *pBuffer){
            this->pConvertBuffer = pBuffer;
        }

		//ヘッダを作成してバッファに入れる.
		void Generate();


	};
}
#endif // !defined(__tool__HeaderGenerator__)
