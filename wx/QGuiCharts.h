#pragma once

#include <QWidget>
#include "ui_QGuiCharts.h"

// 用户图表类
class QGuiCharts : public QWidget
{
	Q_OBJECT

public:
	QGuiCharts(QString user, QWidget *parent = Q_NULLPTR);
	~QGuiCharts();
	void InitStyle();
	void UpdateCharts();
	void on_heightlight_Goods(QString goods);
	
protected:
	virtual void resizeEvent(QResizeEvent *event);

private:
	Ui::QGuiCharts ui;
	QString user_;
};
