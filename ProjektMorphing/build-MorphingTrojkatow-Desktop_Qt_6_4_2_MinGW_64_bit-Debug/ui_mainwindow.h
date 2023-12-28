/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *glowny;
    QWidget *docelowy;
    QSlider *klatki;
    QPushButton *Prev;
    QPushButton *Next;
    QSpinBox *klatkiS;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1268, 633);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        glowny = new QWidget(centralwidget);
        glowny->setObjectName("glowny");
        glowny->setGeometry(QRect(50, 10, 500, 500));
        docelowy = new QWidget(centralwidget);
        docelowy->setObjectName("docelowy");
        docelowy->setGeometry(QRect(700, 10, 500, 500));
        klatki = new QSlider(centralwidget);
        klatki->setObjectName("klatki");
        klatki->setGeometry(QRect(620, 160, 18, 160));
        klatki->setOrientation(Qt::Vertical);
        Prev = new QPushButton(centralwidget);
        Prev->setObjectName("Prev");
        Prev->setGeometry(QRect(50, 520, 83, 29));
        Next = new QPushButton(centralwidget);
        Next->setObjectName("Next");
        Next->setGeometry(QRect(470, 520, 83, 29));
        klatkiS = new QSpinBox(centralwidget);
        klatkiS->setObjectName("klatkiS");
        klatkiS->setGeometry(QRect(610, 330, 48, 29));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1268, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        Prev->setText(QCoreApplication::translate("MainWindow", "Prev", nullptr));
        Next->setText(QCoreApplication::translate("MainWindow", "Next", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
