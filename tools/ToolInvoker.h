#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
class CToolInvoker
{
private:
	CArgumentParser *parser;
	static const char *CH_EXPORT_HEADER_FILE_NAME;
	
	// データ行数.
	int n_line;

	// データ個数.
	int n_count;

	void Read();
	void Convert();
	void LineCount();
	void Write();
public:
	CToolInvoker(CArgumentParser *parser):
		parser(parser),
		n_line(0),
		n_count(0)
		{
	}
	int Invoke();

};
#endif // !defined(__tool__ToolInvoker__)
