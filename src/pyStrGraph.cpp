#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <functional>
#include <iostream>
#include <string>
#include <src/include/operators.h>
#include <src/include/DAG.h>

namespace py = pybind11;

using namespace StrGraph;

PYBIND11_MODULE(pyStrGraph, m) {
    m.doc() = "pyStrGraph is a python wrapper for StrGraph, used for DAG-type manipulations on input strings";

    py::class_<StrGraph::DAG, std::shared_ptr<StrGraph::DAG>>( m, "DAG" )
        .def( py::init<const std::vector<std::string>&>() )
        .def( "touch", &StrGraph::DAG::touch )
        .def( "markDirty", &StrGraph::DAG::markDirty )
        .def( "addOperatorNode", &StrGraph::DAG::addOperatorNodeById )
        .def( "doCompute", &StrGraph::DAG::doCompute, py::call_guard<py::gil_scoped_release>() )
    ;
}