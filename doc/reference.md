# Reference

## How this works

The library is commit to solve string manipulation problems. All the content involved are strings.

When dealing with a string manipulation problem, one will have a handful of strings as inputs. And have numerous operations that operate on some of the inputs each. One can continue to define addtional operations on either the input strings or intermediate results. Finally one expects some output from the system.

### Concepts
1. `pyStrGraph.DAG`
    - The central data structure. It is meant to model the topology/dependencies of the computations;
    - Houses `InputNode` and `OperatorNode`
    - On creating, one only needs to supply the contents of the input nodes

2. `InputNode`
    - Houses only the input content.
    - Can be the parent of `OperatorNode`.
    - Can sometimes be the node that holds the output result (when it is not the parent of any `OperatorNode`).

3. `OperatorNode`
    - Initially houses only the information of which nodes are its parent nodes and the operaion/operator.
    - Houses also the result of the evalutaion after it is carried out.
    - Can be the parent of `OperatorNode`.
    - Can sometimes be the node that holds the output result (when it is not the parent of any `OperatorNode`).

4. `Operator`
    - Just a function that accepts a list of strings and output a string.
    - It is a part of `OperatorNode`.

5. node id
    - An integer that identifies the node.


### Workflow
This library models the workflow as follows:
1. Create a `pyStrGraph.DAG` object;
2. Recursively add `OperatorNode` to the `pyStrGraph.DAG` object. One should specify the parent nodes (can be either `InputNode` or `OperatorNode`) of the newly-added `OperatorNode`, together with the `Operator` the node supports;
3. After building out the `pyStrGraph.DAG` object, call the `doCompute()` method to carry out the computation;
4. The ouput of the system is the considered as the contents of the nodes (`InputNode` or `OperatorNode`) that do not have child nodes;

## Code Workflow (Python)

#### please refer to `tests/showcase.ipynb` for reference

1. Build the `pyStrGraph` shared library, it should be in `build/src/`, extension is dependent on the platform
1. Import the library. Care to specify library path for Python interpreter to locate the library
1. Create a `pyStrGraph.DAG` object with the desired input string values; Input nodes containing the values are automatically created
    - sensitive to order
    - the input nodes are automatically assigned the node ids starting from 0 through #`InputNode` - 1
3. Add operator nodes through the interface `pyStrGraph.DAG.addOperatorNode/pyStrGraph.DAG.__lshift__`.
    - accepts two arguments, first being the list of the parent nodes, second being the operator (a function accepts list of strings and return one string)
    - some operators only supports certain number of input strings, that number is checked in runtime. If not met, an `std::runtime_error` is thrown, but can cause `ipykernel` or `jupyter` to die
    - `addOperatorNode` returns the node id of the added Node for subsequent reference
    - `__lshift__` returns the reference of the `DAG` object such that the operations can be chained
4. Call `pyStrGraph.DAG.doCompute()` to do the computation;
    - accepts `use_multithread` as a bool and `verbose_level` as an integer
    - returns a list of str as the result


## FAQs
1. How do I refer to a node ?
    - Use node id. Node ids are automatically assigned to nodes with auto incremental property, starting from index 0. `DAG` object maintains the counting.
1. Python can do multithreading ?
    - Yes, `pybind11` support the release of `GIL`.
1. Does this support on-to-many operators, like split a string to many ?
    - Noe, currently not supported.