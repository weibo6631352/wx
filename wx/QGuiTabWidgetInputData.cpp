#include "QGuiTabWidgetInputData.h"

QGuiTabWidgetInputData::QGuiTabWidgetInputData(QWidget *parent)
	: QTabWidget(parent)
{
	ui.setupUi(this);
	tabText(-1);
}

QGuiTabWidgetInputData::~QGuiTabWidgetInputData()
{
}

int QGuiTabWidgetInputData::GetIndexFromTabText(QString tabName)
{
	int tab_count = count();
	for (int i = 0; i < tab_count; ++i)
	{
		if (tabText(i) == tabName)
		{
			return i ;
		}
	}
	return -1;
}

QString QGuiTabWidgetInputData::GetTabTextFromIndex(int index)
{
	return tabText(index);
}

void QGuiTabWidgetInputData::AddTab(QString tabName)
{
	int tab_count = count();



	QDoubleClickTextEdit *textEdit = new QDoubleClickTextEdit;
	insertTab(tab_count, textEdit, tabName);
}

QTextEdit * QGuiTabWidgetInputData::GetCurTabWidget()
{
	return (QTextEdit*)currentWidget();
}

void QGuiTabWidgetInputData::SetFocus()
{
	GetCurTabWidget()->setFocus();
}

QString QGuiTabWidgetInputData::GetCurTabTitle()
{
	int index = currentIndex();
	return tabText(index);
}

QDoubleClickTextEdit::QDoubleClickTextEdit(QWidget *parent /*= Q_NULLPTR*/)
	:QTextEdit(parent)
{

}

QDoubleClickTextEdit::~QDoubleClickTextEdit()
{

}

void QDoubleClickTextEdit::focusInEvent(QFocusEvent *e)
{
	QTextEdit::focusInEvent(e);

	selectAll();
}

void QDoubleClickTextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
	selectAll();
}
