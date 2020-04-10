#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <SplayTree.hpp>
#include <RedBlackTree.hpp>
#include <UndoableTree.hpp>
#include <AnyTree.hpp>
#include <TreeDB.hpp>

namespace lab {



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _db(TreeDatabase::instance())
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    _curTreeName = "Default Tree";
    _state = CurPage::Pills;
    _getToDBPage();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::_getToDBPage(){
    ui->stackedWidget->setCurrentIndex(1);
    _dbModel = new QStringListModel(this);
    _loadedNames = _db.loadNames();
    QStringList treeNames;
    for (const auto& name: _loadedNames)
        treeNames << QString::fromStdString(name);
    _dbModel->setStringList(treeNames);
    ui->DBlist->setModel(_dbModel);

}

void MainWindow::_getToPillsPage(){
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::_getToTreeUIPage(){
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::_setCmp(){
    if (ui->Less->isChecked())
        _cmp = CmpType::Less;
    else
        _cmp = CmpType::Greater;
}
void MainWindow::on_BluePill_clicked()
{

    if (_cmp == lab::CmpType::Less){
        auto loadedTree = _db.load<forest::UndoableTree<forest::SplayTree<std::string>>>(_curTreeName);
        if(loadedTree == std::nullopt)
            _tree.setTree(forest::UndoableTree<forest::SplayTree<std::string>>());
        else
            _tree.setTree(loadedTree.value());
    }
    else{
        auto loadedTree = _db.load<forest::UndoableTree<forest::SplayTree<std::string, std::greater<>>>>(_curTreeName);
        if(loadedTree == std::nullopt)
            _tree.setTree(forest::UndoableTree<forest::SplayTree<std::string, std::greater<>>>());
        else
            _tree.setTree(loadedTree.value());
    }
    _getToTreeUIPage();
}

void MainWindow::on_RedPill_clicked()
{
    if (_cmp == lab::CmpType::Less){
        auto loadedTree = _db.load<forest::UndoableTree<forest::RedBlackTree<std::string>>>(_curTreeName);
        if(loadedTree == std::nullopt)
            _tree.setTree(forest::UndoableTree<forest::RedBlackTree<std::string>>());
        else
            _tree.setTree(loadedTree.value());
    }
    else{
        auto loadedTree = _db.load<forest::UndoableTree<forest::RedBlackTree<std::string, std::greater<>>>>(_curTreeName);
        if(loadedTree == std::nullopt)
            _tree.setTree(forest::UndoableTree<forest::RedBlackTree<std::string, std::greater<>>>());
        else
            _tree.setTree(loadedTree.value());
    }
    _getToTreeUIPage();
}

void MainWindow::on_CreateNew_clicked()
{
    _setCmp();  
    _curTreeName = ui->lineEdit->text().toStdString();
    _getToPillsPage();
}

void MainWindow::on_LoadButton_clicked()
{
    _setCmp();
    _curTreeName = _loadedNames[ui->DBlist->currentIndex().row()];
    _getToPillsPage();


}

void MainWindow::on_Undo_clicked()
{
}

void MainWindow::on_Redo_clicked()
{

}

void MainWindow::on_InsertButton_clicked()
{
    ui->TreeOutput->clear();
    auto toInsert = ui->textEdit->toPlainText().toStdString();
    if (toInsert == "")
        return;
    ui->textEdit->clear();
    _tree.insert(toInsert);
    ui->TreeOutput->insertPlainText(QString::fromStdString("INSERTED : \n" + toInsert));
}

void MainWindow::on_Search_clicked()
{
    ui->TreeOutput->clear();
    auto toSearch = ui->textEdit->toPlainText().toStdString();
    if (toSearch == "")
        return;
    ui->textEdit->clear();
    if (_tree.search(toSearch) != _tree.end())
        ui->TreeOutput->insertPlainText(QString::fromStdString("WAS FOUND:\n" + toSearch));
    else
        ui->TreeOutput->insertPlainText(QString::fromStdString("WASN`T FOUND:\n" + toSearch));

}

void MainWindow::on_DeleteButton_clicked()
{
    ui->TreeOutput->clear();
    auto toDelete = ui->textEdit->toPlainText().toStdString();
    if (toDelete == "")
        return;
    _tree.erase(toDelete);
    ui->textEdit->clear();
    ui->TreeOutput->insertPlainText(QString::fromStdString("DELETED:\n" + toDelete));
}

void MainWindow::on_pushButton_clicked()
{
    ui->TreeOutput->clear();
    ui->TreeOutput->insertPlainText("PRINTED TREE:");
    for( const auto& i: _tree)
        ui->TreeOutput->insertPlainText("\n    ->" + QString::fromStdString(i));
}

void MainWindow::on_SaveButton_clicked()
{
    _db.save(_tree, _curTreeName);
    _getToDBPage();

}

}//kjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjnnnnnnnnnnnnnnnnnnnn

