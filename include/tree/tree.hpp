
#pragma once

#include <array>
#include <memory>
#include <vector>
#include <algorithm>
#include "mpark/variant.hpp"

namespace tree {

struct Leaf {};

template <size_t arity, typename T>
struct Tree;

template <size_t arity, typename T>
struct Node
{
    using tree_type = Tree<arity, T>;
    using child_type = std::unique_ptr< tree_type >;
    using children_type = std::array< child_type, arity>;
    T value_;
    children_type children;

    Node(T v) : value_{v} {}
    //Node(T v, std::initializer_list<tree_type> cdren) : value_{v}, children{cdren.begin(), cdren.end()} {}
    Node(T v, std::array<tree_type, arity>&& cdren) : value_{v} {
        size_t idx = 0;
        for (auto& c : cdren) {
            children[idx] = std::make_unique<tree_type>(std::move(c));
            idx++;
        }
    }
};

template <size_t arity, typename T>
struct Tree : mpark::variant<Leaf, Node<arity, T>> {
    using node_type = Node<arity, T>;
    using tree_type = mpark::variant<Leaf, node_type>;

    Tree() = default;
    explicit Tree(node_type&& node) : tree_type{std::forward<node_type>(node)} {}
    explicit Tree(Leaf&& leaf) : tree_type{std::forward<Leaf>(leaf)} {}

    //Tree(Tree&& t) = default;

    static
    Tree empty_tree() {
        return Tree{Leaf{}};
    }

    static
    Tree singleton(T&& v) {
        return Tree{{v}};
    }

};


template <typename F>
struct DepthVisitor {
    F f;
    DepthVisitor(F&& f) : f{std::forward<F>(f)} {}
    void operator() (const Leaf& leaf) {}
    template <size_t arity, typename T>
    void operator() (const Node<arity, T>& node) {
        f(node.value_);
        std::for_each(node.children.begin(), node.children.end(), [this](auto& child) {
                if( child )
                    mpark::visit(*this, *child);
                });
    }
};

template <size_t arity, typename T, typename F>
void depth_first(Tree<arity, T>& tree, F&& f)
{
    mpark::visit(DepthVisitor<F>{std::move(f)}, tree);
}

namespace impl {

template <typename F, typename G>
struct BreadthVisitor {
    F f;
    G g;
    BreadthVisitor(F&& f, G g) : f{std::forward<F>(f)}, g{g} {}
    void operator() (const Leaf& leaf) {}

    template <size_t arity, typename T>
    void operator() (const Node<arity, T>& node) {
        f(node.value_);
        for( auto& c : node.children ) {
            if (c) {
                g(*c);
            }
        }
    }
};

template <size_t arity, typename T, typename F>
void bf(const std::vector<Tree<arity, T>*>& trees, F&& f) {
    std::vector<Tree<arity, T>*> children;
    auto inserter = [&children](Tree<arity, T>& c) { children.push_back(&c); };

    for (auto& t : trees) {
        mpark::visit(BreadthVisitor<F, decltype(inserter)>{
                std::forward<F>(f), inserter
        }, *t);
    }

    if (!children.empty())
        bf(children, f);
}
}

template <size_t arity, typename T, typename F>
void breadth_first(Tree<arity, T>& tree, F&& f) {
    impl::bf( std::vector<Tree<arity, T>*>{&tree}, std::forward<F>(f) );
}

}

using tree::Tree;
