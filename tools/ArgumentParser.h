#if !defined(__tool__ArgumentParser__)
#define __tool__ArgumentParser__
#include <stdio.h>
class CArgumentParser
{
private:
	const int n_argc;
	char **ch_argv;

public:
	enum eFORMAT{
		eFORMAT_START = 0,

	    eFORMAT_TEXT = eFORMAT_START,
	    eFORMAT_BINARY ,

	    eFORMAT_NUM,
	    eFORMAT_MAX = eFORMAT_NUM - 1,
	};
	enum eBYTE_ORDER{
		eBYTE_ORDER_START = 0,

	    eBYTE_ORDER_BIGENDIAN = eBYTE_ORDER_START,
	    eBYTE_ORDER_LITTLEENDIAN ,

	    eBYTE_ORDER_NUM,
	    eBYTE_ORDER_MAX = eBYTE_ORDER_NUM - 1,
	};
	
	enum eARGUMENT{
		eARGUMENT_START = 0,

		eARGUMENT_INPUT_FILE_COMMAND = eARGUMENT_START,
		eARGUMENT_INPUT_FILE_DATA,
		eARGUMENT_OUTPUT_FILE_COMMAND,
		eARGUMENT_OUTPUT_FILE_DATA,
		eARGUMENT_ENDIAN_COMMAND,
		eARGUMENT_ENDIAN_DATA,
		eARGUMENT_VERSION_COMMAND,
		eARGUMENT_VERSION_DATA,

		eARGUMENT_NUM,
		eARGUMENT_MAX = eARGUMENT_NUM - 1,

	};
private:
	// 引数チェック用
	const char *ch_checkArgs[eARGUMENT_NUM];
	eBYTE_ORDER byteOrder;
public:
	CArgumentParser(int argc, char **argv):
		n_argc(argc)
		{
		ch_argv = argv;

	    for(int nCnt=0;nCnt<eARGUMENT_NUM;nCnt++){
	    	ch_checkArgs[nCnt] = NULL;
	    }

	}
	char **GetParseArgs(){
		return (char**)ch_checkArgs;
	}
	bool Parse();
	eBYTE_ORDER GetByteOrder(){
		return byteOrder;
	}
private:
	void Error(const char *ch_message);

	// 引数を解釈.
	bool ParseArguments();

	// 妥当性の検証.
    bool ArgumentCheck();
};
#endif // !defined(__tool__ArgumentParser__)
