#pragma once

#include <QDialog>
#include "ui_Login.h"

class Login : public QDialog
{
	Q_OBJECT

public:
	Login(QWidget *parent = Q_NULLPTR);
	~Login();

	QString nickname_;
public slots:
void Verify();

private:
	Ui::Login ui;
};
