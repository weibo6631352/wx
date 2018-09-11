/********************************************************************************
** Form generated from reading UI file 'QDayView.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QDAYVIEW_H
#define UI_QDAYVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QDayView
{
public:
    QGridLayout *gridLayout;
    QTableWidget *tableWidget_day_totol;

    void setupUi(QWidget *QDayView)
    {
        if (QDayView->objectName().isEmpty())
            QDayView->setObjectName(QStringLiteral("QDayView"));
        QDayView->resize(838, 319);
        gridLayout = new QGridLayout(QDayView);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(2, 2, 2, 2);
        tableWidget_day_totol = new QTableWidget(QDayView);
        if (tableWidget_day_totol->columnCount() < 7)
            tableWidget_day_totol->setColumnCount(7);
        if (tableWidget_day_totol->rowCount() < 8)
            tableWidget_day_totol->setRowCount(8);
        tableWidget_day_totol->setObjectName(QStringLiteral("tableWidget_day_totol"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableWidget_day_totol->sizePolicy().hasHeightForWidth());
        tableWidget_day_totol->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(11);
        font.setBold(true);
        font.setWeight(75);
        tableWidget_day_totol->setFont(font);
        tableWidget_day_totol->setFocusPolicy(Qt::NoFocus);
        tableWidget_day_totol->setFrameShape(QFrame::NoFrame);
        tableWidget_day_totol->setLineWidth(0);
        tableWidget_day_totol->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_day_totol->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableWidget_day_totol->setAutoScroll(false);
        tableWidget_day_totol->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_day_totol->setShowGrid(true);
        tableWidget_day_totol->setGridStyle(Qt::SolidLine);
        tableWidget_day_totol->setRowCount(8);
        tableWidget_day_totol->setColumnCount(7);
        tableWidget_day_totol->horizontalHeader()->setVisible(false);
        tableWidget_day_totol->horizontalHeader()->setCascadingSectionResizes(false);
        tableWidget_day_totol->horizontalHeader()->setDefaultSectionSize(100);
        tableWidget_day_totol->horizontalHeader()->setHighlightSections(false);
        tableWidget_day_totol->horizontalHeader()->setMinimumSectionSize(25);
        tableWidget_day_totol->horizontalHeader()->setStretchLastSection(false);
        tableWidget_day_totol->verticalHeader()->setVisible(false);
        tableWidget_day_totol->verticalHeader()->setCascadingSectionResizes(false);
        tableWidget_day_totol->verticalHeader()->setDefaultSectionSize(20);
        tableWidget_day_totol->verticalHeader()->setHighlightSections(false);
        tableWidget_day_totol->verticalHeader()->setStretchLastSection(false);

        gridLayout->addWidget(tableWidget_day_totol, 0, 0, 1, 1);


        retranslateUi(QDayView);

        QMetaObject::connectSlotsByName(QDayView);
    } // setupUi

    void retranslateUi(QWidget *QDayView)
    {
        QDayView->setWindowTitle(QApplication::translate("QDayView", "QDayView", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QDayView: public Ui_QDayView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QDAYVIEW_H
