/********************************************************************************
** Form generated from reading UI file 'DayTotolTable.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DAYTOTOLTABLE_H
#define UI_DAYTOTOLTABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DayTotolTableClass
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label_date_path;
    QToolButton *toolButton_selete_datedir;
    QToolButton *toolButton;
    QLabel *label;

    void setupUi(QWidget *DayTotolTableClass)
    {
        if (DayTotolTableClass->objectName().isEmpty())
            DayTotolTableClass->setObjectName(QStringLiteral("DayTotolTableClass"));
        DayTotolTableClass->resize(557, 82);
        verticalLayout = new QVBoxLayout(DayTotolTableClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_date_path = new QLabel(DayTotolTableClass);
        label_date_path->setObjectName(QStringLiteral("label_date_path"));

        horizontalLayout->addWidget(label_date_path);

        toolButton_selete_datedir = new QToolButton(DayTotolTableClass);
        toolButton_selete_datedir->setObjectName(QStringLiteral("toolButton_selete_datedir"));

        horizontalLayout->addWidget(toolButton_selete_datedir);

        toolButton = new QToolButton(DayTotolTableClass);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        horizontalLayout->addWidget(toolButton);


        verticalLayout->addLayout(horizontalLayout);

        label = new QLabel(DayTotolTableClass);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);


        retranslateUi(DayTotolTableClass);
        QObject::connect(toolButton_selete_datedir, SIGNAL(clicked()), DayTotolTableClass, SLOT(on_change_date()));
        QObject::connect(toolButton, SIGNAL(clicked()), DayTotolTableClass, SLOT(on_totol()));

        QMetaObject::connectSlotsByName(DayTotolTableClass);
    } // setupUi

    void retranslateUi(QWidget *DayTotolTableClass)
    {
        DayTotolTableClass->setWindowTitle(QApplication::translate("DayTotolTableClass", "\346\227\245\347\273\223\347\273\237\350\256\241", nullptr));
        label_date_path->setText(QString());
        toolButton_selete_datedir->setText(QApplication::translate("DayTotolTableClass", "\351\200\211\346\213\251\346\226\207\344\273\266\345\244\271", nullptr));
        toolButton->setText(QApplication::translate("DayTotolTableClass", "\347\273\237\350\256\241", nullptr));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DayTotolTableClass: public Ui_DayTotolTableClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DAYTOTOLTABLE_H
