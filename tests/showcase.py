import os, sys
sys.path.append("../build/src/") # add the route of the exported dynamic library
import pyStrGraph

# example 1
# define the topology of the DAG
dag = pyStrGraph.DAG( ["Hello", "World", "Str", "Graph"] )
concated_id = dag.addOperatorNode([0, 1], pyStrGraph.ConcatOperator(" "))
repeated_id = dag.addOperatorNode([2], pyStrGraph.RepeatOperator(2, " | "))
concated_id_1 = dag.addOperatorNode([concated_id, repeated_id, 3], pyStrGraph.ConcatOperator(" "))
final_id = dag.addOperatorNode([concated_id_1], pyStrGraph.InsertOperator(3, " <=> "))

res = dag.doCompute(use_multithread = True, verbose_level = 1 )
print(res[0])
