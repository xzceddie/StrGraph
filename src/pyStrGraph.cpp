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
        .def( "isIntegral", &StrGraph::DAG::isIntegral )
        .def( "addOperatorNode", &StrGraph::DAG::addOperatorNodeById, py::arg("input_node_ids"), py::arg("operator") )
        .def( "getLastNode", &StrGraph::DAG::getLastNodeId )
        .def( "__lshift__", &StrGraph::DAG::operator<<, py::arg("recipe")  )
        .def( "queryNodeValue", &StrGraph::DAG::queryNodeValue )
        .def( "doCompute", &StrGraph::DAG::doCompute, py::arg("use_multithread"), py::arg("verbose_level"), py::call_guard<py::gil_scoped_release>() )
        .def_property_readonly_static("__doc__",
            [](py::object) {
                return
                    "This is the entry point for constructing the DAG, input list of str as input nodes"
                    "\tmethod addOperatorNode: expect input_node_ids (List[int]) and operator. Return the id if the new node. The new id will be automaticalluy assigned as the 1 plus the current largest id"
                    "\tmethod touch: make the DAG dirty such that subsequent doCompute() call will actually compute the DAG all over again"
                    "\tmethod markDirty: alias for touch"
                    "\tmethod isIntegral: return True if the DAG is integral (not dirty)"
                    "\tmethod __lshift__: expect tuple of input_node_ids (List[int]) and operator, return the updated DAG"
                    "\tmethod getLastNode: return the last node id has been added to the DAG"
                    "\tmethod queryNodeValue: input the node id, return the value of the node. Cautious to use this method if DAG is not integral (dirty)"
                    ;
            })
    ;

    py::class_<StrGraph::Operator>( m, "Operator" )
        .def( py::init<const OpFunc&>() )
        .def( "__call__", &StrGraph::Operator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "This is the operator you can pass in a Python call back function for the computation logic"
                       "input : func (List[str]) -> str"
                       "return: str"
                ;
            })
    ;

    py::class_<StrGraph::ConcatOperator>( m, "ConcatOperator" )
        .def( py::init<const std::string&>() )
        .def( "__call__", &StrGraph::ConcatOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Concatenate input strings, pass in a delimiter string to represent the delimiter"
                ;
            })
    ;

    py::class_<StrGraph::RepeatOperator>( m, "RepeatOperator" )
        .def( py::init<const int, const std::string&>() )
        .def( "__call__", &StrGraph::RepeatOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Repeat input strings for given times, pass in (times, delimiter)"
                ;
            })
    ;

    py::class_<StrGraph::ToUpperOperator>( m, "ToUpperOperator" )
        .def( py::init<>() )
        .def( "__call__", &StrGraph::ToUpperOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Make the input string all upper case"
                ;
            })
    ;

    py::class_<StrGraph::ToLowerOperator>( m, "ToLowerOperator" )
        .def( py::init<>() )
        .def( "__call__", &StrGraph::ToLowerOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Make the input string all lower case"
                ;
            })
    ;

    py::class_<StrGraph::CapitalizeOperator>( m, "CapitalizeOperator" )
        .def( py::init<>() )
        .def( "__call__", &StrGraph::CapitalizeOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object) {
                return "Make every character after a whitespace uppercase";
                ;
            })
    ;

    py::class_<StrGraph::InsertOperator>( m, "InsertOperator" )
        .def( py::init<const int, const std::string& >() )
        .def( "__call__", &StrGraph::InsertOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Insert delimiters into the original string at evenly spaced indices, pass in (interval, delimiter)"
                ;
            })
    ;
    
    py::class_<StrGraph::RotateOperator>( m, "RotateOperator" )
        .def( py::init<const int>() )
        .def( "__call__", &StrGraph::RotateOperator::operator() )
        .def_property_readonly_static("__doc__",
            [](py::object /* self */) {
                return "Rotate the string to the right (positive) or left (negative) by the given amount"
                ;
            })
    ;
}