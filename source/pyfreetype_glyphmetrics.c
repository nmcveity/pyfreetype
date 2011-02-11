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
	{"width",				T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_width), 0, "Width of character, in points."},
	{"height",				T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_height), 0, "Height of character, in points."},
	{"horiBearingX",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiBearingX), 0, "Horizontal Bearing X, in points."},
	{"horiBearingY",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiBearingY), 0, "Horizontal Bearing Y, in points."},
	{"horiAdvance",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_horiAdvance), 0, "Horizontal Advance, in points."},
	{"vertBearingX",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertBearingX), 0, "Vertical Bearing X, in points."},
	{"vertBearingY",		T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertBearingY), 0, "Vertical Bearing X, in points."},
	{"vertAdvance",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_vertAdvance), 0, "Vertical Advance, in points."},
	{"linearHoriAdvance",	T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_linearHoriAdvance), 0, "Horizontal Advance, in points."},
	{"linearVertAdvance",	T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_linearVertAdvance), 0, "Vertical Bearing X, in points."},
	{"advanceX",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_advanceX), 0, "Vertical Bearing X, in points."},
	{"advanceY",			T_FLOAT,	offsetof(pyfreetype_GlyphMetrics, m_advanceY), 0, "Vertical Advance, in points."},

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