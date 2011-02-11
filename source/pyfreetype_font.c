#include "pyfreetype_font.h"
#include "pyfreetype_charmapiter.h"

#include "structmember.h"		// PyMemberDef, T_*

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

static PyMethodDef font_methods[] = {
	{"charmaps",  pyfreetype_Font_charmaps, METH_VARARGS, "Iterate all of the charmaps available to the font."},

	{NULL}
};

static PyMemberDef font_members[] = {
	{"num_charmaps",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_numCharMaps), 0, "Number of charmaps in font."},
	{"family_name",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_familyName), 0, "Font family name."},
	{"style_name",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_styleName), 0, "Font style name."},

	/* face flags */
	{"is_flag_scalable",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_scalable), 0, "Font has has flag: FT_FACE_FLAG_SCALABLE."},
	{"is_flag_fixed_sizes",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fixed_sizes), 0, "Font has has flag: FT_FACE_FLAG_FIXED_SIZES."},
	{"is_flag_fixed_width",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fixed_width), 0, "Font has has flag: FT_FACE_FLAG_FIXED_WIDTH."},
	{"is_flag_sfnt",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_sfnt), 0, "Font has has flag: FT_FACE_FLAG_SFNT."},
	{"is_flag_horizontal",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_horizontal), 0, "Font has has flag: FT_FACE_FLAG_HORIZONTAL."},
	{"is_flag_vertical",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_vertical), 0, "Font has has flag: FT_FACE_FLAG_VERTICAL."},
	{"is_flag_kerning",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_kerning), 0, "Font has has flag: FT_FACE_FLAG_KERNING."},
	{"is_flag_fast_glyphs",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_fast_glyphs), 0, "Font has has flag: FT_FACE_FLAG_FAST_GLYPHS."},
	{"is_flag_multiple_masters",	T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_multiple_masters), 0, "Font has has flag: FT_FACE_FLAG_MULTIPLE_MASTERS."},
	{"is_flag_names",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_names), 0, "Font has has flag: FT_FACE_FLAG_GLYPH_NAMES."},
	{"is_flag_external_stream",		T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_external_stream), 0, "Font has has flag: FT_FACE_FLAG_EXTERNAL_STREAM."},
	{"is_flag_hinter",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_hinter), 0, "Font has has flag: FT_FACE_FLAG_HINTER."},
	{"is_flag_cid_keyed",			T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_cid_keyed), 0, "Font has has flag: FT_FACE_FLAG_CID_KEYED."},
	{"is_flag_tricky",				T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_flag_tricky), 0, "Font has has flag: FT_FACE_FLAG_TRICKY."},

	/* style flags */
	{"is_bold",						T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_bold), 0, "Font is flagged as bold"},
	{"is_italic",					T_OBJECT_EX,	offsetof(pyfreetype_Font, m_is_italic), 0, "Font is flagged as italic"},

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