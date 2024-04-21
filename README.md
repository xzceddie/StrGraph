# StrGraph  (TODO: Lot of stuff needs to be fixed)

## Build

## Dependencies
- `pybind11`
    - to install:
    ```
    pip3 install pybind11
    ```
    or
    ```
    conda install -c conda-forge pybind11
    ```
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

## Features
1. Idempotent
    - After constructing a `DAG` and call `doCompute()`. It is guaranteened that calling it again will yield the identical result.
2. Multithread supported
    - Just call `StrGraph::DAG::doCompute(true)` in C++.
