#include <catch2/catch_all.hpp>
#include <src/include/node.h>

using namespace StrGraph;

const std::string input1 = "Hello ";
const std::string input2 = "World ";

const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );
TEST_CASE( "test_node", "[test_node concat]" ) {
    const auto ConcatOp = [] (const std::vector<std::string>& inputs) {
        return std::accumulate(inputs.begin(), inputs.end(), std::string());
    };

    const OperatorNode
        op_node{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 }, ConcatOp } ;
    op_node.compute();
    REQUIRE( op_node.getValue() == "Hello World " );

}

TEST_CASE( "test_node", "[test_node repeat5]" ) {
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

    const OperatorNode
        op_node{ std::vector<std::shared_ptr<Node>>{ inode1 }, RepeatOp5 } ;
    op_node.compute();
    REQUIRE( op_node.getValue() == "Hello Hello Hello Hello Hello " );
}
