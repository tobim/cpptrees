
#include "tree/tree.hpp"
#include "catch.hpp"

using Ti = Tree<2, int>;

SCENARIO( "trees can be constructed in various ways", "[tree]" ) {
    GIVEN( "An empty tree" ) {
        auto empty  = Ti::empty_tree();
        REQUIRE( empty.empty() );
    }

    GIVEN( "An singleton tree" ) {
        auto single = Ti::singleton(1);
        auto& node  = mpark::get<1>(single);
        REQUIRE( node.value_ == 1 );
    }
}
