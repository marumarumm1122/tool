#if !defined(__tool__Converter__)
#define __tool__Converter__

using namespace tool;
namespace tool{
	class CConverter
	{
	private:
		unsigned short ush_version;
		void *pBuffer;
		void *pConvertBuffer;
	public:

		//バージョンをセットする
		void SetVersion(unsigned short ush_version);

		//変換もと文字列バッファをセットする
		void SetBufferFrom(void *pBuffer);

		//変換先格納バッファをセットする
		void SetBufferTo(void *pBuffer);

		//コンバートしてバイナリ結果を変換先バッファにセットする
		void ToBinary();

		//コンバートした結果を文字列として変換先バッファにセットする
		void ToString();

	};
}
#endif // !defined(__tool__Converter__)
