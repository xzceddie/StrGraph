#include <catch2/catch_all.hpp>
#include <src/include/node.h>
#include <src/include/DAG.h>

using namespace StrGraph;

const std::string input1 = "Hello ";
const std::string input2 = "World ";

const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );
TEST_CASE( "test_DAG", "[simple]" ) {
    const auto ConcatOp = [] (const std::vector<std::string>& inputs) {
        return std::accumulate(inputs.begin(), inputs.end(), std::string());
    };
    // const OperatorNode<decltype( ConcatOp )>
    //     op_node{ std::vector<std::shared_ptr<InputNode>>{ inode1, inode2 }, ConcatOp } ;

    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 } };
    dag.addOperatorNode( dag.getInputs(), ConcatOp );

    // REQUIRE( op_node.compute() == "Hello World " );
}

