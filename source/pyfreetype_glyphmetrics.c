#include "pyfreetype_glyphmetrics.h"
#include "pyfreetype_font.h"

#include "structmember.h"		// PyMemberDef, T_*

PyTypeObject pyfreetype_GlyphMetricsType= {
	PyObject_HEAD_INIT(NULL)
	0,									/*ob_size*/
	"pyfreetype.GlyphMetrics",			/*tp_name*/
	sizeof(pyfreetype_GlyphMetrics),	/*tp_basicsize*/
	0,                         			/*tp_itemsize*/
	0,                         			/*tp_dealloc*/
	0,                         			/*tp_print*/
	0,                         			/*tp_getattr*/
	0,                         			/*tp_setattr*/
	0,                         			/*tp_compare*/
	0,									/*tp_repr*/
	0,                         			/*tp_as_number*/
	0,                         			/*tp_as_sequence*/
	0,                         			/*tp_as_mapping*/
	0,                         			/*tp_hash */
	0,                         			/*tp_call*/
	0,									/*tp_str*/
	0,                         			/*tp_getattro*/
	0,                         			/*tp_setattro*/
	0,                         			/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        			/*tp_flags*/
	"Glyph metrics",        			/*tp_doc */
};

static PyMethodDef metrics_methods[] = {
	{NULL}  /* Sentinel */
};

static PyMemberDef metrics_members[] = {
	{"width",				T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_width), READONLY, "Width of character, in points."},
	{"height",				T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_height), READONLY, "Height of character, in points."},
	{"horiBearingX",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiBearingX), READONLY, "Horizontal Bearing X, in points."},
	{"horiBearingY",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiBearingY), READONLY, "Horizontal Bearing Y, in points."},
	{"horiAdvance",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiAdvance), READONLY, "Horizontal Advance, in points."},
	{"vertBearingX",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertBearingX), READONLY, "Vertical Bearing X, in points."},
	{"vertBearingY",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertBearingY), READONLY, "Vertical Bearing X, in points."},
	{"vertAdvance",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertAdvance), READONLY, "Vertical Advance, in points."},
	{"linearHoriAdvance",	T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_linearHoriAdvance), READONLY, "Horizontal Advance, in points."},
	{"linearVertAdvance",	T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_linearVertAdvance), READONLY, "Vertical Bearing X, in points."},
	{"advanceX",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_advanceX), READONLY, "Vertical Bearing X, in points."},
	{"advanceY",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_advanceY), READONLY, "Vertical Advance, in points."},

	{NULL}
};

void pyfreetype_register_glyphmetrics_type(PyObject * module)
{
	pyfreetype_GlyphMetricsType.tp_new = PyType_GenericNew;
	pyfreetype_GlyphMetricsType.tp_members = metrics_members;

	if (PyType_Ready(&pyfreetype_GlyphMetricsType) < 0)
		return;

	// NICK: is this required if we dont register with module?
	Py_INCREF(&pyfreetype_GlyphMetricsType);
	//PyModule_AddObject(module, "Font", (PyObject *)&pyfreetype_CharMapIterType);
}