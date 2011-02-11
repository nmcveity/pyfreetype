#ifndef INCLUDED_pyfreetype_charmapiter_h
#define INCLUDED_pyfreetype_charmapiter_h

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Python.h>

typedef struct {
	PyObject_HEAD

	struct pyfreetype_Font_ * m_face;
	int m_index;
} pyfreetype_CharMapIter;

extern PyTypeObject pyfreetype_CharMapIterType;
extern void pyfreetype_register_charmapiter_type(PyObject * module);

#endif // INCLUDED_pyfreetype_charmapiter_h
