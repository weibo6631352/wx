/********************************************************************************
** Form generated from reading UI file 'DesolateTotol.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DESOLATETOTOL_H
#define UI_DESOLATETOTOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DesolateTotolClass
{
public:
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_3;
    QLineEdit *lineEdit_filepath;
    QPushButton *pushButton_selfile;
    QPushButton *pushButton_export;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_title;
    QLabel *label_4;
    QLineEdit *lineEdit_title_days;
    QWidget *widget_chart;
    QVBoxLayout *verticalLayout_12;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_title;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_good_name;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *verticalSpacer;
    QTableWidget *tableWidget_totol;

    void setupUi(QDialog *DesolateTotolClass)
    {
        if (DesolateTotolClass->objectName().isEmpty())
            DesolateTotolClass->setObjectName(QStringLiteral("DesolateTotolClass"));
        DesolateTotolClass->resize(524, 880);
        verticalLayout_6 = new QVBoxLayout(DesolateTotolClass);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        lineEdit_filepath = new QLineEdit(DesolateTotolClass);
        lineEdit_filepath->setObjectName(QStringLiteral("lineEdit_filepath"));

        horizontalLayout_3->addWidget(lineEdit_filepath);

        pushButton_selfile = new QPushButton(DesolateTotolClass);
        pushButton_selfile->setObjectName(QStringLiteral("pushButton_selfile"));

        horizontalLayout_3->addWidget(pushButton_selfile);

        pushButton_export = new QPushButton(DesolateTotolClass);
        pushButton_export->setObjectName(QStringLiteral("pushButton_export"));
        pushButton_export->setEnabled(false);

        horizontalLayout_3->addWidget(pushButton_export);


        verticalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(DesolateTotolClass);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_title = new QLineEdit(DesolateTotolClass);
        lineEdit_title->setObjectName(QStringLiteral("lineEdit_title"));

        horizontalLayout_4->addWidget(lineEdit_title);

        label_4 = new QLabel(DesolateTotolClass);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        lineEdit_title_days = new QLineEdit(DesolateTotolClass);
        lineEdit_title_days->setObjectName(QStringLiteral("lineEdit_title_days"));

        horizontalLayout_4->addWidget(lineEdit_title_days);


        verticalLayout_6->addLayout(horizontalLayout_4);

        widget_chart = new QWidget(DesolateTotolClass);
        widget_chart->setObjectName(QStringLiteral("widget_chart"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_chart->sizePolicy().hasHeightForWidth());
        widget_chart->setSizePolicy(sizePolicy);
        verticalLayout_12 = new QVBoxLayout(widget_chart);
        verticalLayout_12->setSpacing(6);
        verticalLayout_12->setContentsMargins(11, 11, 11, 11);
        verticalLayout_12->setObjectName(QStringLiteral("verticalLayout_12"));
        verticalLayout_12->setContentsMargins(25, 15, 25, 15);
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_title = new QLabel(widget_chart);
        label_title->setObjectName(QStringLiteral("label_title"));
        QFont font;
        font.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font.setPointSize(15);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        label_title->setFont(font);
        label_title->setStyleSheet(QStringLiteral("color: rgb(246, 172, 152);"));
        label_title->setAlignment(Qt::AlignCenter);

        verticalLayout_5->addWidget(label_title);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_good_name = new QLabel(widget_chart);
        label_good_name->setObjectName(QStringLiteral("label_good_name"));
        label_good_name->setMinimumSize(QSize(0, 0));
        label_good_name->setMaximumSize(QSize(16777215, 16777215));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\346\245\267\344\275\223"));
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setWeight(75);
        label_good_name->setFont(font1);
        label_good_name->setStyleSheet(QStringLiteral("color: rgb(246, 172, 152);"));
        label_good_name->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_good_name);

        label = new QLabel(widget_chart);
        label->setObjectName(QStringLiteral("label"));
        label->setFont(font1);
        label->setStyleSheet(QStringLiteral("color: rgb(246, 172, 152);"));
        label->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label);

        label_2 = new QLabel(widget_chart);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFont(font1);
        label_2->setStyleSheet(QStringLiteral("color: rgb(246, 172, 152);"));
        label_2->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_2);

        horizontalLayout_9->setStretch(1, 1);

        verticalLayout_5->addLayout(horizontalLayout_9);


        verticalLayout_12->addLayout(verticalLayout_5);

        verticalSpacer = new QSpacerItem(20, 4, QSizePolicy::Minimum, QSizePolicy::Fixed);

        verticalLayout_12->addItem(verticalSpacer);

        tableWidget_totol = new QTableWidget(widget_chart);
        if (tableWidget_totol->columnCount() < 3)
            tableWidget_totol->setColumnCount(3);
        if (tableWidget_totol->rowCount() < 36)
            tableWidget_totol->setRowCount(36);
        tableWidget_totol->setObjectName(QStringLiteral("tableWidget_totol"));
        sizePolicy.setHeightForWidth(tableWidget_totol->sizePolicy().hasHeightForWidth());
        tableWidget_totol->setSizePolicy(sizePolicy);
        QFont font2;
        font2.setPointSize(11);
        font2.setBold(true);
        font2.setWeight(75);
        tableWidget_totol->setFont(font2);
        tableWidget_totol->setFocusPolicy(Qt::NoFocus);
        tableWidget_totol->setFrameShape(QFrame::NoFrame);
        tableWidget_totol->setLineWidth(0);
        tableWidget_totol->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_totol->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_totol->setAutoScroll(false);
        tableWidget_totol->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_totol->setShowGrid(false);
        tableWidget_totol->setRowCount(36);
        tableWidget_totol->setColumnCount(3);
        tableWidget_totol->horizontalHeader()->setVisible(false);
        tableWidget_totol->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget_totol->horizontalHeader()->setDefaultSectionSize(60);
        tableWidget_totol->horizontalHeader()->setHighlightSections(false);
        tableWidget_totol->horizontalHeader()->setStretchLastSection(false);
        tableWidget_totol->verticalHeader()->setVisible(false);
        tableWidget_totol->verticalHeader()->setCascadingSectionResizes(false);
        tableWidget_totol->verticalHeader()->setDefaultSectionSize(20);
        tableWidget_totol->verticalHeader()->setHighlightSections(false);
        tableWidget_totol->verticalHeader()->setStretchLastSection(false);

        verticalLayout_12->addWidget(tableWidget_totol);


        verticalLayout_6->addWidget(widget_chart);


        retranslateUi(DesolateTotolClass);
        QObject::connect(pushButton_selfile, SIGNAL(clicked()), DesolateTotolClass, SLOT(setfilepath()));
        QObject::connect(pushButton_export, SIGNAL(clicked()), DesolateTotolClass, SLOT(on_export()));
        QObject::connect(lineEdit_title, SIGNAL(textChanged(QString)), DesolateTotolClass, SLOT(UpdateDesolateView()));
        QObject::connect(lineEdit_title_days, SIGNAL(textChanged(QString)), DesolateTotolClass, SLOT(UpdateDesolateView()));

        QMetaObject::connectSlotsByName(DesolateTotolClass);
    } // setupUi

    void retranslateUi(QDialog *DesolateTotolClass)
    {
        DesolateTotolClass->setWindowTitle(QApplication::translate("DesolateTotolClass", "\345\206\267\345\256\235\347\273\237\350\256\241", nullptr));
        pushButton_selfile->setText(QApplication::translate("DesolateTotolClass", "\351\200\211\346\213\251\346\226\207\344\273\266", nullptr));
        pushButton_export->setText(QApplication::translate("DesolateTotolClass", "\345\257\274\345\207\272\345\233\276\347\211\207", nullptr));
        label_3->setText(QApplication::translate("DesolateTotolClass", "\345\234\272\345\234\260\357\274\232", nullptr));
        label_4->setText(QApplication::translate("DesolateTotolClass", "\345\244\251\346\225\260\357\274\232", nullptr));
        lineEdit_title_days->setText(QApplication::translate("DesolateTotolClass", "30", nullptr));
        label_title->setText(QApplication::translate("DesolateTotolClass", "\346\240\207\351\242\230", nullptr));
        label_good_name->setText(QApplication::translate("DesolateTotolClass", "\345\256\235\345\220\215\347\247\260", nullptr));
        label->setText(QApplication::translate("DesolateTotolClass", "\350\277\236\347\273\255\346\234\252\345\274\200\345\234\272\346\225\260", nullptr));
        label_2->setText(QApplication::translate("DesolateTotolClass", "\350\277\236\347\273\255\346\234\252\345\274\200\345\244\251\346\225\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DesolateTotolClass: public Ui_DesolateTotolClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DESOLATETOTOL_H
