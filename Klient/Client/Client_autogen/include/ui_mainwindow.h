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
    QPushButton *goButton;
    QListWidget *rooms;
    QGroupBox *queueGroup;
    QGridLayout *gridLayout_2;
    QPushButton *muteButton;
    QPushButton *loopButton;
    QListWidget *queue;
    QPushButton *upButton;
    QPushButton *randomButton;
    QPushButton *downButton;
    QPushButton *hideButton;
    QPushButton *skipButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(408, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        connectGroup = new QGroupBox(centralwidget);
        connectGroup->setObjectName(QString::fromUtf8("connectGroup"));
        connectGroup->setGeometry(QRect(20, -20, 351, 111));
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
        radioGroup_2->setGeometry(QRect(10, 70, 391, 321));
        radioGroup = new QGridLayout(radioGroup_2);
        radioGroup->setObjectName(QString::fromUtf8("radioGroup"));
        goButton = new QPushButton(radioGroup_2);
        goButton->setObjectName(QString::fromUtf8("goButton"));

        radioGroup->addWidget(goButton, 3, 0, 1, 1);

        rooms = new QListWidget(radioGroup_2);
        rooms->setObjectName(QString::fromUtf8("rooms"));

        radioGroup->addWidget(rooms, 0, 0, 3, 1);

        queueGroup = new QGroupBox(radioGroup_2);
        queueGroup->setObjectName(QString::fromUtf8("queueGroup"));
        gridLayout_2 = new QGridLayout(queueGroup);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        muteButton = new QPushButton(queueGroup);
        muteButton->setObjectName(QString::fromUtf8("muteButton"));

        gridLayout_2->addWidget(muteButton, 4, 0, 1, 1);

        loopButton = new QPushButton(queueGroup);
        loopButton->setObjectName(QString::fromUtf8("loopButton"));

        gridLayout_2->addWidget(loopButton, 5, 0, 1, 1);

        queue = new QListWidget(queueGroup);
        queue->setObjectName(QString::fromUtf8("queue"));

        gridLayout_2->addWidget(queue, 1, 0, 1, 3);

        upButton = new QPushButton(queueGroup);
        upButton->setObjectName(QString::fromUtf8("upButton"));

        gridLayout_2->addWidget(upButton, 2, 1, 1, 1);

        randomButton = new QPushButton(queueGroup);
        randomButton->setObjectName(QString::fromUtf8("randomButton"));

        gridLayout_2->addWidget(randomButton, 5, 1, 1, 1);

        downButton = new QPushButton(queueGroup);
        downButton->setObjectName(QString::fromUtf8("downButton"));

        gridLayout_2->addWidget(downButton, 4, 1, 1, 1);

        hideButton = new QPushButton(queueGroup);
        hideButton->setObjectName(QString::fromUtf8("hideButton"));

        gridLayout_2->addWidget(hideButton, 2, 0, 1, 1);

        skipButton = new QPushButton(queueGroup);
        skipButton->setObjectName(QString::fromUtf8("skipButton"));

        gridLayout_2->addWidget(skipButton, 0, 0, 1, 1);


        radioGroup->addWidget(queueGroup, 0, 1, 4, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        connectButton->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        goButton->setText(QApplication::translate("MainWindow", "Go", nullptr));
        muteButton->setText(QApplication::translate("MainWindow", "Mute", nullptr));
        loopButton->setText(QApplication::translate("MainWindow", "Loop", nullptr));
        upButton->setText(QApplication::translate("MainWindow", "^", nullptr));
        randomButton->setText(QApplication::translate("MainWindow", "Rnd", nullptr));
        downButton->setText(QApplication::translate("MainWindow", "v", nullptr));
        hideButton->setText(QApplication::translate("MainWindow", "Hide", nullptr));
        skipButton->setText(QApplication::translate("MainWindow", "Skip", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
