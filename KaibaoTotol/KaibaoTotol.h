#pragma once

#include <QtWidgets/QDialog>
#include "ui_KaibaoTotol.h"

class KaibaoTotol : public QDialog
{
	Q_OBJECT

public:
	KaibaoTotol(QWidget *parent = Q_NULLPTR);
	void UpdateKaibaoView();
	int GetMouthDays();
	void InitYearMonth();
	void InitMonth();
	public slots:
	void setfilepath();

	void on_export();
	void on_year_change(QString);
	void on_month_change(QString);
	
private:
	Ui::KaibaoTotolClass ui;

	QMap<int, QMap<int, QMap<int, QVector<QString>>>> totol_goods_;	//ƒÍ‘¬»’
};
