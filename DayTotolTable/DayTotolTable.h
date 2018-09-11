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


	bool getUserSetting(QString file_path, QMap<QString, QMap<QString, QMap<QString, DataInfo>>> &locals_info, QMap<QString, QVector<QString>> &kaibao_result, QMap<QString, double> &daili_profit, QMap<QString, double> &good_Profit, QString &date);
	public slots:
	void on_change_date();
private:
	Ui::DayTotolTableClass ui;
};
