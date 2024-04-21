#include <catch2/catch_all.hpp>
#include <src/include/node.h>
#include <src/include/DAG.h>
#include <src/include/operators.h>
#include <iostream>

using namespace StrGraph;

TEST_CASE( "test_DAG", "[test_concat_repeat_operators]" ) {
    const std::string input1 = "Hello";
    const std::string input2 = "World";

    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );

    const auto concat_op = ConcatOperator( " " );
    const auto repeat_op = RepeatOperator( 5, " | " );

    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 } };

    const auto new_node_id = dag.addOperatorNode( dag.getInputs(), concat_op );
    const auto new_node_id2 = dag.addOperatorNode( std::vector<int>{ new_node_id }, repeat_op );

    auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence
    REQUIRE( res.front() == "Hello World | Hello World | Hello World | Hello World | Hello World" );
    REQUIRE( res2.front() == "Hello World | Hello World | Hello World | Hello World | Hello World" );
}

TEST_CASE( "test_DAG", "[test_concat_repeat_operators2]" ) {
    const std::string input1 = "Hello";
    const std::string input2 = "World";
    const std::string input3 = "StrGraph";
    const std::string input4 = "PythonCpp";

    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );
    const std::shared_ptr<Node> inode3 = std::make_shared<InputNode>( input3 );
    const std::shared_ptr<Node> inode4 = std::make_shared<InputNode>( input4 );

    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2, inode3, inode4 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0, 1 }, concat_op );
    const auto new_node_id2 = dag.addOperatorNode( std::vector<int>{ 2, 3 }, concat_op );
    const auto new_node_id3 = dag.addOperatorNode( std::vector<int>{ new_node_id, new_node_id2 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "Hello World StrGraph PythonCpp" );
    REQUIRE( res2.front() == "Hello World StrGraph PythonCpp" );
}

TEST_CASE( "test_DAG", "[test_toupper_operator]" ) {
    const std::string input1 = "hello";
    const std::string input2 = "world";

    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );

    const auto toupper_op = ToUpperOperator();
    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0 }, toupper_op );
    // const auto new_node_id2 = dag.addOperatorNode( std::vector<int>{ 1 }, toupper_op );
    const auto new_node_id3 = dag.addOperatorNode( std::vector<int>{ new_node_id, 1 }, concat_op );


    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "HELLO world" );
    REQUIRE( res2.front() == "HELLO world" );
}

TEST_CASE( "test_DAG", "[test_tolower_operator]" ) {
    const std::string input1 = "HELLO";
    const std::string input2 = "WORLD";

    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );

    const auto tolower = ToLowerOperator();
    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1, inode2 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0 }, tolower );
    const auto new_node_id3 = dag.addOperatorNode( std::vector<int>{ new_node_id, 1 }, concat_op );


    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "hello WORLD" );
    REQUIRE( res2.front() == "hello WORLD" );
}

TEST_CASE( "test_dag", "[test_insert_operator]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto concat_op = InsertOperator( 5, " " );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "hello world hello world hello world" );
    REQUIRE( res2.front() == "hello world hello world hello world" );
}

TEST_CASE( "test_dag", "[test_rotate_operator]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto concat_op = RotateOperator( -5 );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "worldhelloworldhelloworldhello" );
    REQUIRE( res2.front() == "worldhelloworldhelloworldhello" );
}

TEST_CASE( "test_dag", "[test_rotate_operator_2]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto concat_op = RotateOperator( 5 );
    DAG dag{ std::vector<std::shared_ptr<Node>>{ inode1 } };

    const auto new_node_id = dag.addOperatorNode( std::vector<int>{ 0 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "worldhelloworldhelloworldhello" );
    REQUIRE( res2.front() == "worldhelloworldhelloworldhello" );
}