
#pragma once

#include <array>
#include <memory>
#include <variant>

namespace tree {

struct Leaf {};

template <typename T, size_t arity>
struct Tree;

template <typename T, size_t arity>
struct Node
{
    T value_;
    std::array< std::unique_ptr< Tree<T, arity> >, arity> children;
};

template <typename T, size_t arity>
struct Tree : std::variant<Leaf, Node<T, arity>> {};


template <typename T, size_t arity>
Tree<T, arity> singleton(T&& v) {
    return {v};
}

}

using tree::Tree;
