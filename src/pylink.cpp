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
template <auto DecodeFunc, typename DimensionType, char DimensionChar>
static PyObject *decode(PyObject *self, PyObject *args)
{
    // define vars
    Py_buffer view;
    DimensionType width, height;
    char fmt[] = {'y', '*', DimensionChar, DimensionChar, '\0'};

    if (!PyArg_ParseTuple(args, fmt, &view, &width, &height))
        return NULL;

    // reserve return image
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
    {
        PyBuffer_Release(&view);
        return PyErr_NoMemory();
    }
    // decode
    const uint8_t *texture_data = reinterpret_cast<const uint8_t *>(view.buf);
    uint32_t *image_data = reinterpret_cast<uint32_t *>(PyBytes_AsString(res));
    auto decode_result = DecodeFunc(texture_data, width, height, image_data);
    PyBuffer_Release(&view);

    // check decode result
    if (!decode_result)
    {
        Py_DECREF(res);
        PyErr_SetString(PyExc_RuntimeError, "Decoding failed");
        return NULL;
    }
    // return
    return res;
}

template <auto DecodeFunc>
auto decode_l = decode<DecodeFunc, long, 'l'>;

template <auto DecodeFunc>
auto decode_u32 = decode<DecodeFunc, int, 'I'>;

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
    Py_buffer view;
    long width, height;
    uint8_t is2bpp = 0;

    if (!PyArg_ParseTuple(args, "y*ll|b", &view, &width, &height, &is2bpp))
        return NULL;

    // reserve return image
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
    {
        PyBuffer_Release(&view);
        return PyErr_NoMemory();
    }
    // decode
    const uint8_t *texture_data = reinterpret_cast<const uint8_t *>(view.buf);
    uint32_t *image_data = reinterpret_cast<uint32_t *>(PyBytes_AsString(res));
    auto decode_result = decode_pvrtc(texture_data, width, height, image_data, is2bpp ? 1 : 0);
    PyBuffer_Release(&view);
    // check decode result
    if (!decode_result)
    {
        Py_DECREF(res);
        PyErr_SetString(PyExc_RuntimeError, "Decoding failed");
        return NULL;
    }
    // return
    return res;
}

static PyObject *_decode_astc(PyObject *self, PyObject *args)
{
    // define vars
    Py_buffer view;
    uint32_t width, height, block_width, block_height;

    if (!PyArg_ParseTuple(args, "y*IIII", &view, &width, &height, &block_width, &block_height))
        return NULL;

    // reserve return image - always BGRA
    PyObject *res = PyBytes_FromStringAndSize(nullptr, width * height * 4);
    if (res == NULL)
    {
        PyBuffer_Release(&view);
        return PyErr_NoMemory();
    }
    // decode
    const uint8_t *texture_data = reinterpret_cast<const uint8_t *>(view.buf);
    uint32_t *image_data = reinterpret_cast<uint32_t *>(PyBytes_AsString(res));
    auto decode_result = decode_astc(texture_data, width, height, block_width, block_height, image_data);
    PyBuffer_Release(&view);
    // check decode result
    if (!decode_result)
    {
        Py_DECREF(res);
        PyErr_SetString(PyExc_RuntimeError, "Decoding failed");
        return NULL;
    }
    // return
    return res;
}

typedef bool (*CrunchUnpackFunc_t)(const uint8_t *, uint32_t, uint32_t, void **, uint32_t *);
template <CrunchUnpackFunc_t CrunchUnpackFunc>
static PyObject *unpack_crunch(PyObject *self, PyObject *args)
{
    // define vars
    Py_buffer view;
    uint32_t level_index = 0;

    if (!PyArg_ParseTuple(args, "y*|I", &view, &level_index))
        return NULL;

    // unpack
    const uint8_t *crunch_data = reinterpret_cast<const uint8_t *>(view.buf);
    uint8_t *image_data = nullptr;
    uint32_t retSize;
    auto unpack_result = CrunchUnpackFunc(crunch_data, static_cast<uint32_t>(view.len), level_index, reinterpret_cast<void **>(&image_data), &retSize);
    PyBuffer_Release(&view);
    if (!unpack_result)
    {
        if (image_data)
        {
            delete[] image_data;
        }
        PyErr_SetString(PyExc_RuntimeError, "Unpacking failed");
        return NULL;
    }

    PyObject *res = Py_BuildValue("y#", image_data, retSize);
    delete[] image_data;
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
     (PyCFunction)decode_l<decode_bc1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc3",
     (PyCFunction)decode_l<decode_bc3>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_pvrtc",
     (PyCFunction)_decode_pvrtc,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image, bool is2bpp"},
    {"decode_etc1",
     (PyCFunction)decode_l<decode_etc1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2",
     (PyCFunction)decode_l<decode_etc2>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2a1",
     (PyCFunction)decode_l<decode_etc2a1>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_etc2a8",
     (PyCFunction)decode_l<decode_etc2a8>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacr",
     (PyCFunction)decode_l<decode_eacr>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacr_signed",
     (PyCFunction)decode_l<decode_eacr_signed>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacrg",
     (PyCFunction)decode_l<decode_eacrg>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_eacrg_signed",
     (PyCFunction)decode_l<decode_eacrg_signed>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc4",
     (PyCFunction)decode_u32<decode_bc4>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc5",
     (PyCFunction)decode_u32<decode_bc5>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc6",
     (PyCFunction)decode_u32<decode_bc6>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_bc7",
     (PyCFunction)decode_u32<decode_bc7>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_atc_rgb4",
     (PyCFunction)decode_u32<decode_atc_rgb4>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_atc_rgba8",
     (PyCFunction)decode_u32<decode_atc_rgba8>,
     METH_VARARGS,
     "bytes data, long w, long h, bytes image"},
    {"decode_astc",
     (PyCFunction)_decode_astc,
     METH_VARARGS,
     "bytes data, long w, long h, int bw, int bh, bytes image"},
    {"unpack_crunch",
     (PyCFunction)unpack_crunch<crunch_unpack_level>,
     METH_VARARGS,
     "bytes data"},
    {"unpack_unity_crunch",
     (PyCFunction)unpack_crunch<unity_crunch_unpack_level>,
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