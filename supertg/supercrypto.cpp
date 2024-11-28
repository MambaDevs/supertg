#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string>

#include "aes256.h"
#include "ige256.h"
#include "ctr256.h"
#include "cbc256.h"

#define DESCRIPTION "Fast and Portable Cryptography Extension Library for Pyrogram\n" \
    "TgCrypto is part of Pyrogram, a Telegram MTProto library for Python\n" \
    "You can learn more about Pyrogram here: https://pyrogram.org\n"

// Utility function to handle PyBuffer release safely using RAII
struct PyBufferGuard {
    Py_buffer buffer;

    PyBufferGuard(PyObject *obj) {
        if (!PyObject_GetBuffer(obj, &buffer, PyBUF_SIMPLE)) {
            throw std::runtime_error("Failed to get buffer.");
        }
    }

    ~PyBufferGuard() {
        PyBuffer_Release(&buffer);
    }

    void* data() { return buffer.buf; }
    size_t size() { return buffer.len; }
};

static PyObject* ige(PyObject* args, uint8_t encrypt) {
    try {
        PyObject *dataObj, *keyObj, *ivObj;

        if (!PyArg_ParseTuple(args, "OOO", &dataObj, &keyObj, &ivObj)) {
            return nullptr;
        }

        PyBufferGuard dataBuffer(dataObj);
        PyBufferGuard keyBuffer(keyObj);
        PyBufferGuard ivBuffer(ivObj);

        if (dataBuffer.size() == 0) {
            PyErr_SetString(PyExc_ValueError, "Data must not be empty");
            return nullptr;
        }

        if (dataBuffer.size() % 16 != 0) {
            PyErr_SetString(PyExc_ValueError, "Data size must match a multiple of 16 bytes");
            return nullptr;
        }

        if (keyBuffer.size() != 32) {
            PyErr_SetString(PyExc_ValueError, "Key size must be exactly 32 bytes");
            return nullptr;
        }

        if (ivBuffer.size() != 32) {
            PyErr_SetString(PyExc_ValueError, "IV size must be exactly 32 bytes");
            return nullptr;
        }

        std::unique_ptr<uint8_t[]> buf;
        {
            Py_BEGIN_ALLOW_THREADS
            buf.reset(ige256(dataBuffer.data(), dataBuffer.size(),
                             keyBuffer.data(), ivBuffer.data(), encrypt));
            Py_END_ALLOW_THREADS
        }

        if (!buf) {
            PyErr_SetString(PyExc_RuntimeError, "Encryption/Decryption failed.");
            return nullptr;
        }

        return Py_BuildValue("y#", buf.get(), dataBuffer.size());
    } catch (const std::exception& ex) {
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        return nullptr;
    }
}

static PyObject* ige256_encrypt(PyObject* self, PyObject* args) {
    return ige(args, 1);
}

static PyObject* ige256_decrypt(PyObject* self, PyObject* args) {
    return ige(args, 0);
}

static PyObject* ctr256_encrypt(PyObject* self, PyObject* args) {
    try {
        PyObject *dataObj, *keyObj, *ivObj, *stateObj;

        if (!PyArg_ParseTuple(args, "OOOO", &dataObj, &keyObj, &ivObj, &stateObj)) {
            return nullptr;
        }

        PyBufferGuard dataBuffer(dataObj);
        PyBufferGuard keyBuffer(keyObj);
        PyBufferGuard ivBuffer(ivObj);
        PyBufferGuard stateBuffer(stateObj);

        if (dataBuffer.size() == 0) {
            PyErr_SetString(PyExc_ValueError, "Data must not be empty");
            return nullptr;
        }

        if (keyBuffer.size() != 32) {
            PyErr_SetString(PyExc_ValueError, "Key size must be exactly 32 bytes");
            return nullptr;
        }

        if (ivBuffer.size() != 16) {
            PyErr_SetString(PyExc_ValueError, "IV size must be exactly 16 bytes");
            return nullptr;
        }

        if (stateBuffer.size() != 1 || *(uint8_t*)stateBuffer.data() > 15) {
            PyErr_SetString(PyExc_ValueError, "State value must be in the range [0, 15]");
            return nullptr;
        }

        std::unique_ptr<uint8_t[]> buf;
        {
            Py_BEGIN_ALLOW_THREADS
            buf.reset(ctr256(dataBuffer.data(), dataBuffer.size(),
                             keyBuffer.data(), ivBuffer.data(), stateBuffer.data()));
            Py_END_ALLOW_THREADS
        }

        if (!buf) {
            PyErr_SetString(PyExc_RuntimeError, "Encryption failed.");
            return nullptr;
        }

        return Py_BuildValue("y#", buf.get(), dataBuffer.size());
    } catch (const std::exception& ex) {
        PyErr_SetString(PyExc_RuntimeError, ex.what());
        return nullptr;
    }
}

static PyMethodDef methods[] = {
    {"ige256_encrypt", (PyCFunction)ige256_encrypt, METH_VARARGS, "AES-256-IGE Encryption"},
    {"ige256_decrypt", (PyCFunction)ige256_decrypt, METH_VARARGS, "AES-256-IGE Decryption"},
    {"ctr256_encrypt", (PyCFunction)ctr256_encrypt, METH_VARARGS, "AES-256-CTR Encryption"},
    {nullptr, nullptr, 0, nullptr}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT,
    "TgCrypto",
    DESCRIPTION,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_tgcrypto() {
    return PyModule_Create(&module);
}

