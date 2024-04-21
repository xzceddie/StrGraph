#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <functional>
#include <iostream>
#include <string>
#include <src/include/operators.h>

namespace py = pybind11;

PYBIND11_MODULE(pyStrGraph, m) {
    m.doc() = "pyStrGraph is a python wrapper for StrGraph, used for DAG-type manipulations on input strings";
}