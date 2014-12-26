#if !defined(__tool__ToolInvoker__)
#define __tool__ToolInvoker__
#include <stdio.h>
#include "ArgumentParser.h"
namespace tool{
	class CToolInvoker
	{
	private:
		CArgumentParser *parser;
		static const char *CH_EXPORT_HEADER_FILE_NAME;
	public:
		CToolInvoker(CArgumentParser *parser):
			parser(parser)
			{
		}
		int Invoke();

	};
}
#endif // !defined(__tool__ToolInvoker__)
