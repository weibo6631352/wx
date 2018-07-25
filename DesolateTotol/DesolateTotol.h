#pragma once

#include <QtWidgets/QDialog>
#include "ui_DesolateTotol.h"
#include <QMap>
#include <QVector>

class DesolateTotol : public QDialog
{
	Q_OBJECT

public:
	DesolateTotol(QWidget *parent = Q_NULLPTR);
	
	void DataDispose();
	void DataInit();
public slots:
void setfilepath();
void on_export();
void UpdateDesolateView();

	
	

private:
	Ui::DesolateTotolClass ui;
	QMap<int, QMap<int, QMap<int, QVector<QVector<QString>>>>> totol_goods_;	//ƒÍ‘¬»’
	QMap<QString, int> desolate_nums_;
	QMap<QString, int> desolate_day_nums_;
};
