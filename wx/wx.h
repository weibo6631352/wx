#pragma once

#include <QtWidgets/QWidget>
#include "ui_wx.h"
#include <QVector>
#include <QMap>
#include <QCloseEvent>
#include "QwxData.h"
#include "QGuiViewport.h"

class QGuiViewport;
// ������
class wx : public QDialog
{
	Q_OBJECT

public:
	wx(QWidget *parent = Q_NULLPTR);
	~wx();

	void UpdateOutPutEdit();
	void RevocationUpdateOutPutEdit(QString user);
	void CleanInputTextEdit();
	void UpdateLogTableWidget();
	void UpdateDailiJishu();

protected:
	void InitStyle();
	void InitUi();

	// ����qwidget�¼�
	virtual void paintEvent(QPaintEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	public slots:
void on_addUser(); //����user

void on_datainput(); //��������

void on_showset();
void on_clearData();

void on_tabinputtextEdit();

void RevocationLogTableWidgetItem();
	

private:
	Ui::wxClass ui;
	QGuiViewport *view_port_;
};