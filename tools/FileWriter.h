#if !defined(__tool__FileWriter__)
#define __tool__FileWriter__

using namespace tool;
namespace tool{
	class CFileWriter
	{
	private:
		const char *filepath;
		CTool::eFORMAT eformat;
		void *pBuffer;
	public:

		//ファイルパスをセットする
		void SetFilePath(const char *filePath);

		//フォーマットをセット(テキスト、バイナリ)
		void SetFormat(CTool::eFORMAT eformat);
	
		//バッファ用ポインタをセットする
		void SetBuffer(void *pBuffer);

		//指定されたポインタの中身を指定されたパスに書き込む
		void Write();


	};
}
#endif // !defined(__tool__FileWriter__)
