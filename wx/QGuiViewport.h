#pragma once

#include <QDialog>
#include "ui_QGuiViewport.h"

// Õº±ÌΩÁ√Ê
class QGuiViewport : public QDialog
{
	Q_OBJECT

public:
	QGuiViewport(QWidget *parent = Q_NULLPTR);
	~QGuiViewport();

	void InitUi();
	bool CreateDir(QString fullPath);
	void SetCurrentIndex(int index);
	QString GetPreGoods();
	void HeightLightGoods(QString goods);
	int GetTabIndex(QString title);

	void on_exportTotolPic(QString path, QString user);
	void on_exportTotolTxt(QString path, QString user);
	void on_exportLog(QString path, QString user);
	

	public slots:
	void on_export();
	void on_serach_change(const QString& goods);
	void on_updateCharts();
	void on_preGoods_change(const QString& goods);
	void on_rendpic();

public:
	Ui::QGuiViewport ui;
};
