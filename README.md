# StrGraph

`StrGraph` is a library that aims for carrying out string manipulations modeled by directed acyclic graphs (DAGs);

`StrGraph` has both Python and C++ bindings. One can use either language to build out the DAGs and carry out the computation;

Check `doc/reference.md` for detailed explanation;

## Features
1. Lazy evaluation
    - The graph is firstly constructed. Only after `StrGraph::DAG::doCompute()/pyStrGraph.DAG.doCompute()` being called will the evaluation be carried out.
1. Versatility
    - A handful of predefined operators are available and can be used in after imported
    - One can always define its own operator in Python or C++, as long as it conforms to the interface
1. Idempotency
    - After constructing a `DAG` and call `doCompute()`. It is guaranteened that calling it again will yield the identical result.
    - If the graph is integral, the second call will directly read out the result rather than carry out the computation all over again.
1. Multithread supported
    - Just call `StrGraph::DAG::doCompute()/pyStrGraph.DAG.doCompute()` with the argument `use_multithread` set to true.
    - Please refer to the last example in `tests/showcase.ipynb`.

## Download
```bash
git clone --recursive https://github.com/xzceddie/StrGraph.git
git checkout v1.0
```
The `--recursive` flag is needed for cloning the `Catch2` dependency.

## Build
starting at root level of the repository:
```bash
mkdir build
cd build
cmake ../
make -j8 
```
Make sure to use `C++17` compatible compilers to make it work.
Also make sure `pybind11` is installed

## Run Test
starting at root level of the repository:
```bash
cd build/tests/
ctest
```
Also checkout `tests/showcase.ipynb` after compilation is done.

## Dependencies
- `pybind11`
    - to install:
    ```
    pip3 install pybind11
    ```
    OR
    ```
    conda install -c conda-forge pybind11
    ```

    - **NOTE**:
        In case `cmake` still does not find `pybind11`, try to do:
        ```
        pip3 install pybind11[global]
        ```
        or 
        ```
        pip3 install pybind11\[global\]
        ```
        whichever works
- `cmake`
    - to install (MacOS):
    ```
    brew install cmake
    ```
    or (Ubuntu/Debian)
    ```
    sudo apt install cmake
    ```

- `Catch2`
    - Just included for testing. It is included as a git submodule; Supplying `--recursive` in `git clone` command suffice.

## TODO
1. Add support for defining `InputNode/OperatorNode` first and then connect them together;
2. Add serialize/deserialize functionalities;
3. Add out-of-memory computation support for big Graphs;