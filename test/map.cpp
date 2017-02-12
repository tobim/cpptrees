
#include "tree/tree.hpp"

#include <iostream>

constexpr size_t r = 2;
Tree<r, int> gen()
{
    using Ti   = Tree<r, int>;
    using Node = Ti::node_type;
    auto tree  = Ti{Node{1, {{Ti::singleton(2), Ti::singleton(3)}}}};

    return tree;
}

int main()
{
    {
        auto tree = gen();

        tree::depth_first(tree, [](int p) { std::cout << p << ' '; });

        std::cout << std::endl;
    }

    {
        auto tree1 = gen();

        using Ti   = Tree<r, int>;
        using Node = Ti::node_type;
        auto tree  = Ti{Node{5, {{std::move(tree1), gen()}}}};

        tree::depth_first(tree, [](int p) { std::cout << p << ' '; });
        std::cout << std::endl;

        tree::breadth_first(tree, [](int p) { std::cout << p << ' '; });
        std::cout << std::endl;
    }

    {
        constexpr size_t r = 3;
        using Ti           = Tree<r, int>;
        using Node         = Ti::node_type;
        auto tree          = Ti{
            Node{1, {{Ti::singleton(2), Ti::empty_tree(), Ti::singleton(3)}}}};

        tree::depth_first(tree, [](int p) { std::cout << p << ' '; });

        std::cout << std::endl;
    }
}
