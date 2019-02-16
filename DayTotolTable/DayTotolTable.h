#pragma once

#include <QtWidgets/QWidget>
#include "ui_DayTotolTable.h"
#include <set>
#include <QDayView.h>

class DayTotolTable : public QWidget
{
	Q_OBJECT

public:
	DayTotolTable(QWidget *parent = Q_NULLPTR);

	virtual void paintEvent(QPaintEvent *event) override;


    bool getUserSetting(QString file_path, QMap<QString, 
		QMap<QString, QMap<QString, DataInfo>>> &locals_info, 
		QMap<QString, QVector<QString>> &kaibao_result, 
		QMap<QString, QMap<QString, QMap<QString, double>>> &daili_session_profit,
		QMap<QString, double> &good_Profit,
		QString &date);
    void totol();
	public slots:
	void on_change_date();
	



private:
	Ui::DayTotolTableClass ui;

	// 场地 代理 场次
	QMap<QString, QMap<QString, QMap<QString, DataInfo>>> locals_info;
	QMap<QString, QVector<QString>> kaibao_result;
	//QMap<QString, double> daili_profit;
	QMap<QString, QMap<QString, QMap<QString, double>>> daili_session_profit;
	QMap<QString, double> good_Profit;
	QString date;
	QString dir_path;
	QString pre_date_last_session;
};
