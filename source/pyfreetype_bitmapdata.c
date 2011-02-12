#include "pyfreetype_bitmapdata.h"
#include "pyfreetype_font.h"

#include "structmember.h"		// PyMemberDef, T_*

static void pyfreetype_BitmapData_dealloc(pyfreetype_BitmapData * self)
{
	Py_DECREF(self->m_left);
	Py_DECREF(self->m_top);
	Py_DECREF(self->m_pixels);

	self->ob_type->tp_free((PyObject*)self);
}


PyTypeObject pyfreetype_BitmapDataType= {
	PyObject_HEAD_INIT(NULL)
	0,									/*ob_size*/
	"pyfreetype.BitmapData",			/*tp_name*/
	sizeof(pyfreetype_BitmapData),		/*tp_basicsize*/
	0,                         			/*tp_itemsize*/
	(destructor)pyfreetype_BitmapData_dealloc, 
										/*tp_dealloc*/
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
	"Bitmap Data",        				/*tp_doc */
};

static PyMemberDef bitmap_members[] = {
	{"width",	T_INT,			offsetof(pyfreetype_BitmapData, m_width), READONLY, "Width of character, in points."},
	{"height",	T_INT,			offsetof(pyfreetype_BitmapData, m_height), READONLY, "Height of character, in points."},
	{"left",	T_OBJECT_EX,	offsetof(pyfreetype_BitmapData, m_left), READONLY, "Horizontal Bearing X, in points."},
	{"top",		T_OBJECT_EX,	offsetof(pyfreetype_BitmapData, m_top), READONLY, "Horizontal Bearing Y, in points."},
	{"pixels",	T_OBJECT_EX,	offsetof(pyfreetype_BitmapData, m_pixels), READONLY, "Horizontal Advance, in points."},

	{NULL}
};

void pyfreetype_register_bitmapdata_type(PyObject * module)
{
	pyfreetype_BitmapDataType.tp_new = PyType_GenericNew;
	pyfreetype_BitmapDataType.tp_members = bitmap_members;

	if (PyType_Ready(&pyfreetype_BitmapDataType) < 0)
		return;

	// NICK: is this required if we don't register with module?
	Py_INCREF(&pyfreetype_BitmapDataType);
}