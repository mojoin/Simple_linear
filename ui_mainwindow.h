/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "graphicsview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QVBoxLayout *verticalLayout;
    QRadioButton *radioButton_x;
    QRadioButton *radioButton_xy1;
    QRadioButton *radioButton_xy2;
    QPushButton *pushButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_daoru;
    QPushButton *pushButton_tianjia;
    QPushButton *pushButton_shanchu;
    QTableWidget *tableWidget;
    QPushButton *pushButton_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    GraphicsView *graphicsView;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QPushButton *pushButton_fangda;
    QPushButton *pushButton_suoxiao;
    QPushButton *pushButton_chongzhi;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(715, 340);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/linear.ico"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        MainWindow->setWindowIcon(icon);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_4 = new QHBoxLayout(centralwidget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName("gridLayout_2");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout_2->addWidget(label, 5, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        radioButton_x = new QRadioButton(groupBox);
        radioButton_x->setObjectName("radioButton_x");
        radioButton_x->setAutoExclusive(true);

        verticalLayout->addWidget(radioButton_x);

        radioButton_xy1 = new QRadioButton(groupBox);
        radioButton_xy1->setObjectName("radioButton_xy1");

        verticalLayout->addWidget(radioButton_xy1);

        radioButton_xy2 = new QRadioButton(groupBox);
        radioButton_xy2->setObjectName("radioButton_xy2");

        verticalLayout->addWidget(radioButton_xy2);


        gridLayout_2->addLayout(verticalLayout, 2, 0, 1, 1);

        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");

        gridLayout_2->addWidget(pushButton, 3, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        pushButton_daoru = new QPushButton(groupBox);
        pushButton_daoru->setObjectName("pushButton_daoru");

        horizontalLayout->addWidget(pushButton_daoru);

        pushButton_tianjia = new QPushButton(groupBox);
        pushButton_tianjia->setObjectName("pushButton_tianjia");

        horizontalLayout->addWidget(pushButton_tianjia);

        pushButton_shanchu = new QPushButton(groupBox);
        pushButton_shanchu->setObjectName("pushButton_shanchu");

        horizontalLayout->addWidget(pushButton_shanchu);


        gridLayout_2->addLayout(horizontalLayout, 1, 0, 1, 1);

        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        tableWidget->setObjectName("tableWidget");
        tableWidget->horizontalHeader()->setCascadingSectionResizes(true);

        gridLayout_2->addWidget(tableWidget, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName("pushButton_2");

        gridLayout_2->addWidget(pushButton_2, 4, 0, 1, 1);


        horizontalLayout_3->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralwidget);
        groupBox_2->setObjectName("groupBox_2");
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName("gridLayout");
        graphicsView = new GraphicsView(groupBox_2);
        graphicsView->setObjectName("graphicsView");

        gridLayout->addWidget(graphicsView, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        horizontalLayout_2->addWidget(label_2);

        pushButton_fangda = new QPushButton(groupBox_2);
        pushButton_fangda->setObjectName("pushButton_fangda");

        horizontalLayout_2->addWidget(pushButton_fangda);

        pushButton_suoxiao = new QPushButton(groupBox_2);
        pushButton_suoxiao->setObjectName("pushButton_suoxiao");

        horizontalLayout_2->addWidget(pushButton_suoxiao);

        pushButton_chongzhi = new QPushButton(groupBox_2);
        pushButton_chongzhi->setObjectName("pushButton_chongzhi");

        horizontalLayout_2->addWidget(pushButton_chongzhi);


        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        horizontalLayout_3->addWidget(groupBox_2);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 715, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\346\265\213\347\273\230\345\237\272\347\241\200\347\220\206\350\256\272\344\270\216\346\226\271\346\263\225(\351\242\2302)", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\346\225\260\346\215\256\347\256\241\347\220\206\345\231\250", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\213\237\345\220\210\347\273\223\346\236\234\357\274\232\347\255\211\345\276\205\350\256\241\347\256\227...", nullptr));
        radioButton_x->setText(QCoreApplication::translate("MainWindow", "Y \346\230\257\350\247\202\346\265\213\345\200\274\357\274\210X \346\227\240\350\257\257\345\267\256\357\274\211\346\231\256\351\200\232\346\234\200\345\260\217\344\272\214\344\271\230\346\263\225\357\274\210OLS\357\274\211", nullptr));
        radioButton_xy1->setText(QCoreApplication::translate("MainWindow", "XY \351\203\275\346\230\257\350\247\202\346\265\213\345\200\274  (\347\211\271\345\276\201\345\200\274\345\210\206\350\247\243\346\263\225)\346\200\273\344\275\223\346\234\200\345\260\217\344\272\214\344\271\230\346\263\225\357\274\210TLS\357\274\211", nullptr));
        radioButton_xy2->setText(QCoreApplication::translate("MainWindow", "XY \351\203\275\346\230\257\350\247\202\346\265\213\345\200\274  (\345\245\207\345\274\202\345\200\274\345\210\206\350\247\243\346\263\225)\346\200\273\344\275\223\346\234\200\345\260\217\344\272\214\344\271\230\346\263\225\357\274\210TLS\357\274\211", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\350\256\241\347\256\227\345\271\263\345\267\256", nullptr));
        pushButton_daoru->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\205\245\346\225\260\346\215\256", nullptr));
        pushButton_tianjia->setText(QCoreApplication::translate("MainWindow", "\346\267\273\345\212\240\347\202\271", nullptr));
        pushButton_shanchu->setText(QCoreApplication::translate("MainWindow", "\345\210\240\351\231\244\351\200\211\344\270\255\347\202\271", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "\345\257\274\345\207\272\345\233\276\347\211\207(PNG)", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\345\233\276\345\275\242\346\230\276\347\244\272", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\347\274\251\346\224\276\346\216\247\345\210\266\357\274\232", nullptr));
        pushButton_fangda->setText(QCoreApplication::translate("MainWindow", "\346\224\276\345\244\247", nullptr));
        pushButton_suoxiao->setText(QCoreApplication::translate("MainWindow", "\347\274\251\345\260\217", nullptr));
        pushButton_chongzhi->setText(QCoreApplication::translate("MainWindow", "\351\207\215\347\275\256", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
