
#include "tree/tree.hpp"

int main () {
    Tree<2, int> single = tree::singleton<2>(1);
    auto& node = mpark::get<1>(single);
    return (node.value_ == 1) ? 0 : 1;
}
