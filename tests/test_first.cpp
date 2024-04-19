#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>


TEST_CASE( "SimpleTest", "[simple]" ) {
    REQUIRE( 1 + 1 == 2 );
}
