#pragma once

#include <NodeUtilities.hpp>

namespace lab::tree {

    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::search (const T& key) noexcept
        -> typename BSTBase<T, Compare, DerivedTree>::iterator
    {
        auto found = bstutils::find(_root, key, _comp);
        if (found) {
            auto iter = begin();
            while (iter) {
                if (*iter == key) {
                    break;
                }
                ++iter;
            }
            return iter;
        }
        else {
            return end();
        }
    }

    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::begin() const noexcept
        -> typename BSTBase<T, Compare, DerivedTree>::iterator
    {
        return iterator(bstutils::min(_root));
    }

    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::end() const noexcept
            -> typename BSTBase<T, Compare, DerivedTree>::iterator
    {
        return iterator(nullptr);
    }

    template <typename T, typename Compare, typename DerivedTree>
    void BSTBase<T, Compare, DerivedTree>::simpleInsert (Node<DerivedTree>* toInsert) {
        if (_root == nullptr) {
            _root = toInsert;
        } else {
            bstutils::insertWithParent(_root, toInsert, _comp);
        }
    }

    template <typename T, typename Compare, typename DerivedTree>
    BSTBase<T, Compare, DerivedTree>::~BSTBase ()  {
        if (_root) {
            bstutils::eraseSubTree(_root);
        }
    }
    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::size() const noexcept -> std::size_t {
        return _size;
    }

    template <typename T, typename Compare, typename DerivedTree>
    BSTBase<T, Compare, DerivedTree>::BSTBase (const BSTBase& other)
            : _root(bstutils::clone(other._root)),
              _size(other._size),
              _comp(other._comp)
    {}


    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::operator= (DerivedTree other)
        -> BSTBase<T, Compare, DerivedTree>&
    {
        std::swap (*this, other);
        return *this;
    }

    template <typename T, typename Compare, typename DerivedTree>
    BSTBase<T, Compare, DerivedTree>::BSTBase (const Compare& comp)
            : _comp(comp)
    {}

    template <typename T, typename Compare, typename DerivedTree>
    auto BSTBase<T, Compare, DerivedTree>::compareFunc () const noexcept -> Compare {
        return _comp;
    }
}