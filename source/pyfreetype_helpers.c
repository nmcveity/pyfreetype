#include "pyfreetype_helpers.h"

/* Apparently this is way to get to all the error messages in freetype, 
   as described in fterrors.h */

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  { e, s },
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       { 0, 0 } };

static const struct
{
	int          err_code;
	const char*  err_msg;
} ft_errors[] =

#include FT_ERRORS_H

const char * pyfreetype_GetErrorCode(FT_Error code)
{
	int i = 0;

	for (i = 0; ft_errors[i].err_msg; i++)
	{
		if (ft_errors[i].err_code == code)
			return ft_errors[i].err_msg;
	}

	return "unknown";
}

void * pyfreetype_SetErrorAndReturn(const char * when, FT_Error code)
{
	PyErr_SetObject(FTErrorException, PyString_FromFormat("%s: (%d) - %s", when, code, pyfreetype_GetErrorCode(code)));

	return NULL;
}
