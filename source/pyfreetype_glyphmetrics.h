#ifndef INCLUDED_pyfreetype_glyphmetrics_h
#define INCLUDED_pyfreetype_glyphmetrics_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Python.h>

typedef struct {
	PyObject_HEAD

	/* copy of the values from the FT_Glyph_Metrics type */
	float m_width;
	float m_height;

	float m_horiBearingX;
	float m_horiBearingY;
	float m_horiAdvance;

	float m_vertBearingX;
	float m_vertBearingY;
	float m_vertAdvance;

	float m_linearHoriAdvance;
	float m_linearVertAdvance;

	float m_advanceX;
	float m_advanceY;
} pyfreetype_GlyphMetrics;

extern PyTypeObject pyfreetype_GlyphMetricsType;
extern void pyfreetype_register_glyphmetrics_type(PyObject * module);

#endif // INCLUDED_pyfreetype_glyphmetrics_h
