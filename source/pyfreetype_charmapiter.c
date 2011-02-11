#include "pyfreetype_charmapiter.h"
#include "pyfreetype_font.h"

static PyObject* pyfreetype_CharMapIter_iter(PyObject *self)
{
	Py_INCREF(self);
	return self;
}

static PyObject* pyfreetype_CharMapIter_iternext(PyObject *self)
{
	pyfreetype_CharMapIter * iter = (pyfreetype_CharMapIter*)self;

	if (iter->m_index < iter->m_face->m_face->num_charmaps)
	{
		FT_Encoding enc;

		switch (enc = iter->m_face->m_face->charmap[iter->m_index++].encoding)
		{
		case FT_ENCODING_NONE:
			return PyString_FromString("None");
		case FT_ENCODING_MS_SYMBOL:
			return PyString_FromString("MS Symbol");
		case FT_ENCODING_UNICODE:
			return PyString_FromString("Unicode");
		case FT_ENCODING_SJIS:
			return PyString_FromString("SJIS");
		case FT_ENCODING_GB2312:
			return PyString_FromString("GB2312");
		case FT_ENCODING_BIG5:
			return PyString_FromString("Big5");
		case FT_ENCODING_WANSUNG:
			return PyString_FromString("Wansung");
		case FT_ENCODING_JOHAB:
			return PyString_FromString("Johab");
		case FT_ENCODING_ADOBE_STANDARD:
			return PyString_FromString("Adobe Standard");
		case FT_ENCODING_ADOBE_EXPERT:
			return PyString_FromString("Adobe Expert");
		case FT_ENCODING_ADOBE_CUSTOM:
			return PyString_FromString("Adobe Custom");
		case FT_ENCODING_ADOBE_LATIN_1:
			return PyString_FromString("Adobe Latin 1");
		case FT_ENCODING_OLD_LATIN_2:
			return PyString_FromString("Old Latin 2");
		case FT_ENCODING_APPLE_ROMAN:
			return PyString_FromString("Apple Roman");
		}

		return PyString_FromFormat("ID: %x", enc);
	}
	else
	{
		PyErr_SetNone(PyExc_StopIteration);
		return NULL;
	}
}

PyTypeObject pyfreetype_CharMapIterType= {
	PyObject_HEAD_INIT(NULL)
	0,									/*ob_size*/
	"pyfreetype.CharMapIter",			/*tp_name*/
	sizeof(pyfreetype_CharMapIter),		/*tp_basicsize*/
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
	"Charmap iterator",        			/*tp_doc */
	0,									/* tp_traverse */
	0,									/* tp_clear */
	0,									/* tp_richcompare */
	0,									/* tp_weaklistoffset */
	pyfreetype_CharMapIter_iter,		/* tp_iter: __iter__() method */
	pyfreetype_CharMapIter_iternext,	/* tp_iternext: next() method */
};

static PyMethodDef iter_methods[] = {
	{NULL}  /* Sentinel */
};

void pyfreetype_register_charmapiter_type(PyObject * module)
{
	pyfreetype_CharMapIterType.tp_new = PyType_GenericNew;

	if (PyType_Ready(&pyfreetype_CharMapIterType) < 0)
		return;

	// NICK: is this required if we dont register with module?
	Py_INCREF(&pyfreetype_CharMapIterType);
	//PyModule_AddObject(module, "Font", (PyObject *)&pyfreetype_CharMapIterType);
}