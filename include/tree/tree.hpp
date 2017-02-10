
#pragma once

#include <array>
#include <memory>
#include "mpark/variant.hpp"

namespace tree {

struct Leaf {};

template <size_t arity, typename T>
struct Tree;

template <size_t arity, typename T>
struct Node
{
    T value_;
    std::array< std::unique_ptr< Tree<arity, T> >, arity> children;
};

template <size_t arity, typename T>
struct Tree : mpark::variant<Leaf, Node<arity, T>> {
    using VNode = Node<arity, T>;
    using VTree = mpark::variant<Leaf, VNode>;
    Tree() = default;
    explicit Tree(VNode&& node) : VTree{std::forward<VNode>(node)} {}
    explicit Tree(Leaf&& leaf) : VTree{std::forward<Leaf>(leaf)} {}
};


template <size_t arity, typename T>
Tree<arity, T> singleton(T&& v) {
    return Tree<arity, T>{{v}};
}


template <size_t arity, typename T, typename F>
void depth_first(Tree<arity, T>& tree, F f)
{

}

}

using tree::Tree;
