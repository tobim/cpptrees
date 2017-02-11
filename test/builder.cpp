
#include "tree/tree.hpp"

int main () {
    using Ti = Tree<2, int>;
    auto single = Ti::singleton(1);
    auto& node = mpark::get<1>(single);
    auto empty = Ti::empty_tree();
    return (node.value_ == 1) ? 0 : 1;
}
