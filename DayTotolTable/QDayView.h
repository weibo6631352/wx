#pragma once

#include <QWidget>
#include "ui_QDayView.h"
#include <QTableWidget>

struct DataInfo
{
	QString date;
	QString	localc;
	QString	session;
	QString	daili;
	QString	filename;
	std::map<QString, int> data;
};

struct LocalInfoComp
{
	bool operator()(const DataInfo& left, const DataInfo& right) const  //÷ÿ‘ÿ£®£©‘ÀÀ„∑˚
	{
		if(left.localc!=right.localc)
			return left.localc < right.localc;
		else
		{
			if(left.daili != right.daili)
				return left.daili < right.daili;
			else
			{
				return left.session < right.session;
			}
		}
	}
};

class QDayView : public QWidget
{
	Q_OBJECT

public:
	QDayView(QWidget *parent = Q_NULLPTR);
	QTableWidget *tablewidget;
	virtual void paintEvent(QPaintEvent *event) override;
	~QDayView();

	QTableWidget* getTable();
private:
	Ui::QDayView ui;
	QTableWidget * table;
};
