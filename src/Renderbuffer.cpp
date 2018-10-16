#include "Types.hpp"

namespace cuda {
// CUDA headers
#include <cuda_runtime_api.h>
#include <cuda_gl_interop.h>
}

PyObject * MGLContext_renderbuffer(MGLContext * self, PyObject * args) {
	int width;
	int height;

	int components;

	int samples;

	const char * dtype;
	Py_ssize_t dtype_size;

	int args_ok = PyArg_ParseTuple(
		args,
		"(II)IIs#",
		&width,
		&height,
		&components,
		&samples,
		&dtype,
		&dtype_size
	);

	if (!args_ok) {
		return 0;
	}

	if (components < 1 || components > 4) {
		MGLError_Set("the components must be 1, 2, 3 or 4");
		return 0;
	}

	if ((samples & (samples - 1)) || samples > self->max_samples) {
		MGLError_Set("the number of samples is invalid");
		return 0;
	}

	if (dtype_size != 2) {
		MGLError_Set("invalid dtype");
		return 0;
	}

	MGLDataType * data_type = from_dtype(dtype);

	if (!data_type) {
		MGLError_Set("invalid dtype");
		return 0;
	}

	int format = data_type->internal_format[components];

	const GLMethods & gl = self->gl;

	MGLRenderbuffer * renderbuffer = (MGLRenderbuffer *)MGLRenderbuffer_Type.tp_alloc(&MGLRenderbuffer_Type, 0);

	renderbuffer->renderbuffer_obj = 0;
	gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	if (!renderbuffer->renderbuffer_obj) {
		MGLError_Set("cannot create renderbuffer");
		Py_DECREF(renderbuffer);
		return 0;
	}

	gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

	if (samples == 0) {
		gl.RenderbufferStorage(GL_RENDERBUFFER, format, width, height);
	} else {
		gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
	}

	renderbuffer->width = width;
	renderbuffer->height = height;
	renderbuffer->components = components;
	renderbuffer->samples = samples;
	renderbuffer->data_type = data_type;
	renderbuffer->depth = false;
	renderbuffer->cuda_graphics_resource = 0;

	Py_INCREF(self);
	renderbuffer->context = self;

	Py_INCREF(renderbuffer);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)renderbuffer);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(renderbuffer->renderbuffer_obj));
	return result;
}

PyObject * MGLContext_depth_renderbuffer(MGLContext * self, PyObject * args) {
	int width;
	int height;

	int samples;

	int args_ok = PyArg_ParseTuple(
		args,
		"(II)I",
		&width,
		&height,
		&samples
	);

	if (!args_ok) {
		return 0;
	}

	if ((samples & (samples - 1)) || samples > self->max_samples) {
		MGLError_Set("the number of samples is invalid");
		return 0;
	}

	const GLMethods & gl = self->gl;

	MGLRenderbuffer * renderbuffer = (MGLRenderbuffer *)MGLRenderbuffer_Type.tp_alloc(&MGLRenderbuffer_Type, 0);

	renderbuffer->renderbuffer_obj = 0;
	gl.GenRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	if (!renderbuffer->renderbuffer_obj) {
		MGLError_Set("cannot create renderbuffer");
		Py_DECREF(renderbuffer);
		return 0;
	}

	gl.BindRenderbuffer(GL_RENDERBUFFER, renderbuffer->renderbuffer_obj);

	if (samples == 0) {
		gl.RenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	} else {
		gl.RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, width, height);
	}

	renderbuffer->width = width;
	renderbuffer->height = height;
	renderbuffer->components = 1;
	renderbuffer->samples = samples;
	renderbuffer->data_type = from_dtype("f4");
	renderbuffer->depth = true;
	renderbuffer->cuda_graphics_resource = 0;

	Py_INCREF(self);
	renderbuffer->context = self;

	Py_INCREF(renderbuffer);

	PyObject * result = PyTuple_New(2);
	PyTuple_SET_ITEM(result, 0, (PyObject *)renderbuffer);
	PyTuple_SET_ITEM(result, 1, PyLong_FromLong(renderbuffer->renderbuffer_obj));
	return result;
}

PyObject * MGLRenderbuffer_tp_new(PyTypeObject * type, PyObject * args, PyObject * kwargs) {
	MGLRenderbuffer * self = (MGLRenderbuffer *)type->tp_alloc(type, 0);

	if (self) {
	}

	return (PyObject *)self;
}

void MGLRenderbuffer_tp_dealloc(MGLRenderbuffer * self) {
	MGLRenderbuffer_Type.tp_free((PyObject *)self);
}

PyObject * MGLRenderbuffer_release(MGLRenderbuffer * self) {
	MGLRenderbuffer_Invalidate(self);
	Py_RETURN_NONE;
}

#define CHECK_CUDA(x) \
	if (cuda::cudaError_t ret = (x)) \
		return PyErr_Format(PyExc_RuntimeError,  "CUDA error: %d.", ret);

PyObject * MGLRenderbuffer_cuda_map(MGLRenderbuffer * self, PyObject * args) {
	unsigned int array_index = 0;
	unsigned int mip_level = 0;

	int args_ok = PyArg_ParseTuple(
		args,
		"|II",
		&array_index,
		&mip_level
	);

	if (!args_ok) {
		PyErr_BadArgument();
		return 0;
	}

	cuda::cudaGraphicsResource_t resource =
		(cuda::cudaGraphicsResource_t) self->cuda_graphics_resource;

	if (resource == 0) {
		//CHECK_CUDA(cuda::cudaGLSetGLDevice(self->context->gl_context.cuda_device));
    	CHECK_CUDA(cuda::cudaGraphicsGLRegisterImage(
			&resource, self->renderbuffer_obj, GL_RENDERBUFFER,
			cuda::cudaGraphicsRegisterFlagsReadOnly));
	}

	const GLMethods & gl = self->context->gl;
	gl.BindRenderbuffer(GL_RENDERBUFFER, 0);

	CHECK_CUDA(cuda::cudaGraphicsMapResources(1, &resource));

	cuda::cudaArray_t array;
	CHECK_CUDA(cuda::cudaGraphicsSubResourceGetMappedArray(
		&array, resource, array_index, mip_level));

	return PyLong_FromLong((long)array);
}

PyObject * MGLRenderbuffer_cuda_unmap(MGLRenderbuffer * self) {
	cuda::cudaGraphicsResource_t resource =
		(cuda::cudaGraphicsResource_t) self->cuda_graphics_resource;

	if (resource != 0) {
		cuda::cudaGraphicsUnmapResources(1, &resource);

		const GLMethods & gl = self->context->gl;
		gl.BindRenderbuffer(GL_RENDERBUFFER, self->renderbuffer_obj);
	}

	return Py_None;
}

PyObject * MGLRenderbuffer_cuda_copy(MGLRenderbuffer * self, PyObject * args) {
	long data_ptr = 0;
	unsigned int cols = 1;

	int args_ok = PyArg_ParseTuple(
		args,
		"l|I",
		&data_ptr,
		&cols
	);

	if (!args_ok) {
		PyErr_BadArgument();
		return 0;
	}

	cuda::cudaGraphicsResource_t resource =
		(cuda::cudaGraphicsResource_t) self->cuda_graphics_resource;

	const GLMethods & gl = self->context->gl;

	if (resource == 0) {
    	CHECK_CUDA(cuda::cudaGraphicsGLRegisterImage(
			&resource, self->renderbuffer_obj, GL_RENDERBUFFER,
			cuda::cudaGraphicsRegisterFlagsReadOnly));
	}

	gl.BindRenderbuffer(GL_RENDERBUFFER, 0);

	CHECK_CUDA(cuda::cudaGraphicsMapResources(1, &resource));

	cuda::cudaArray_t array;
	CHECK_CUDA(cuda::cudaGraphicsSubResourceGetMappedArray(
		&array, resource, 0, 0));

	size_t width = self->width / cols;
	size_t height = self->height;
	size_t sz = 4 * self->data_type->size;

	for(size_t i = 0; i < cols; ++i)
		CHECK_CUDA(cuda::cudaMemcpy2DFromArrayAsync(
			(void*)(data_ptr + i * height * width * sz),
			width * sz, 	// dpitch
			array, 			// src
			i * width * sz, // w_offset,
			0, 				// h_offset,
			width * sz, 	// width of matrix transfer (columns in bytes)
			height, 		// height of matrix transfer (rows)
			cuda::cudaMemcpyDeviceToDevice
			));

	CHECK_CUDA(cuda::cudaGraphicsUnmapResources(1, &resource));

	gl.BindRenderbuffer(GL_RENDERBUFFER, self->renderbuffer_obj);

	return Py_None;
}

PyMethodDef MGLRenderbuffer_tp_methods[] = {
	{"release", (PyCFunction)MGLRenderbuffer_release, METH_NOARGS, 0},
	{"cuda_map", (PyCFunction)MGLRenderbuffer_cuda_map, METH_VARARGS, "Map buffer for access by CUDA and return array."},
	{"cuda_unmap", (PyCFunction)MGLRenderbuffer_cuda_unmap, METH_NOARGS, "Unmap graphics resources."},
    {"cuda_copy", (PyCFunction)MGLRenderbuffer_cuda_copy, METH_VARARGS, "Copy data to CUDA buffer"},
	{0},
};

PyTypeObject MGLRenderbuffer_Type = {
	PyVarObject_HEAD_INIT(0, 0)
	"mgl.Renderbuffer",                                     // tp_name
	sizeof(MGLRenderbuffer),                                // tp_basicsize
	0,                                                      // tp_itemsize
	(destructor)MGLRenderbuffer_tp_dealloc,                 // tp_dealloc
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
	MGLRenderbuffer_tp_methods,                             // tp_methods
	0,                                                      // tp_members
	0,                                                      // tp_getset
	0,                                                      // tp_base
	0,                                                      // tp_dict
	0,                                                      // tp_descr_get
	0,                                                      // tp_descr_set
	0,                                                      // tp_dictoffset
	0,                                                      // tp_init
	0,                                                      // tp_alloc
	MGLRenderbuffer_tp_new,                                 // tp_new
};

void MGLRenderbuffer_Invalidate(MGLRenderbuffer * renderbuffer) {
	if (Py_TYPE(renderbuffer) == &MGLInvalidObject_Type) {
		return;
	}

	// TODO: decref

	if (renderbuffer->cuda_graphics_resource) {
		cuda::cudaGraphicsResource_t resource =
			(cuda::cudaGraphicsResource_t) renderbuffer->cuda_graphics_resource;
		cuda::cudaGraphicsUnregisterResource(resource);
	}

	const GLMethods & gl = renderbuffer->context->gl;
	gl.DeleteRenderbuffers(1, (GLuint *)&renderbuffer->renderbuffer_obj);

	Py_TYPE(renderbuffer) = &MGLInvalidObject_Type;
	Py_DECREF(renderbuffer);
}
