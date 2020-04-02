#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

#include <SplayTree.hpp>
#include <RedBlackTree.hpp>
#include <UndoableTree.hpp>
#include <AnyTree.hpp>



namespace lab {



enum CurPage{Pills, DB, TreeUI};

/*class TreeDatabase;*/
/*
namespace tree {
    template<typename, typename>
    class AnyTree;
    template<typename>
    class SupportedValueType;

    template<typename... T>
    class SupportedComparators;
}*/


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_BluePill_clicked();

    void on_RedPill_clicked();

private:
    Ui::MainWindow *ui;
    CurPage _state;
    std::string _curTreeName;
    //TreeDatabase& _db = TreeDatabase::instance();
    //tree::AnyTree<SupportedValueType<std::string>, SupportedComparators<std::less<>, std::greater<>>> _tree;
};
}
#endif // MAINWINDOW_H
