#include <catch2/catch_all.hpp>
#include <src/include/node.h>
#include <src/include/DAG.h>
#include <src/include/operators.h>
#include <iostream>

using namespace StrGraph;

TEST_CASE( "test_DAG", "[test_concat_repeat_operators]" ) {
    const std::string input1 = "Hello";
    const std::string input2 = "World";

    const auto concat_op = ConcatOperator( " " );
    const auto repeat_op = RepeatOperator( 5, " | " );

    DAG dag{ std::vector<std::string>{ input1, input2 } };

    const auto new_node_id = dag.addOperatorNode( dag.getInputs(), concat_op );
    const auto new_node_id2 = dag.addOperatorNodeById( std::vector<int>{ new_node_id }, repeat_op );

    auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence
    REQUIRE( res.front() == "Hello World | Hello World | Hello World | Hello World | Hello World" );
    REQUIRE( res2.front() == "Hello World | Hello World | Hello World | Hello World | Hello World" );
}

TEST_CASE( "test_DAG", "[test_concat_repeat_operators2]" ) {
    const std::string input1 = "Hello";
    const std::string input2 = "World";
    const std::string input3 = "StrGraph";
    const std::string input4 = "PythonCpp";

    // const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    // const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );
    // const std::shared_ptr<Node> inode3 = std::make_shared<InputNode>( input3 );
    // const std::shared_ptr<Node> inode4 = std::make_shared<InputNode>( input4 );

    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::string>{ input1, input2, input3, input4 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0, 1 }, concat_op );
    const auto new_node_id2 = dag.addOperatorNodeById( std::vector<int>{ 2, 3 }, concat_op );
    const auto new_node_id3 = dag.addOperatorNodeById( std::vector<int>{ new_node_id, new_node_id2 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "Hello World StrGraph PythonCpp" );
    REQUIRE( res2.front() == "Hello World StrGraph PythonCpp" );
}

TEST_CASE( "test_DAG", "[test_toupper_operator]" ) {
    const std::string input1 = "hello";
    const std::string input2 = "world";

    const auto toupper_op = ToUpperOperator();
    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::string>{ input1, input2 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0 }, toupper_op );
    const auto new_node_id3 = dag.addOperatorNodeById( std::vector<int>{ new_node_id, 1 }, concat_op );


    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "HELLO world" );
    REQUIRE( res2.front() == "HELLO world" );
}

TEST_CASE( "test_DAG", "[test_tolower_operator]" ) {
    const std::string input1 = "HELLO";
    const std::string input2 = "WORLD";

    // const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );
    // const std::shared_ptr<Node> inode2 = std::make_shared<InputNode>( input2 );

    const auto tolower = ToLowerOperator();
    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::string>{ input1, input2 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0 }, tolower );
    const auto new_node_id3 = dag.addOperatorNodeById( std::vector<int>{ new_node_id, 1 }, concat_op );


    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "hello WORLD" );
    REQUIRE( res2.front() == "hello WORLD" );
}

TEST_CASE( "test_DAG", "[test_capitalize_operator]" ) {
    const std::string input1 = "hello";
    const std::string input2 = "world";

    const auto cap_op = CapitalizeOperator();
    const auto concat_op = ConcatOperator( " " );
    DAG dag{ std::vector<std::string>{ input1, input2 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0, 1 }, concat_op );
    const auto new_node_id3 = dag.addOperatorNodeById( std::vector<int>{ new_node_id }, cap_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "Hello World" );
    REQUIRE( res2.front() == "Hello World" );
}

TEST_CASE( "test_dag", "[test_insert_operator]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    // const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto concat_op = InsertOperator( 5, " " );
    DAG dag{ std::vector<std::string>{ input1 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0 }, concat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "hello world hello world hello world" );
    REQUIRE( res2.front() == "hello world hello world hello world" );
}

TEST_CASE( "test_dag", "[test_rotate_operator]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    // const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto op = RotateOperator( -5 );
    DAG dag{ std::vector<std::string>{ input1 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0 }, op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res.front() == "worldhelloworldhelloworldhello" );
    REQUIRE( res2.front() == "worldhelloworldhelloworldhello" );
}

TEST_CASE( "test_dag", "[test_rotate_operator_2]" ) {
    const std::string input1 = "helloworldhelloworldhelloworld";
    // const std::shared_ptr<Node> inode1 = std::make_shared<InputNode>( input1 );

    const auto op = RotateOperator( 5 );
    DAG dag{ std::vector<std::string>{ input1 } };

    const auto new_node_id = dag.addOperatorNodeById( std::vector<int>{ 0 }, op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "worldhelloworldhelloworldhello" );
    REQUIRE( res2.front() == "worldhelloworldhelloworldhello" );
}

/*
    More sophisticated test cases
*/
TEST_CASE( "test_dag", "[test_complex1]" ) {
    const std::string input1 = "hello";
    const std::string input2 = "world";
    const std::string input3 = "Str";
    const std::string input4 = "Graph";

    const auto concat_op = ConcatOperator( " " );
    const auto repeat_op = RepeatOperator( 2, " | " );
    const auto insert_op = InsertOperator( 3, " <=> " );
    DAG dag{ std::vector<std::string>{ input1, input2, input3, input4 } };

    const auto concated_id = dag.addOperatorNodeById( std::vector<int>{ 0, 1 }, concat_op );
    const auto repeated_id = dag.addOperatorNodeById( std::vector<int>{ 2 }, repeat_op );
    const auto concated_id_1 = dag.addOperatorNodeById( std::vector<int>{ concated_id, repeated_id, 3 }, concat_op );
    const auto final_id = dag.addOperatorNodeById( std::vector<int>{ concated_id_1 }, insert_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true, 1 ); // indempotence && multithread
    REQUIRE( res.front() == "hel <=> lo  <=> wor <=> ld  <=> Str <=>  |  <=> Str <=>  Gr <=> aph" );
    REQUIRE( res2.front() == "hel <=> lo  <=> wor <=> ld  <=> Str <=>  |  <=> Str <=>  Gr <=> aph" );
}

TEST_CASE( "test_dag", "[test_complex2]" ) {
    const std::string input1 = "a";
    const std::string input2 = "b";
    const std::string input3 = "c";
    const std::string input4 = "d";

    const auto concat_op = ConcatOperator( " " );
    const auto repeat_op = RepeatOperator( 2, " | " );
    // const auto insert_op = InsertOperator( 3, " <=> " );
    DAG dag{ std::vector<std::string>{ input1, input2, input3, input4 } };

    const auto concated_id = dag.addOperatorNodeById( std::vector<int>{ 0, 2 }, concat_op );
    const auto concated_id_1 = dag.addOperatorNodeById( std::vector<int>{ 0, 1, 3 }, concat_op );
    const auto repeated_id = dag.addOperatorNodeById( std::vector<int>{ concated_id }, repeat_op );

    const auto res = dag.doCompute();
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread
    REQUIRE( res == std::vector<std::string> { "a b d", "a c | a c" } );
    REQUIRE( res2 == std::vector<std::string> { "a b d", "a c | a c" } );
}

TEST_CASE( "test_dag", "[test_streaming_operator]" ) {
    DAG dag{ std::vector<std::string>{"Hello", "World", "Str", "Graph"} };

    dag << std::pair<std::vector<int>, OpFunc>{std::vector<int>{0, 1}, ConcatOperator(" ")}
        << std::pair<std::vector<int>, OpFunc>{std::vector<int>{4, 2}, ConcatOperator(" ")}
        << std::pair<std::vector<int>, OpFunc>{std::vector<int>{5, 3}, ConcatOperator(" ")}
    ;

    auto res = dag.doCompute(false );
    dag.touch();
    auto res2 = dag.doCompute( true ); // indempotence && multithread

    REQUIRE( res == std::vector<std::string> { "Hello World Str Graph"} );
    REQUIRE( res2 == std::vector<std::string> { "Hello World Str Graph"} );
}