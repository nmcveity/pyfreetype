#ifndef INCLUDED_pyfreetype_font_h
#define INCLUDED_pyfreetype_font_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Python.h>

typedef struct pyfreetype_Font_ {
	PyObject_HEAD

	FT_Face m_face;

	PyObject * m_numCharMaps;
	PyObject * m_familyName;
	PyObject * m_styleName;

	/* flags, all boolean */
	PyObject * m_is_flag_scalable;
	PyObject * m_is_flag_fixed_sizes;
	PyObject * m_is_flag_fixed_width;
	PyObject * m_is_flag_sfnt;
	PyObject * m_is_flag_horizontal;
	PyObject * m_is_flag_vertical;
	PyObject * m_is_flag_kerning;
	PyObject * m_is_flag_fast_glyphs;
	PyObject * m_is_flag_multiple_masters;
	PyObject * m_is_flag_names;
	PyObject * m_is_flag_external_stream;
	PyObject * m_is_flag_hinter;
	PyObject * m_is_flag_cid_keyed;
	PyObject * m_is_flag_tricky;

	/* style flags, boolean */
	PyObject * m_is_bold;
	PyObject * m_is_italic;

	/* global metrics */
	PyObject * m_bbox;
	PyObject * m_units_per_EM;
	PyObject * m_ascender;
	PyObject * m_descender;
	PyObject * m_height;
	PyObject * m_max_advance_width;
	PyObject * m_max_advance_height;
	PyObject * m_underline_position;
	PyObject * m_underline_thickness;

	/* size metrics */
	PyObject * m_size;
} pyfreetype_Font;

extern PyTypeObject pyfreetype_FontType;
extern void pyfreetype_register_font_type(PyObject * module);

#endif // INCLUDED_pyfreetype_font_h
