#if !defined(__tool__Tool__)
#define __tool__Tool__
#include <stdio.h>
class CTool
{
private:
	const int n_argc;
	char **ch_argv;

public:
	// メイン処理.
	void Main();

	CTool(int argc, char **argv):
		n_argc(argc)
		{
		ch_argv = argv;
	}

};
#endif // !defined(__tool__Tool__)
