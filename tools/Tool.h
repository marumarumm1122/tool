#if !defined(__tool__Tool__)
#define __tool__Tool__
#include <stdio.h>
namespace tool{
	class CTool
	{
	private:
		const int n_argc;
		char **ch_argv;
	public:
		static const char *CH_EXPORT_HEADER_FILE_NAME;
		enum eFORMAT{
    		eFORMAT_START = 0,

		    eFORMAT_TEXT = eFORMAT_START,
		    eFORMAT_BINARY ,

		    eFORMAT_NUM,
		    eFORMAT_MAX = eFORMAT_NUM - 1,
		};

		// メイン処理.
		void Main();

		CTool(int argc, char **argv):
			n_argc(argc)
			{
			ch_argv = argv;
		}
	private:
		void Error(const char *ch_message);
        bool ArgumentCheck();
	};
}
#endif // !defined(__tool__Tool__)
