#pragma once

#include "RBNodeUtilities.hpp"

namespace tree::utils {

    template <typename T>
    class DeletionFixRBTRee {
    private:
        Node<RedBlackTree<T>>* root;
        Node<RedBlackTree<T>>* doubleBlackNode; //DBN
        Node<RedBlackTree<T>>* DBNSibling;
        Node<RedBlackTree<T>>* DBNParent;

        Node<RedBlackTree<T>>* nodeToDelete;
        Node<RedBlackTree<T>>* nodeToReplace;
        Node<RedBlackTree<T>>* parentNodeToDelete;
        bool bothBlack;

        void redSiblingCase() {
            DBNParent->color = tree::Node<tree::RedBlackTree<T>>::Red;
            DBNSibling->color = tree::Node<tree::RedBlackTree<T>>::Black;
            if (tree::utils::isLeftSon(DBNSibling)) {
                // left case
                tree::utils::rotateRight(root, DBNParent);
            } else {
                // right case
                tree::utils::rotateLeft(root, DBNParent);
            }
            fixBothBlack();
        }

        void siblingHasLeftRedChild() {
            if (tree::utils::isLeftSon(DBNSibling)) {
                // left left
                (DBNSibling->left)->color = DBNSibling->color;
                DBNSibling->color = DBNParent->color;
                tree::utils::rotateRight(root, DBNParent);
            } else {
                // right left
                (DBNSibling->left)->color = DBNParent->color;
                tree::utils::rotateRight(root, DBNSibling);
                tree::utils::rotateLeft(root, DBNParent);
            }
        }

        void siblingHasRightRedChild() {
            if (tree::utils::isLeftSon(DBNSibling)) {
                // left right
                (DBNSibling->right)->color = DBNParent->color;
                tree::utils::rotateLeft(root, DBNSibling);
                tree::utils::rotateRight(root, DBNParent);
            } else {
                // right right
                (DBNSibling->right)->color = DBNSibling->color;
                DBNSibling->color = DBNParent->color;
                tree::utils::rotateLeft(root, DBNParent);
            }
        }

        void siblingHasOnlyBlackChildren() {
            DBNSibling->color = tree::Node<tree::RedBlackTree<T>>::Red;
            if (DBNParent->color == tree::Node<tree::RedBlackTree<T>>::Black) {
                doubleBlackNode = DBNParent;
                fixBothBlack();
            }
            else {
                DBNParent->color = tree::Node<tree::RedBlackTree<T>>::Black;
            }
        }

        void blackSiblingCase() {
            if (tree::utils::hasRedChild(DBNSibling)) {
                // at least 1 Red child
                if (DBNSibling->left != nullptr && DBNSibling->left->color == tree::Node<tree::RedBlackTree<T>>::Red) {
                    siblingHasLeftRedChild();
                } else {
                    siblingHasRightRedChild();
                }
                DBNParent->color = tree::Node<tree::RedBlackTree<T>>::Black;
            } else {
                siblingHasOnlyBlackChildren();
            }
        }

        void fixBothBlack() {
            if (doubleBlackNode == root) { // Reached root
                return;
            }

            DBNSibling = tree::utils::sibling(doubleBlackNode);
            DBNParent = doubleBlackNode->parent;

            if (DBNSibling == nullptr) {
                // No sibling, double Black pushed up
                doubleBlackNode = DBNParent;
                fixBothBlack();
            } else if (DBNSibling->color == tree::Node<tree::RedBlackTree<T>>::Red) {
                redSiblingCase();
            } else {
                blackSiblingCase();
            }
        }

        void nodeToDeleteIsLeaf() {
                if (bothBlack) {
                    doubleBlackNode = nodeToDelete;
                    fixBothBlack();
                } else {
                    if (tree::utils::sibling(nodeToDelete) != nullptr)
                        // sibling is not null, make it Red
                        tree::utils::sibling(nodeToDelete)->color = tree::Node<tree::RedBlackTree<T>>::Red;
                }

                // delete node from the tree
                if (tree::utils::isLeftSon(nodeToDelete)) {
                    parentNodeToDelete->left = nullptr;
                } else {
                    parentNodeToDelete->right = nullptr;
                }
            delete nodeToDelete;
        }

        void nodeToDeleteHasOnlyOneChild() {
            if (nodeToDelete == root) {
                nodeToDelete->data = nodeToReplace->data;
                delete nodeToReplace;
            } else {
                // Detach nodeToDelete from tree and move nodeToReplace up
                if (tree::utils::isLeftSon(nodeToDelete)) {
                    parentNodeToDelete->left = nodeToReplace;
                } else {
                    parentNodeToDelete->right = nodeToReplace;
                }
                delete nodeToDelete;

                nodeToReplace->parent = parentNodeToDelete;
                if (bothBlack) {
                    doubleBlackNode = nodeToReplace;
                    fixBothBlack();
                } else {
                    nodeToReplace->color = tree::Node<tree::RedBlackTree<T>>::Black;
                }
            }
        }

        void eraseNode() {
            nodeToReplace = tree::utils::findReplacement(nodeToDelete);
            parentNodeToDelete = nodeToDelete->parent;
            bothBlack = ((nodeToReplace == nullptr
                          || nodeToReplace->color == tree::Node<tree::RedBlackTree<T>>::Black)
                         && (nodeToDelete->color == tree::Node<tree::RedBlackTree<T>>::Black));

            if (nodeToReplace == nullptr) {
                nodeToDeleteIsLeaf();
            } else if (nodeToDelete->left == nullptr || nodeToDelete->right == nullptr) {
                nodeToDeleteHasOnlyOneChild();
            } else { // nodeToDelete has 2 children, swap values with replacement node and recurse
                tree::utils::swapData(nodeToReplace, nodeToDelete);
                nodeToDelete = nodeToReplace;
                eraseNode();
            }
        }

    public:
        DeletionFixRBTRee(Node<RedBlackTree<T>>*& _root, Node<RedBlackTree<T>>* _nodeToDelete)
                : root(_root), nodeToDelete(_nodeToDelete) {
            eraseNode();
            _root = root;
        }
    };
}