#if !defined(__tool__Error__)
#define __tool__Error__
class CError
{

public:
	// エラー定数.
    static const short ERR_ARG_REQUIRED_PARAMETER_INPUT_FILE;
    static const short ERR_ARG_REQUIRED_PARAMETER_OUTPUT_FILE;
    static const short ERR_ARG_REQUIRED_PARAMETER_ENDIAN;
    static const short ERR_ARG_REQUIRED_PARAMETER_FILEVERSION;
    static const short ERR_ARG_ILLEGAL_PARAMETER_INPUT_FILE_VALUE;
    static const short ERR_ARG_ILLEGAL_PARAMETER_OUTPUT_FILE_VALUE;
    static const short ERR_ARG_ILLEGAL_PARAMETER_ENDIAN_VALUE;
    static const short ERR_ARG_ILLEGAL_PARAMETER_FILEVERSION_VALUE;
    static const short ERR_ARG_UNKNOWN_PARAMETER_ENDIAN_VALUE;
    static const short ERR_ARG_NOT_DIGIT_PARAMETER_FILEVERSION_VALUE;
    static const short ERR_INV_INPUT_FILE_NOT_OPEN;
    static const short ERR_INV_OUTPUT_FILE_NOT_OPEN;
    static const short ERR_INV_WORK_FILE_NOT_OPEN;

};
#endif // !defined(__tool__Error__)
