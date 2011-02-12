#ifndef INCLUDED_pyfreetype_sizemetrics_h
#define INCLUDED_pyfreetype_sizemetrics_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Python.h>

typedef struct {
	PyObject_HEAD

	/* copy of the values from the FT_SizeRec type */
	int m_x_ppem;
	int m_y_ppem;

	float m_x_scale;
	float m_y_scale;
	float m_ascender;
	float m_descender;
	float m_height;
	float m_max_advance;
} pyfreetype_SizeMetrics;

extern PyTypeObject pyfreetype_SizeMetricsType;
extern void pyfreetype_register_sizemetrics_type(PyObject * module);

#endif // INCLUDED_pyfreetype_glyphmetrics_h
