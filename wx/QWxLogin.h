#pragma once

#include <QDialog>
#include "ui_QWxLogin.h"

class QWxLogin : public QDialog
{
	Q_OBJECT

public:
	QWxLogin(QWidget *parent = Q_NULLPTR);
	~QWxLogin();

	QString session_;
	QString date_;

public slots:
void accept();
private:
	Ui::QWxLogin ui;
};
