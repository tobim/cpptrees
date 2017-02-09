
#include "tree/tree.hpp"

#include <iostream>

int main () {
    Tree<int, 2> single = tree::singleton<2>{1};
}
