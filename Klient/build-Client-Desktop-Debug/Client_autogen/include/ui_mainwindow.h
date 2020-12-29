/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGroupBox *connectGroup;
    QGridLayout *gridLayout;
    QPushButton *connectButton;
    QTextEdit *textEdit;
    QGroupBox *radioGroup_2;
    QGridLayout *radioGroup;
    QListWidget *rooms;
    QPushButton *goButton;
    QGroupBox *queueGroup;
    QGridLayout *gridLayout_2;
    QPushButton *addButton;
    QPushButton *deleteButton;
    QListWidget *queue;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(408, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        connectGroup = new QGroupBox(centralwidget);
        connectGroup->setObjectName(QString::fromUtf8("connectGroup"));
        connectGroup->setGeometry(QRect(30, 0, 351, 81));
        connectGroup->setCheckable(false);
        gridLayout = new QGridLayout(connectGroup);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        connectButton = new QPushButton(connectGroup);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        gridLayout->addWidget(connectButton, 0, 0, 1, 1);

        textEdit = new QTextEdit(connectGroup);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));

        gridLayout->addWidget(textEdit, 1, 0, 1, 1);

        radioGroup_2 = new QGroupBox(centralwidget);
        radioGroup_2->setObjectName(QString::fromUtf8("radioGroup_2"));
        radioGroup_2->setEnabled(false);
        radioGroup_2->setGeometry(QRect(10, 90, 391, 261));
        radioGroup = new QGridLayout(radioGroup_2);
        radioGroup->setObjectName(QString::fromUtf8("radioGroup"));
        rooms = new QListWidget(radioGroup_2);
        rooms->setObjectName(QString::fromUtf8("rooms"));

        radioGroup->addWidget(rooms, 2, 0, 1, 1);

        goButton = new QPushButton(radioGroup_2);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        radioGroup->addWidget(goButton, 3, 0, 1, 1);

        queueGroup = new QGroupBox(radioGroup_2);
        queueGroup->setObjectName(QString::fromUtf8("queueGroup"));
        gridLayout_2 = new QGridLayout(queueGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        addButton = new QPushButton(queueGroup);
        addButton->setObjectName(QString::fromUtf8("addButton"));

        gridLayout_2->addWidget(addButton, 1, 0, 1, 1);

        deleteButton = new QPushButton(queueGroup);
        deleteButton->setObjectName(QString::fromUtf8("deleteButton"));

        gridLayout_2->addWidget(deleteButton, 1, 1, 1, 1);

        queue = new QListWidget(queueGroup);
        queue->setObjectName(QString::fromUtf8("queue"));

        gridLayout_2->addWidget(queue, 0, 0, 1, 2);


        radioGroup->addWidget(queueGroup, 2, 1, 2, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        goButton->setText(QApplication::translate("MainWindow", "Go", nullptr));
        addButton->setText(QApplication::translate("MainWindow", "Add", nullptr));
        deleteButton->setText(QApplication::translate("MainWindow", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
