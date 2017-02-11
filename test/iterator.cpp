
#include "tree/tree.hpp"
#include <iostream>

constexpr size_t r = 2;
Tree<r, int> gen()
{
    using Ti   = Tree<r, int>;
    using Node = Ti::node_type;
    auto tree  = Ti{Node{1, {Ti::singleton(2), Ti::singleton(3)}}};

    return tree;
}

int main()
{
    {
        auto tree1 = gen();

        using Ti   = Tree<r, int>;
        using Node = Ti::node_type;
        auto tree  = Ti{Node{5, {std::move(tree1), gen()}}};

        auto bfit = tree.bf_begin();
        auto end  = tree.end();

        for (; bfit != end; ++bfit) {
            std::cout << *bfit << ' ';
        }
        std::cout << std::endl;
    }
}
