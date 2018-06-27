#include "Types.hpp"

PyObject * MGLContext_sampler(MGLContext * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

	const GLMethods & gl = self->gl;

	MGLSampler * sampler = (MGLSampler *)MGLSampler_Type.tp_alloc(&MGLSampler_Type, 0);

	sampler->sampler_obj = 0;

	// TODO: impl sampler

	Py_INCREF(self);
	sampler->context = self;

	Py_INCREF(sampler);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)sampler);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(sampler->sampler_obj));
	return result;
}

PyObject * MGLSampler_tp_new(PyTypeObject * type, PyObject * args, PyObject * kwargs) {
	MGLSampler * self = (MGLSampler *)type->tp_alloc(type, 0);

	if (self) {
	}

	return (PyObject *)self;
}

void MGLSampler_tp_dealloc(MGLSampler * self) {
	MGLSampler_Type.tp_free((PyObject *)self);
}

PyObject * MGLSampler_foo(MGLContext * self, PyObject * args) {
	int args_ok = PyArg_ParseTuple(
		args,
		""
	);

	if (!args_ok) {
		return 0;
	}

	// TODO: impl sampler

	Py_RETURN_NONE;
}

PyObject * MGLSampler_release(MGLSampler * self) {
	MGLSampler_Invalidate(self);
	Py_RETURN_NONE;
}

PyMethodDef MGLSampler_tp_methods[] = {
	{"foo", (PyCFunction)MGLSampler_foo, METH_VARARGS, 0},
	{"release", (PyCFunction)MGLSampler_release, METH_NOARGS, 0},
	{0},
};

PyObject * MGLSampler_get_bar(MGLSampler * self) {
	// TODO: impl sampler
	Py_RETURN_NONE;
}

int MGLSampler_set_bar(MGLSampler * self, PyObject * value) {
	// TODO: impl sampler
	return 0;
}

PyGetSetDef MGLSampler_tp_getseters[] = {
	{(char *)"bar", (getter)MGLSampler_get_bar, (setter)MGLSampler_set_bar, 0, 0},
	{0},
};

PyTypeObject MGLSampler_Type = {
	PyVarObject_HEAD_INIT(0, 0)
	"mgl.Sampler",                                          // tp_name
	sizeof(MGLSampler),                                     // tp_basicsize
	0,                                                      // tp_itemsize
	(destructor)MGLSampler_tp_dealloc,                      // tp_dealloc
	0,                                                      // tp_print
	0,                                                      // tp_getattr
	0,                                                      // tp_setattr
	0,                                                      // tp_reserved
	0,                                                      // tp_repr
	0,                                                      // tp_as_number
	0,                                                      // tp_as_sequence
	0,                                                      // tp_as_mapping
	0,                                                      // tp_hash
	0,                                                      // tp_call
	0,                                                      // tp_str
	0,                                                      // tp_getattro
	0,                                                      // tp_setattro
	0,                                                      // tp_as_buffer
	Py_TPFLAGS_DEFAULT,                                     // tp_flags
	0,                                                      // tp_doc
	0,                                                      // tp_traverse
	0,                                                      // tp_clear
	0,                                                      // tp_richcompare
	0,                                                      // tp_weaklistoffset
	0,                                                      // tp_iter
	0,                                                      // tp_iternext
	MGLSampler_tp_methods,                                  // tp_methods
	0,                                                      // tp_members
	MGLSampler_tp_getseters,                                // tp_getset
	0,                                                      // tp_base
	0,                                                      // tp_dict
	0,                                                      // tp_descr_get
	0,                                                      // tp_descr_set
	0,                                                      // tp_dictoffset
	0,                                                      // tp_init
	0,                                                      // tp_alloc
	MGLSampler_tp_new,                                      // tp_new
};

void MGLSampler_Invalidate(MGLSampler * sampler) {
	if (Py_TYPE(sampler) == &MGLInvalidObject_Type) {
		return;
	}

	// TODO: decref

	const GLMethods & gl = sampler->context->gl;
	gl.DeleteSamplers(1, (GLuint *)&sampler->sampler_obj);

	Py_TYPE(sampler) = &MGLInvalidObject_Type;
	Py_DECREF(sampler);
}
