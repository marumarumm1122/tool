#if !defined(__tool__FileReader__)
#define __tool__FileReader__

using namespace tool;
namespace tool{
	class CFileReader
	{
	private:
		CTool::eFORMAT e_format;
		void *pBuffer;
	public:
		
		//ファイルパスをセットする
		void SetFilePath(const char *ch_filePath);

		//フォーマットをセット(テキスト、バイナリ)
		void SetFormat(CTool::eFORMAT eformat);
	
		//ファイルサイズをカウントする
		int GetSize() const;

		//ファイル行数をカウントする
		int GetCount() const;

		//バッファ用ポインタをセットする
		void SetBuffer(void *pBuffer);

		//指定されたパスのファイルの中身を指定されたバッファ用ポインタにセットする
		void Read();


	};
}
#endif // !defined(__tool__FileReader__)
