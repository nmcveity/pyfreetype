#ifndef INCLUDED_pyfreetype_bitmapdata_h
#define INCLUDED_pyfreetype_bitmapdata_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Python.h>

typedef struct pyfreetype_BitmapData_ {
	PyObject_HEAD

	/* copy of the values from the FT_Bitmap type */
	int m_width;
	int m_height;

	PyObject * m_left;		/* not int because it can be None */
	PyObject * m_top;		/* not int because it can be None */

	PyObject * m_pixels;
} pyfreetype_BitmapData;

extern PyTypeObject pyfreetype_BitmapDataType;
extern void pyfreetype_register_bitmapdata_type(PyObject * module);

#endif // INCLUDED_pyfreetype_bitmapdata_h
