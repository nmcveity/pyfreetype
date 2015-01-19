#include <Python.h>

#include "pyfreetype_font.h"
#include "pyfreetype_charmapiter.h"
#include "pyfreetype_glyphmetrics.h"
#include "pyfreetype_bitmapdata.h"
#include "pyfreetype_sizemetrics.h"
#include "pyfreetype_helpers.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library ftlib = NULL;

PyObject * FTErrorException = NULL;

static PyObject * pyfreetype_ft_version(PyObject * self, PyObject * args)
{
	return Py_BuildValue("(iii)", FREETYPE_MAJOR, FREETYPE_MINOR, FREETYPE_PATCH);
}

#define PYFREETYPE_CHECK(x) if ((x) != NULL) ; else {Py_DECREF(font); return NULL;}

static PyObject * pyfreetype_open_font(PyObject * self, PyObject * args)
{
	pyfreetype_Font * font = NULL;
	const char * filename;
	int index = 0;
	FT_Error err = 0;

	if (!PyArg_ParseTuple(args, "s|i", &filename, &index))
		return NULL;

	font = pyfreetype_Font_new(&pyfreetype_FontType, 0, 0);

	if (font == NULL)
		return NULL;

	err = FT_New_Face(ftlib, filename, index, &font->m_face);

	if (err)
	{
		Py_DECREF(font);
		return pyfreetype_SetErrorAndReturn("FT_New_Face", err);
	}

	PYFREETYPE_CHECK(font->m_numCharMaps = PyInt_FromLong(font->m_face->num_charmaps));
	PYFREETYPE_CHECK(font->m_familyName = PyString_FromString(font->m_face->family_name));
	PYFREETYPE_CHECK(font->m_styleName = PyString_FromString(font->m_face->style_name));

	PYFREETYPE_CHECK(font->m_is_flag_scalable = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_SCALABLE));
	PYFREETYPE_CHECK(font->m_is_flag_fixed_sizes = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FIXED_SIZES));
	PYFREETYPE_CHECK(font->m_is_flag_fixed_width = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FIXED_WIDTH));
	PYFREETYPE_CHECK(font->m_is_flag_sfnt = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_SFNT));
	PYFREETYPE_CHECK(font->m_is_flag_horizontal = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_HORIZONTAL));
	PYFREETYPE_CHECK(font->m_is_flag_vertical = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_VERTICAL));
	PYFREETYPE_CHECK(font->m_is_flag_kerning = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_KERNING));
	PYFREETYPE_CHECK(font->m_is_flag_fast_glyphs = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_FAST_GLYPHS));
	PYFREETYPE_CHECK(font->m_is_flag_multiple_masters = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_MULTIPLE_MASTERS));
	PYFREETYPE_CHECK(font->m_is_flag_names = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_GLYPH_NAMES));
	PYFREETYPE_CHECK(font->m_is_flag_external_stream = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_EXTERNAL_STREAM));
	PYFREETYPE_CHECK(font->m_is_flag_hinter = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_HINTER));
	PYFREETYPE_CHECK(font->m_is_flag_cid_keyed = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_CID_KEYED));
	PYFREETYPE_CHECK(font->m_is_flag_tricky = PyBool_FromLong(font->m_face->face_flags & FT_FACE_FLAG_TRICKY));

	PYFREETYPE_CHECK(font->m_is_italic = PyBool_FromLong(font->m_face->style_flags & FT_STYLE_FLAG_ITALIC));
	PYFREETYPE_CHECK(font->m_is_bold = PyBool_FromLong(font->m_face->style_flags & FT_STYLE_FLAG_BOLD));

	PYFREETYPE_CHECK(font->m_bbox = Py_BuildValue("(ffff)", 
		PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->bbox.xMin), 
		PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->bbox.yMin), 
		PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->bbox.xMax), 
		PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->bbox.yMax)
	));

	PYFREETYPE_CHECK(font->m_units_per_EM = PyInt_FromLong(font->m_face->units_per_EM));
	PYFREETYPE_CHECK(font->m_ascender = PyInt_FromLong(font->m_face->ascender));
	PYFREETYPE_CHECK(font->m_descender = PyInt_FromLong(font->m_face->descender));
	PYFREETYPE_CHECK(font->m_height = PyInt_FromLong(font->m_face->height));
	PYFREETYPE_CHECK(font->m_max_advance_width = PyInt_FromLong(font->m_face->max_advance_width));
	PYFREETYPE_CHECK(font->m_max_advance_height = PyInt_FromLong(font->m_face->max_advance_height));
	PYFREETYPE_CHECK(font->m_underline_position = PyInt_FromLong(font->m_face->underline_position));
	PYFREETYPE_CHECK(font->m_underline_thickness = PyInt_FromLong(font->m_face->underline_thickness));

	font->m_size = NULL;

	return (PyObject*)font;
}

#undef PYFREETYPE_CHECK

static PyMethodDef ModuleMethods[] = 
{
	{"ft2_version",		pyfreetype_ft_version,	METH_VARARGS,	"Retrieve the freetype version used by this module."},
	{"open_font",		pyfreetype_open_font,	METH_VARARGS,	"Load a new font object and return it."},

	{NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initpyfreetype(void)
{
	PyObject * module = NULL;
	FT_Error res = FT_Init_FreeType(&ftlib);

	if (res)
	{
		PyErr_SetObject(PyExc_SystemError, PyString_FromFormat("Failed to initialise FT_Library: (%d) - %s", res, pyfreetype_GetErrorCode(res)));
		return;
	}

	module = Py_InitModule("pyfreetype", ModuleMethods);

	pyfreetype_register_font_type(module);
	pyfreetype_register_charmapiter_type(module);
	pyfreetype_register_glyphmetrics_type(module);
	pyfreetype_register_bitmapdata_type(module);
	pyfreetype_register_sizemetrics_type(module);

	FTErrorException = PyErr_NewException("pyfreetype.FTError", NULL, NULL);
	Py_INCREF(FTErrorException);
}
