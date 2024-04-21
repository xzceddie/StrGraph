#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/complex.h>
#include <pybind11/functional.h>
#include <functional>
#include <iostream>
#include <string>

namespace py = pybind11;


int add(int i, int j) {
    return i + j;
}

int subtract(int i, int j) {
    return i - j;
}

using CallbackFunction = std::function<int(int)>;

class MyInterface {
    CallbackFunction mCallBack;
public:
    void setCallback(const CallbackFunction& callback);
    int performOperation(int value);
};

void MyInterface::setCallback(const CallbackFunction& callback) {
    mCallBack = callback;
}

int MyInterface::performOperation(int value) {
    std::cout << "(Cpp) Hello, I accept input: " << value << std::endl;
    return mCallBack(value);
}


using CallBackStrVec = std::function<std::string(const std::vector<std::string>&)>;
std::string processStrings(const CallBackStrVec& callback) {
    const std::vector<std::string> strings = { "Hello", "World", "from", "C++" };
    return callback(strings);
}

PYBIND11_MODULE(example, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring
    m.def("add", &add, "A function that adds two numbers");
    m.def("subtract", &subtract, "A function that adds two numbers");

    py::class_<MyInterface>(m, "MyInterface")
        .def(py::init<>())
        .def("setCallback", &MyInterface::setCallback)
        .def("performOperation", &MyInterface::performOperation);


    m.def("process_strings", &processStrings, "Function that accepts a call back function to process strings");
}

