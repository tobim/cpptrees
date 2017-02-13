
#pragma once

#include "mpark/variant.hpp"
#include <algorithm>
#include <array>
#include <memory>
#include <stack>
#include <utility>
#include <vector>
#include <iostream>

namespace tree {

struct Leaf {
};

template <size_t arity, typename T>
struct Tree;

template <size_t arity, typename T>
struct Node {
    using tree_type     = Tree<arity, T>;
    using child_type    = std::unique_ptr<tree_type>;
    using children_type = std::array<child_type, arity>;
    T value_;
    children_type children;

    Node(T v) : value_{v} {}
    Node(T v, std::array<tree_type, arity>&& cdren) : value_{v}
    {
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
    using var_type = mpark::variant<Leaf, node_type>;

    Tree() = default;
    explicit Tree(node_type&& node) : var_type{std::forward<node_type>(node)}
    {
    }
    explicit Tree(Leaf&& leaf) : var_type{std::forward<Leaf>(leaf)} {}
    static Tree empty_tree() { return Tree{Leaf{}}; }
    static Tree singleton(T&& v) { return Tree{{v}}; }

    struct df_iterator;
    struct bf_iterator;

    struct end_sentinel {
    };

    df_iterator df_begin() { return df_iterator{this}; }
    bf_iterator bf_begin() { return bf_iterator{this}; }
    end_sentinel end() { return {}; }

    bool empty() const {
        return var_type::index() == 0;
    }
};

template <typename F>
struct DepthVisitor {
    F f;
    DepthVisitor(F&& f) : f{std::forward<F>(f)} {}
    void operator()(const Leaf&) {}
    template <size_t arity, typename T>
    void operator()(const Node<arity, T>& node)
    {
        f(node.value_);
        std::for_each(node.children.begin(), node.children.end(),
                      [this](auto& child) {
                          if (child)
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
    void operator()(const Leaf&) {}
    template <size_t arity, typename T>
    void operator()(const Node<arity, T>& node)
    {
        f(node.value_);
        for (auto& c : node.children) {
            if (c) {
                g(*c);
            }
        }
    }
};

template <size_t arity, typename T, typename F>
void bf(const std::vector<Tree<arity, T>*>& trees, F&& f)
{
    std::vector<Tree<arity, T>*> children;
    auto inserter = [&children](Tree<arity, T>& c) { children.push_back(&c); };

    for (auto& t : trees) {
        mpark::visit(
            BreadthVisitor<F, decltype(inserter)>{std::forward<F>(f), inserter},
            *t);
    }

    if (!children.empty())
        bf(children, f);
}
}

template <size_t arity, typename T, typename F>
void breadth_first(Tree<arity, T>& tree, F&& f)
{
    impl::bf(std::vector<Tree<arity, T>*>{&tree}, std::forward<F>(f));
}

template <size_t arity, typename T>
struct Tree<arity, T>::bf_iterator {
    using difference_type = std::ptrdiff_t;
    using value_typetree = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::forward_iterator_tag;

  private:
    using tree_type = Tree<arity, T>;
    using node_type = typename tree_type::node_type;

    std::vector<tree_type*> current_layer;
    std::vector<tree_type*> next_layer;
    typename std::vector<tree_type *>::iterator it, end;
    bool finished = false;

    void register_children()
    {
        auto& node = mpark::get<node_type>(**it);
        for (auto& c : node.children) {
            if (c) {
                next_layer.push_back(&*c);
            }
        }
    }

  public:
    bf_iterator(tree_type* root)
        : current_layer{root}, it{current_layer.begin()},
          end{current_layer.end()}
    {
        if ((*it)->index() != 1) {
            ++(*this);
        }
        else {
            register_children();
        }
    }

    bf_iterator(const bf_iterator& other)
        : current_layer{other.current_layer}, next_layer{other.next_layer},
          it{other.it}, end{other.end}
    {
    }

    bf_iterator& operator++()
    {
        if (finished) {
            return *this;
        }

        while (++it == end || (*it)->index() != 1) {
            if (it == end) {
                if (next_layer.empty()) {
                    finished = true;
                    return *this;
                }
                else {
                    std::swap(current_layer, next_layer);
                    next_layer.clear();
                    it  = current_layer.begin();
                    end = current_layer.end();
                    break;
                }
            }
        }

        register_children();
        return *this;
    }

    bf_iterator operator++(int) {
        auto ret = *this;
        ++(*this);
        return ret;
    }

    reference operator*() { return mpark::get<node_type>(**it).value_; }
    friend bool operator==(const bf_iterator& bfi, end_sentinel)
    {
        return bfi.finished;
    }
    friend bool operator!=(const bf_iterator& bfi, end_sentinel es)
    {
        return !(bfi == es);
    }
};

template <size_t arity, typename T>
struct Tree<arity, T>::df_iterator {
    using difference_type   = std::ptrdiff_t;
    using value_typetree    = T;
    using pointer           = T*;
    using reference         = T&;
    using iterator_category = std::forward_iterator_tag;

  private:
    using tree_type  = Tree<arity, T>;
    using node_type  = typename tree_type::node_type;
    using cit_type   = typename node_type::children_type::iterator;
    using level_type = std::pair<cit_type, cit_type>;

    std::stack<level_type> up;
    tree_type* current;

  public:
    df_iterator(tree_type* root) : current{root}
    {
        if (current->index() != 1) {
            current = nullptr;
        }
    }

    df_iterator& operator++()
    {
        if (current) {
            auto& node = mpark::get<node_type>(*current);
            auto next  = node.children.begin();
            auto end   = node.children.end();
            while (true) {
                next = std::find_if(next, end, [](auto& c) -> bool {
                    return c ? c->index() == 1 : false;
                });
                if (next != end) {
                    current = &**next;
                    up.push({++next, end});
                    break;
                }
                else {
                    if (up.empty()) {
                        current = nullptr;
                        break;
                    }
                    else {
                        next = up.top().first;
                        end  = up.top().second;
                        up.pop();
                    }
                }
            }
        }
        return *this;
    }

    df_iterator operator++(int) {
        auto ret = *this;
        ++(*this);
        return ret;
    }

    reference operator*() { return mpark::get<node_type>(*current).value_; }
    friend bool operator==(const df_iterator& dfi, end_sentinel)
    {
        return dfi.current == nullptr;
    }
    friend bool operator!=(const df_iterator& dfi, end_sentinel es)
    {
        return !(dfi == es);
    }
};
}

using tree::Tree;
