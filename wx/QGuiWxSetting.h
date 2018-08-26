#pragma once

#include <QDialog>
#include "ui_QGuiWxSetting.h"

class QGuiWxSetting : public QDialog
{
	Q_OBJECT

public:
	QGuiWxSetting(QWidget *parent = Q_NULLPTR);
	~QGuiWxSetting();

	void LoadSetting();
public slots:
	void on_setalias_add();
	void on_setalias_del();
	void on_setalias_apply();

	void on_setreplace_add();
	void on_setreplace_del();
	void on_setreplace_apply();

	void on_setreplacespace_add();
	void on_setreplacespace_del();
	void on_setreplacespace_apply();

private:
	Ui::QGuiWxSetting ui;
};
