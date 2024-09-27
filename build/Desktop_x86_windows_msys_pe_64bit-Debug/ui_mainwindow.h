/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QTableWidget *stockTable;
    QTableWidget *cutTable;
    QPushButton *optimizeButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(600, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        stockTable = new QTableWidget(centralwidget);
        if (stockTable->columnCount() < 2)
            stockTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        stockTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        stockTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        stockTable->setObjectName("stockTable");
        stockTable->setColumnCount(2);
        stockTable->setRowCount(5);

        verticalLayout->addWidget(stockTable);

        cutTable = new QTableWidget(centralwidget);
        if (cutTable->columnCount() < 2)
            cutTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        cutTable->setHorizontalHeaderItem(0, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        cutTable->setHorizontalHeaderItem(1, __qtablewidgetitem3);
        cutTable->setObjectName("cutTable");
        cutTable->setColumnCount(2);
        cutTable->setRowCount(5);

        verticalLayout->addWidget(cutTable);

        optimizeButton = new QPushButton(centralwidget);
        optimizeButton->setObjectName("optimizeButton");

        verticalLayout->addWidget(optimizeButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Cutting Optimizer", nullptr));
        QTableWidgetItem *___qtablewidgetitem = stockTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Length", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = stockTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = cutTable->horizontalHeaderItem(0);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Length", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = cutTable->horizontalHeaderItem(1);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Quantity", nullptr));
        optimizeButton->setText(QCoreApplication::translate("MainWindow", "Optimize Cuts", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
