#pragma once

#include <QTabWidget>
#include "ui_QGuiTabWidgetInputData.h"
#include <QTextEdit>

class QGuiTabWidgetInputData : public QTabWidget
{
	Q_OBJECT

public:
	QGuiTabWidgetInputData(QWidget *parent = Q_NULLPTR);
	~QGuiTabWidgetInputData();

	int GetIndexFromTabText(QString tabName);
	QString GetTabTextFromIndex(int index);
	void AddTab(QString tabName);
	QString GetCurTabTitle();
	QTextEdit *GetCurTabWidget();

	void SetFocus();
private:
	Ui::QGuiTabWidgetInputData ui;
};

class QDoubleClickTextEdit : public QTextEdit
{
	Q_OBJECT

public:
	QDoubleClickTextEdit(QWidget *parent = Q_NULLPTR);
	~QDoubleClickTextEdit();

	virtual void focusInEvent(QFocusEvent *e);

	virtual void mouseDoubleClickEvent(QMouseEvent *e);

};