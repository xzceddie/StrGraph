#include <catch2/catch_all.hpp>
#include <src/include/node.h>
#include <src/include/DAG.h>
#include <iostream>

using namespace StrGraph;

const std::string input1 = "Hello ";
const std::string input2 = "World ";

const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );
TEST_CASE( "test_DAG", "[simple]" ) {
    const auto ConcatOp = [] (const std::vector<std::string>& inputs) {
        return std::accumulate(inputs.begin(), inputs.end(), std::string());
    };
    const auto RepeatOpImpl =
        [] ( const std::string& input, int times, const std::string& delim  = "" ) {
            std::string res;
            if( times == 0 ) {
                return std::string();
            }
            if ( times == 1 ) {
                return input;
            }

            for( int i = 0; i < times - 1; i++ ) {
                std::copy( input.begin(), input.end(), std::back_inserter( res ));
                std::copy( delim.begin(), delim.end(), std::back_inserter( res ));
            }
            std::copy( input.begin(), input.end(), std::back_inserter( res ));
            return res;
    };

    const auto RepeatOp5 = [RepeatOpImpl] ( const std::vector<std::string>& input ) {
        return RepeatOpImpl( input[0], 5 );
    };

    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 } };
    const auto new_node_id = dag.addOperatorNode( dag.getInputs(), ConcatOp );
    const auto new_node_id2 = dag.addOperatorNode( std::vector<int>{ new_node_id }, RepeatOp5 );

    const auto res = dag.doCompute();
    // std::cout << "DAG result: \n";
    // for(auto& ele: res) {
    //     std::cout << ele << std::endl;
    // }
    REQUIRE( res.front() == "Hello World Hello World Hello World Hello World Hello World " );
}

