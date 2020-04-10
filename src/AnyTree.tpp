#pragma once

namespace lab::forest {

    template <typename ValueType, typename... Comparators>
    template <typename Tree>
    AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::AnyTree (const Tree& _tree)
         : m_tree (_tree)
    {}

    template <typename ValueType, typename... Comparators>
    template <typename Tree, typename>
    AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>&
    AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::operator= (const Tree& other) {

         if constexpr (lab::detail::is_template_instantiation<Tree, UndoableTree>) {
            m_tree = other;
         }
         else {
             m_tree = UndoableTree {other};
         }
         return *this;
    }


    template <typename ValueType, typename... Comparators>
    auto AnyTree <SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::size () const noexcept -> std::size_t {
        return std::visit ([] (auto&& _tree) -> std::size_t {
                    return _tree.size();
                       }, m_tree);
    }

    template <typename ValueType, typename... Comparators>
    auto AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::end () const noexcept -> iterator {
        if (!m_valid_iter) {
            updateIter();
        }
        return m_enumerator.end();
    }

    template <typename ValueType, typename... Comparators>
    auto AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::begin () const noexcept -> iterator {
        if (!m_valid_iter) {
            updateIter();
        }
        return m_enumerator.begin();
    }

    template <typename ValueType, typename... Comparators>
    auto AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::search (const ValueType& value) noexcept -> iterator {
        std::size_t pos{};
        std::visit ([&] (auto&& _tree) {
                       pos = std::distance(_tree.begin(), _tree.search(value));}, m_tree);
        if (!m_valid_iter) {
            updateIter();
        }
        return m_enumerator.begin() + pos;
    }

    template <typename ValueType, typename... Comparators>
    void AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::erase (const ValueType& value) {
        std::visit([&value] (auto&& _tree) {_tree.erase(value);}, m_tree);
        m_valid_iter = false;
    }

    template <typename ValueType, typename... Comparators>
    void AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::insert (const ValueType& value) {
          std::visit([&value] (auto&& _tree) {_tree.insert(value);}, m_tree);
          m_valid_iter = false;
    }

    template<typename ValueType, typename... Comparators>
    void AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::undo() {
        std::visit([] (auto&& tree) {tree.undo();}, m_tree);
        m_valid_iter = false;
    }

    template<typename ValueType, typename... Comparators>
    void AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::redo() {
        std::visit([] (auto&& tree) {tree.redo();}, m_tree);
        m_valid_iter = false;
    }

    template <typename ValueType, typename... Comparators>
    void AnyTree<SupportedValueType<ValueType>, SupportedComparators<Comparators...>>::updateIter () const {
        std::visit([this] (auto&& _tree) {
                    m_enumerator.assign(_tree.begin(), _tree.end());
                    }, m_tree);
        m_valid_iter = true;
    }
}