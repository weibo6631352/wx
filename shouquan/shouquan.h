#pragma once

#include <QtWidgets/QWidget>
#include "ui_shouquan.h"

class shouquan : public QWidget
{
	Q_OBJECT

public:
	shouquan(QWidget *parent = Q_NULLPTR);
public slots:
	void onclick();
private:
	Ui::shouquanClass ui;
};
