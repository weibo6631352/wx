#pragma once

#include <QtWidgets/QWidget>
#include "ui_jiqima.h"

class jiqima : public QWidget
{
	Q_OBJECT

public:
	jiqima(QWidget *parent = Q_NULLPTR);

private:
	Ui::jiqimaClass ui;
};
