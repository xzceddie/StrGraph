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
        .def( "addOperatorNode", &StrGraph::DAG::addOperatorNodeById, py::arg("input_node_ids"), py::arg("operator") )
        .def( "doCompute", &StrGraph::DAG::doCompute, py::arg("use_multithread"), py::arg("verbose_level"), py::call_guard<py::gil_scoped_release>() )
        .def_property_readonly_static("__doc__",
            [](py::object) {
                return
                    "This is the entry point for constructing the DAG, input list of str as input nodes";
            })
    ;

    py::class_<StrGraph::ConcatOperator>( m, "ConcatOperator" )
        .def( py::init<const std::string&>() )
        .def( "__call__", &StrGraph::ConcatOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Concatenate input strings, pass in a delimiter string to represent the delimiter";
            })
    ;

    py::class_<StrGraph::RepeatOperator>( m, "RepeatOperator" )
        .def( py::init<const int, const std::string&>() )
        .def( "__call__", &StrGraph::RepeatOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Repeat input strings for given times, pass in (times, delimiter)";
            })
    ;

    py::class_<StrGraph::ToUpperOperator>( m, "ToUpperOperator" )
        .def( py::init<>() )
        .def( "__call__", &StrGraph::ToUpperOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Make the input string all upper case";
            })
    ;

    py::class_<StrGraph::ToLowerOperator>( m, "ToLowerOperator" )
        .def( py::init<>() )
        .def( "__call__", &StrGraph::ToLowerOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Make the input string all lower case";
            })
    ;

    py::class_<StrGraph::InsertOperator>( m, "InsertOperator" )
        .def( py::init<const int, const std::string& >() )
        .def( "__call__", &StrGraph::InsertOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Insert delimiters into the original string at evenly spaced indices, pass in (interval, delimiter)";
            })
    ;
    
    py::class_<StrGraph::RotateOperator>( m, "RotateOperator" )
        .def( py::init<const int>() )
        .def( "__call__", &StrGraph::RotateOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Rotate the string to the right (positive) or left (negative) by the given amount";
            })
    ;
}