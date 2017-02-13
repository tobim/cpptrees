
#include "tree/tree.hpp"
#include "catch.hpp"

static
Tree<2, int> gen2()
{
    using Ti   = Tree<2, int>;
    using Node = Ti::node_type;
    auto tree =
        Ti{Node{5,
                {{Ti{Node{1, {{Ti::singleton(2), Ti::singleton(3)}}}},
                  Ti{Node{1, {{Ti::singleton(2), Ti::singleton(3)}}}}
                 }}
        }};
    return tree;
}

static
Tree<3, int> gen3()
{
    using Ti   = Tree<3, int>;
    using Node = Ti::node_type;
    auto tree  = Ti{Node{
        5,
        {{Ti{Node{1, {{Ti::singleton(2), Ti::singleton(3), Ti::singleton(4)}}}},
          Ti{Node{1, {{Ti::singleton(2), Ti::singleton(3), Ti::singleton(4)}}}},
          Ti{Node{ 1, {{Ti::singleton(2), Ti::singleton(3), Ti::singleton(4)}}}
        }}}
    }};
    return tree;
}

SCENARIO( "we can iterate over a tree in different ways", "[tree]" ) {
    GIVEN( "a binary test tree" ) {
        auto test = gen2();
        WHEN( "we walk it depth-first" ) {
            auto dfit = test.df_begin();
            THEN( "The order is correct" ) {
                REQUIRE( *dfit++ == 5 );
                REQUIRE( *dfit++ == 1 );
                REQUIRE( *dfit++ == 2 );
                REQUIRE( *dfit++ == 3 );
                REQUIRE( *dfit++ == 1 );
                REQUIRE( *dfit++ == 2 );
                REQUIRE( *dfit++ == 3 );
                REQUIRE( dfit == test.end() );
            }
        }
        WHEN( "we walk it breadth-first" ) {
            auto bfit = test.bf_begin();
            THEN( "The order is correct" ) {
                REQUIRE( *bfit++ == 5 );
                REQUIRE( *bfit++ == 1 );
                REQUIRE( *bfit++ == 1 );
                REQUIRE( *bfit++ == 2 );
                REQUIRE( *bfit++ == 3 );
                REQUIRE( *bfit++ == 2 );
                REQUIRE( *bfit++ == 3 );
                REQUIRE( bfit == test.end() );
            }
        }
    }

    GIVEN( "a ternary test tree" ) {
        auto test = gen3();
        WHEN( "we walk it depth-first" ) {
            auto dfit = test.df_begin();
            THEN( "The order is correct" ) {
                REQUIRE( *dfit++ == 5 );
                REQUIRE( *dfit++ == 1 );
                REQUIRE( *dfit++ == 2 );
                REQUIRE( *dfit++ == 3 );
                REQUIRE( *dfit++ == 4 );
                REQUIRE( *dfit++ == 1 );
                REQUIRE( *dfit++ == 2 );
                REQUIRE( *dfit++ == 3 );
                REQUIRE( *dfit++ == 4 );
                REQUIRE( *dfit++ == 1 );
                REQUIRE( *dfit++ == 2 );
                REQUIRE( *dfit++ == 3 );
                REQUIRE( *dfit++ == 4 );
                REQUIRE( dfit == test.end() );
            }
        }
        WHEN( "we walk it breadth-first" ) {
            auto bfit = test.bf_begin();
            THEN( "The order is correct" ) {
                REQUIRE( *bfit++ == 5 );
                REQUIRE( *bfit++ == 1 );
                REQUIRE( *bfit++ == 1 );
                REQUIRE( *bfit++ == 1 );
                REQUIRE( *bfit++ == 2 );
                REQUIRE( *bfit++ == 3 );
                REQUIRE( *bfit++ == 4 );
                REQUIRE( *bfit++ == 2 );
                REQUIRE( *bfit++ == 3 );
                REQUIRE( *bfit++ == 4 );
                REQUIRE( *bfit++ == 2 );
                REQUIRE( *bfit++ == 3 );
                REQUIRE( *bfit++ == 4 );
                REQUIRE( bfit == test.end() );
            }
        }
    }
}
