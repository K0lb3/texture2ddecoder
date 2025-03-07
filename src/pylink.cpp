#define PY_SSIZE_T_CLEAN
#pragma once
#include <Python.h>
#include "bcn.h"
#include "pvrtc.h"
#include "etc.h"
#include "atc.h"
#include "astc.h"
#include "crunch.h"
#include "unitycrunch.h"

/*
 *************************************************
 *
 * general decoder templates
 *
 ************************************************
 */
typedef int (*decode_func_t)(const uint8_t *, const long, const long, uint32_t *);

template <decode_func_t DECODE>
static PyObject *decode(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    size_t data_size;
    uint32_t width, height;

    if (!PyArg_ParseTuple(args, "y#ii", &data, &data_size, &width, &height))
        return NULL;

    // reserve return image
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
        return PyErr_NoMemory();

    uint32_t *buf = (uint32_t *)PyBytes_AsString(res);

    // decode
    if (!DECODE(data, width, height, buf))
        return NULL;

    // return
    return res;
}

typedef int (*decode_func_bc_t)(const uint8_t *, uint32_t, uint32_t, uint32_t *);

template <decode_func_bc_t DECODE>
static PyObject *decode_bc(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    size_t data_size;
    uint32_t width, height;

    if (!PyArg_ParseTuple(args, "y#ii", &data, &data_size, &width, &height))
        return NULL;

    // reserve return image
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
        return PyErr_NoMemory();

    uint32_t *buf = (uint32_t *)PyBytes_AsString(res);

    // decode
    if (!DECODE(data, width, height, buf))
        return NULL;

    // return
    return res;
}

/*
 *************************************************
 *
 * decoder functions
 *  which don't use the general decoders
 ************************************************
 */

static PyObject *_decode_pvrtc(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    size_t data_size;
    uint32_t width, height;
    uint8_t is2bpp = 0;

    if (!PyArg_ParseTuple(args, "y#ii|b", &data, &data_size, &width, &height, &is2bpp))
        return NULL;

    // reserve return image
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
        return PyErr_NoMemory();

    uint32_t *buf = (uint32_t *)PyBytes_AsString(res);

    // decode
    if (!decode_pvrtc(data, width, height, buf, is2bpp ? 1 : 0))
        return NULL;

    // return
    return res;
}

static PyObject *_decode_astc(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    size_t data_size;
    uint32_t width, height, block_width, block_height;

    if (!PyArg_ParseTuple(args, "y#iiii", &data, &data_size, &width, &height, &block_width, &block_height))
        return NULL;

    // reserve return image - always BGRA
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
        return PyErr_NoMemory();

    uint32_t *buf = (uint32_t *)PyBytes_AsString(res);

    // decode
    if (!decode_astc(data, width, height, block_width, block_height, buf))
        return NULL;

    // return
    return res;
}

static PyObject *_unpack_crunch(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    uint32_t data_size;

    if (!PyArg_ParseTuple(args, "y#", &data, &data_size))
        return NULL;

    void *ret;
    uint32_t retSize;
    if (!crunch_unpack_level(data, data_size, 0, &ret, &retSize))
    {
        return NULL;
    }

    PyObject *res = Py_BuildValue("y#", ret, retSize);
    delete ret;
    return res;
}

static PyObject *_unpack_unity_crunch(PyObject *self, PyObject *args)
{
    // define vars
    const uint8_t *data;
    uint32_t data_size;

    if (!PyArg_ParseTuple(args, "y#", &data, &data_size))
        return NULL;

    void *ret;
    uint32_t retSize;
    if (!unity_crunch_unpack_level(data, data_size, 0, &ret, &retSize))
    {
        return NULL;
    }

    PyObject *res = Py_BuildValue("y#", ret, retSize);
    delete ret;
    return res;
}

/*
 *************************************************
 *
 * python connection
 *
 ************************************************
 */

// Exported methods are collected in a table
static struct PyMethodDef method_table[] = {
    {"decode_bc1",
     (PyCFunction)decode<decode_bc1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc3",
     (PyCFunction)decode<decode_bc3>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_pvrtc",
     (PyCFunction)_decode_pvrtc,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image, bool is2bpp"},
    {"decode_etc1",
     (PyCFunction)decode<decode_etc1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2",
     (PyCFunction)decode<decode_etc2>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2a1",
     (PyCFunction)decode<decode_etc2a1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2a8",
     (PyCFunction)decode<decode_etc2a8>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacr",
     (PyCFunction)decode<decode_eacr>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacr_signed",
     (PyCFunction)decode<decode_eacr_signed>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacrg",
     (PyCFunction)decode<decode_eacrg>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacrg_signed",
     (PyCFunction)decode<decode_eacrg_signed>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc4",
     (PyCFunction)decode_bc<decode_bc4>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc5",
     (PyCFunction)decode_bc<decode_bc5>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc6",
     (PyCFunction)decode_bc<decode_bc6>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc7",
     (PyCFunction)decode_bc<decode_bc7>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_atc_rgb4",
     (PyCFunction)decode_bc<decode_atc_rgb4>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_atc_rgba8",
     (PyCFunction)decode_bc<decode_atc_rgba8>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_astc",
     (PyCFunction)_decode_astc,
     METH_VARARGS,
     "bytes data, long w, long h, int bw, int bh, bytes image"},
    {"unpack_crunch",
     (PyCFunction)_unpack_crunch,
     METH_VARARGS,
     "bytes data"},
    {"unpack_unity_crunch",
     (PyCFunction)_unpack_unity_crunch,
     METH_VARARGS,
     "bytes data"},
    {NULL,
     NULL,
     0,
     NULL} // Sentinel value ending the table
};

// A struct contains the definition of a module
static PyModuleDef texture2ddecoder_module = {
    PyModuleDef_HEAD_INIT,
    "texture2ddecoder._texture2ddecoder", // Module name
    "a python wrapper for Perfare's Texture2DDecoder",
    -1, // Optional size of the module state memory
    method_table,
    NULL, // Optional slot definitions
    NULL, // Optional traversal function
    NULL, // Optional clear function
    NULL  // Optional module deallocation function
};

// The module init function
PyMODINIT_FUNC PyInit__texture2ddecoder(void)
{
    return PyModule_Create(&texture2ddecoder_module);
}