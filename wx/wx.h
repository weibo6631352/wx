#pragma once

#include <QtWidgets/QWidget>
#include "ui_wx.h"
#include <QVector>
#include <QMap>
#include <QCloseEvent>
#include "QwxData.h"
#include "QGuiViewport.h"

class QGuiViewport;
// 主界面
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

	void addUser(QString user, QString session, QString date);
protected:
	void InitStyle();
	void UpdateTitle();

	// 重载qwidget事件
	void keyPressEvent(QKeyEvent *) override;
	virtual void paintEvent(QPaintEvent *event) override;
	void closeEvent(QCloseEvent *event) override;
	public slots:
void on_addUser(); //添加user

void on_datainput(); //添加数据

void on_showset();
void on_clearData();

void on_tabinputtextEdit();

void RevocationLogTableWidgetItem();
	
void on_loadChangDi();

private:
	Ui::wxClass ui;
	QGuiViewport *view_port_;
};
