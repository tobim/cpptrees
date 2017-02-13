
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

SCENARIO( "we can traverse a tree in different ways", "[tree]" ) {
    GIVEN( "an binary test tree" ) {
        auto test = gen2();
        WHEN( "We walk it depth-first" ) {
            std::vector<int> out;
            std::vector<int> ref = {5, 1, 2, 3, 1, 2, 3};
            tree::depth_first(test, [&out](int p) { out.push_back(p); });
            THEN( "The order is correct" ) {
                REQUIRE( out == ref );
            }
        }
        WHEN( "We walk it breadth-first" ) {
            std::vector<int> out;
            std::vector<int> ref = {5, 1, 1, 2, 3, 2, 3};
            tree::breadth_first(test, [&out](int p) { out.push_back(p); });
            THEN( "The order is correct" ) {
                REQUIRE( out == ref );
            }
        }
    }

    GIVEN( "an ternary test tree" ) {
        auto test = gen3();
        WHEN( "We walk it depth-first" ) {
            std::vector<int> out;
            std::vector<int> ref = {5, 1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};
            tree::depth_first(test, [&out](int p) { out.push_back(p); });
            THEN( "The order is correct" ) {
                REQUIRE( out == ref );
            }
        }
        WHEN( "We walk it breadth-first" ) {
            std::vector<int> out;
            std::vector<int> ref = {5, 1, 1, 1, 2, 3, 4, 2, 3, 4, 2, 3, 4};
            tree::breadth_first(test, [&out](int p) { out.push_back(p); });
            THEN( "The order is correct" ) {
                REQUIRE( out == ref );
            }
        }
    }
}
