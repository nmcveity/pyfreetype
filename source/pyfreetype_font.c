#include "pyfreetype_font.h"
#include "pyfreetype_charmapiter.h"
#include "pyfreetype_glyphmetrics.h"
#include "pyfreetype_sizemetrics.h"
#include "pyfreetype_bitmapdata.h"
#include "pyfreetype_helpers.h"

#include "structmember.h"		// PyMemberDef, T_*

#include FT_BITMAP_H

extern FT_Library ftlib;

static PyObject * pyfreetype_Font_repr(pyfreetype_Font * self)
{
	return PyString_FromFormat("<font '%s'>", self->m_face->family_name);
}

PyTypeObject pyfreetype_FontType= {
	PyObject_HEAD_INIT(NULL)
	0,									/*ob_size*/
	"pyfreetype.Font",					/*tp_name*/
	sizeof(pyfreetype_Font),			/*tp_basicsize*/
	0,                         			/*tp_itemsize*/
	0,                         			/*tp_dealloc*/
	0,                         			/*tp_print*/
	0,                         			/*tp_getattr*/
	0,                         			/*tp_setattr*/
	0,                         			/*tp_compare*/
	(reprfunc)pyfreetype_Font_repr,     /*tp_repr*/
	0,                         			/*tp_as_number*/
	0,                         			/*tp_as_sequence*/
	0,                         			/*tp_as_mapping*/
	0,                         			/*tp_hash */
	0,                         			/*tp_call*/
	(reprfunc)pyfreetype_Font_repr,     /*tp_str*/
	0,                         			/*tp_getattro*/
	0,                         			/*tp_setattro*/
	0,                         			/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        			/*tp_flags*/
	"Font objects",           			/*tp_doc */
};

static PyObject * pyfreetype_Font_charmaps(PyObject *self, PyObject *args)
{
	pyfreetype_Font * font = (pyfreetype_Font*)self;
	pyfreetype_CharMapIter * iter = NULL;

	iter = PyObject_New(pyfreetype_CharMapIter, &pyfreetype_CharMapIterType);

	if (!iter) 
		return NULL;

	/* I'm not sure if it's strictly necessary. */
	if (!PyObject_Init((PyObject*)iter, &pyfreetype_CharMapIterType)) 
	{
		Py_DECREF(iter);
		return NULL;
	}

	iter->m_face = font;
	iter->m_index = 0;

	Py_INCREF(iter->m_face);

	return (PyObject *)iter;
}

static void pyfreetype_Font_update_size_metrics(pyfreetype_Font * font)
{
	pyfreetype_SizeMetrics * size_metrics = (pyfreetype_SizeMetrics *)font->m_size;

	if (size_metrics == NULL)
		size_metrics = PyObject_New(pyfreetype_SizeMetrics, &pyfreetype_SizeMetricsType);

	/*
	int m_x_ppem;
	int m_y_ppem;

	float m_x_scale;
	float m_y_scale;
	float m_ascender;
	float m_descender;
	float m_height;
	float m_max_advance;
*/
	size_metrics->m_x_ppem = font->m_face->size->metrics.x_ppem;
	size_metrics->m_y_ppem = font->m_face->size->metrics.y_ppem;
	size_metrics->m_x_scale = PYFREETYPE_16_16_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.x_scale);
	size_metrics->m_y_scale = PYFREETYPE_16_16_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.y_scale);
	size_metrics->m_ascender = PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.ascender);
	size_metrics->m_descender = PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.descender);
	size_metrics->m_height = PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.height);
	size_metrics->m_max_advance = PYFREETYPE_26_6_FRACTIONAL_TO_FLOAT(font->m_face->size->metrics.max_advance);
}

static PyObject * pyfreetype_Font_set_char_size(PyObject * self, PyObject * args)
{
	int char_width = 0;
	int char_height = 0;
	int horz_res = 0;
	int vert_res = 0;
	pyfreetype_Font * font = (pyfreetype_Font *)self;
	FT_Error res = 0;

	if (!PyArg_ParseTuple(args, "iiii", &char_width, &char_height, &horz_res, &vert_res))
	{
		return NULL;
	}

	res = FT_Set_Char_Size(font->m_face, char_width<<6, char_height<<6, horz_res, vert_res);

	if (res)
		return pyfreetype_SetErrorAndReturn("FT_Set_Char_Size", res);

	pyfreetype_Font_update_size_metrics(font);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * pyfreetype_Font_set_pixel_size(PyObject * self, PyObject * args)
{
	int char_width = 0;
	int char_height = 0;
	pyfreetype_Font * font = (pyfreetype_Font *)self;
	FT_Error res = 0;

	if (!PyArg_ParseTuple(args, "ii", &char_width, &char_height))
		return NULL;

	res = FT_Set_Pixel_Sizes(font->m_face, char_width, char_height);

	if (res)
		return pyfreetype_SetErrorAndReturn("FT_Set_Pixel_Sizes", res);

	pyfreetype_Font_update_size_metrics(font);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject * pyfreetype_Font_get_char_bitmap(PyObject * self, PyObject * args)
{
	PyObject * str = NULL;
	pyfreetype_Font * font = (pyfreetype_Font *)self;
	FT_UInt glyphIndex = 0;
	FT_Error res = 0;
	FT_Bitmap * ftbitmap = NULL;
	Py_UNICODE * buffer = NULL;
	PyObject * bytes = NULL;
	pyfreetype_BitmapData * bitmap = NULL;

	if (!PyArg_ParseTuple(args, "O", &str))
	{
		PyErr_SetString(PyExc_TypeError, "Pass in a string");
		return NULL;
	}

	if (str->ob_type != &PyUnicode_Type)
	{
		PyErr_SetString(PyExc_TypeError, "Not a unicode string");
		return NULL;
	}

	buffer = PyUnicode_AS_UNICODE(str);

	glyphIndex = FT_Get_Char_Index(font->m_face, buffer[0]);

	res = FT_Load_Glyph(font->m_face, glyphIndex, FT_LOAD_RENDER);

	if (res)
		return pyfreetype_SetErrorAndReturn("FT_Load_Glyph(FT_LOAD_RENDER)", res);

	ftbitmap = &font->m_face->glyph->bitmap;

	if ((ftbitmap->pixel_mode != FT_PIXEL_MODE_GRAY) || (ftbitmap->pitch != ftbitmap->width))
	{
		FT_Bitmap adjusted;

		FT_Bitmap_New(&adjusted);
		FT_Bitmap_Convert(ftlib, ftbitmap, &adjusted, 1);

		bytes = PyByteArray_FromStringAndSize(adjusted.buffer, adjusted.width * adjusted.rows);

		FT_Bitmap_Done(ftlib, &adjusted);
	}
	else
	{
		bytes = PyByteArray_FromStringAndSize(ftbitmap->buffer, ftbitmap->width * ftbitmap->rows);
	}

	bitmap = (pyfreetype_BitmapData *)PyObject_New(pyfreetype_BitmapData, &pyfreetype_BitmapDataType);

	if (font->m_face->glyph->format == FT_GLYPH_FORMAT_BITMAP)
	{
		bitmap->m_left = PyLong_FromLong(font->m_face->glyph->bitmap_left);
		bitmap->m_top = PyLong_FromLong(font->m_face->glyph->bitmap_top);
	}
	else
	{
		bitmap->m_left = Py_None;
		bitmap->m_top = Py_None;

		Py_INCREF(Py_None);
		Py_INCREF(Py_None);
	}

	bitmap->m_width = ftbitmap->width;
	bitmap->m_height = ftbitmap->rows;
	bitmap->m_pixels = bytes;

	return (PyObject *)bitmap;
}

static PyObject * pyfreetype_Font_get_char_metrics(PyObject * self, PyObject * args)
{
	PyObject * str = NULL;
	pyfreetype_Font * font = (pyfreetype_Font *)self;
	FT_UInt glyphIndex = 0;
	FT_Error res = 0;
	Py_UNICODE * buffer = NULL;
	pyfreetype_GlyphMetrics * metrics = NULL;

	if (!PyArg_ParseTuple(args, "O", &str))
	{
		PyErr_SetString(PyExc_TypeError, "Pass in a string");
		return NULL;
	}

	if (str->ob_type != &PyUnicode_Type)
	{
		PyErr_SetString(PyExc_TypeError, "Not a unicode string");
		return NULL;
	}

	buffer = PyUnicode_AS_UNICODE(str);

	glyphIndex = FT_Get_Char_Index(font->m_face, buffer[0]);

	res = FT_Load_Glyph(font->m_face, glyphIndex, 0);

	if (res)
		return pyfreetype_SetErrorAndReturn("FT_Load_Glyph", res);

	metrics = (pyfreetype_GlyphMetrics *)pyfreetype_GlyphMetricsType.tp_alloc(&pyfreetype_GlyphMetricsType, 0);
	metrics->m_width = font->m_face->glyph->metrics.width / 64.0f;
	metrics->m_height = font->m_face->glyph->metrics.height / 64.0f;
	metrics->m_horiBearingX = font->m_face->glyph->metrics.horiBearingX / 64.0f;
	metrics->m_horiBearingY = font->m_face->glyph->metrics.horiBearingY / 64.0f;
	metrics->m_horiAdvance = font->m_face->glyph->metrics.horiAdvance / 64.0f;
	metrics->m_vertBearingX = font->m_face->glyph->metrics.vertBearingX / 64.0f;
	metrics->m_vertBearingY = font->m_face->glyph->metrics.vertBearingY / 64.0f;
	metrics->m_vertAdvance = font->m_face->glyph->metrics.vertAdvance / 64.0f;
	metrics->m_linearHoriAdvance = font->m_face->glyph->linearHoriAdvance / 65536.0f;
	metrics->m_linearVertAdvance = font->m_face->glyph->linearVertAdvance / 65536.0f;
	metrics->m_advanceX = font->m_face->glyph->advance.x / 64.0f;
	metrics->m_advanceY = font->m_face->glyph->advance.y / 64.0f;

	return (PyObject*)metrics;
}

static PyObject * pyfreetype_Font_get_global_design_metrics(PyObject * self, PyObject * args)
{
	return NULL;
}

static PyObject * pyfreetype_Font_get_current_size_metrics(PyObject * self, PyObject * args)
{
	return NULL;
}

static PyObject * pyfreetype_Font_get_kerning(PyObject * self, PyObject * args)
{
	return NULL;
}

static PyMethodDef font_methods[] = {

	{"charmaps",					pyfreetype_Font_charmaps,					METH_VARARGS,	"Iterate all of the charmaps available to the font."},
	{"set_char_size",				pyfreetype_Font_set_char_size,				METH_VARARGS,	"Set the character size (in points) for subsequent operations."},
	{"set_pixel_size",				pyfreetype_Font_set_pixel_size,				METH_VARARGS,	"Set the character size (in pixels) for subsequent operations."},
	{"get_char_bitmap",				pyfreetype_Font_get_char_bitmap,			METH_VARARGS,	"Set the character size (in pixels) for subsequent operations."},
	{"get_char_metrics",			pyfreetype_Font_get_char_metrics,			METH_VARARGS,	"Returns a structure with information about the character specified."},
	{"get_global_design_metrics",	pyfreetype_Font_get_global_design_metrics,	METH_VARARGS,	"Returns a structure with information about the character specified."},
	{"get_current_size_metrics",	pyfreetype_Font_get_current_size_metrics,	METH_VARARGS,	"Returns a structure with information about the character specified."},
	{"get_kerning",					pyfreetype_Font_get_kerning,				METH_VARARGS,	"Returns a structure with information about the character specified."},

	{NULL}
};

static PyMemberDef font_members[] = {
	{"num_charmaps",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_numCharMaps), READONLY, "Number of charmaps in font."},
	{"family_name",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_familyName), READONLY, "Font family name."},
	{"style_name",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_styleName), READONLY, "Font style name."},

	/* face flags */
	{"is_flag_scalable",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_scalable), READONLY, "Font has has flag: FT_FACE_FLAG_SCALABLE."},
	{"is_flag_fixed_sizes",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fixed_sizes), READONLY, "Font has has flag: FT_FACE_FLAG_FIXED_SIZES."},
	{"is_flag_fixed_width",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fixed_width), READONLY, "Font has has flag: FT_FACE_FLAG_FIXED_WIDTH."},
	{"is_flag_sfnt",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_sfnt), READONLY, "Font has has flag: FT_FACE_FLAG_SFNT."},
	{"is_flag_horizontal",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_horizontal), READONLY, "Font has has flag: FT_FACE_FLAG_HORIZONTAL."},
	{"is_flag_vertical",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_vertical), READONLY, "Font has has flag: FT_FACE_FLAG_VERTICAL."},
	{"is_flag_kerning",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_kerning), READONLY, "Font has has flag: FT_FACE_FLAG_KERNING."},
	{"is_flag_fast_glyphs",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fast_glyphs), READONLY, "Font has has flag: FT_FACE_FLAG_FAST_GLYPHS."},
	{"is_flag_multiple_masters",	T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_multiple_masters), READONLY, "Font has has flag: FT_FACE_FLAG_MULTIPLE_MASTERS."},
	{"is_flag_names",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_names), READONLY, "Font has has flag: FT_FACE_FLAG_GLYPH_NAMES."},
	{"is_flag_external_stream",		T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_external_stream), READONLY, "Font has has flag: FT_FACE_FLAG_EXTERNAL_STREAM."},
	{"is_flag_hinter",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_hinter), READONLY, "Font has has flag: FT_FACE_FLAG_HINTER."},
	{"is_flag_cid_keyed",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_cid_keyed), READONLY, "Font has has flag: FT_FACE_FLAG_CID_KEYED."},
	{"is_flag_tricky",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_tricky), READONLY, "Font has has flag: FT_FACE_FLAG_TRICKY."},

	/* style flags */
	{"is_bold",						T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_bold), READONLY , "Font is flagged as bold"},
	{"is_italic",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_italic), READONLY , "Font is flagged as italic"},

	/* global metrics */
	{"units_per_EM",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_units_per_EM), READONLY, "Font is flagged as italic"},
	{"ascender",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_ascender), READONLY, "Font is flagged as italic"},
	{"descender",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_descender), READONLY, "Font is flagged as italic"},
	{"height",						T_OBJECT_EX,	offsetof(pyfreetype_Font, m_height), READONLY, "Font is flagged as italic"},
	{"max_advance_width",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_max_advance_width), READONLY, "Font is flagged as italic"},
	{"max_advance_height",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_max_advance_height), READONLY, "Font is flagged as italic"},
	{"underline_position",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_underline_position), READONLY, "Font is flagged as italic"},
	{"underline_thickness",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_underline_thickness), READONLY, "Font is flagged as italic"},
	{"bbox",						T_OBJECT_EX,	offsetof(pyfreetype_Font, m_bbox), READONLY, "Font is flagged as italic"},

	/* size metrics */
	{"size",						T_OBJECT_EX,	offsetof(pyfreetype_Font, m_size), READONLY, "Font is flagged as italic"},

	{NULL},
};

void pyfreetype_register_font_type(PyObject * module)
{
	pyfreetype_FontType.tp_new = PyType_GenericNew;
	pyfreetype_FontType.tp_methods = font_methods;
	pyfreetype_FontType.tp_members = font_members;

	if (PyType_Ready(&pyfreetype_FontType) < 0)
		return;

	Py_INCREF(&pyfreetype_FontType);
	PyModule_AddObject(module, "Font", (PyObject *)&pyfreetype_FontType);
}