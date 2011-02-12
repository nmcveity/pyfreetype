#include "pyfreetype_sizemetrics.h"
#include "pyfreetype_font.h"

#include "structmember.h"		// PyMemberDef, T_*

PyTypeObject pyfreetype_SizeMetricsType= {
	PyObject_HEAD_INIT(NULL)
	0,									/*ob_size*/
	"pyfreetype.SizeMetrics",			/*tp_name*/
	sizeof(pyfreetype_SizeMetrics),		/*tp_basicsize*/
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
	0,									/*tp_call*/
	0,									/*tp_str*/
	0,                         			/*tp_getattro*/
	0,                         			/*tp_setattro*/
	0,                         			/*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        			/*tp_flags*/
	"Size metrics",        				/*tp_doc */
};

static PyMemberDef metrics_members[] = {
	{"x_ppem",		T_INT,		offsetof(pyfreetype_SizeMetrics, m_x_ppem), READONLY, "Width of character, in points."},
	{"y_ppem",		T_INT,		offsetof(pyfreetype_SizeMetrics, m_y_ppem), READONLY, "Height of character, in points."},
	{"x_scale",		T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_x_scale), READONLY, "Horizontal Bearing X, in points."},
	{"y_scale",		T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_y_scale), READONLY, "Horizontal Bearing Y, in points."},
	{"ascender",	T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_ascender), READONLY, "Horizontal Advance, in points."},
	{"descender",	T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_descender), READONLY, "Vertical Bearing X, in points."},
	{"height",		T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_height), READONLY, "Vertical Bearing X, in points."},
	{"max_advance",	T_FLOAT,	offsetof(pyfreetype_SizeMetrics, m_max_advance), READONLY, "Vertical Advance, in points."},

	{NULL}
};

void pyfreetype_register_sizemetrics_type(PyObject * module)
{
	pyfreetype_SizeMetricsType.tp_new = PyType_GenericNew;
	pyfreetype_SizeMetricsType.tp_members = metrics_members;

	if (PyType_Ready(&pyfreetype_SizeMetricsType) < 0)
		return;

	// NICK: is this required if we dont register with module?
	Py_INCREF(&pyfreetype_SizeMetricsType);
}