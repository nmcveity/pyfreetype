#ifndef INCLUDED_pyfreetype_helpers_h
#define INCLUDED_pyfreetype_helpers_h

#include <Python.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#define PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(x)  ((x) / 64.0f)
#define PYFREETYPE_16_16_FRACTIONAL_TO_FLOAT(x)  ((x) / 65536.0f)

extern void * pyfreetype_SetErrorAndReturn(const char * when, FT_Error code);
extern const char * pyfreetype_GetErrorCode(FT_Error code);

extern struct FT_LibraryRec_ * ftlib;
extern PyObject * FTErrorException;

#endif // INCLUDED_pyfreetype_helpers_h
