#pragma once

#include <QtWidgets/QWidget>
#include "ui_MessagesBrowse.h"
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QTimer>

class QNetworkAccessManager;

/*
1,保存群聊到通讯录，否则有可能收不到消息
2,扫码密码是当前时间，年/2+月/2+日/2+时/2,  忽略小数点
3,设置通讯录密码：对微信文件助手发送 设置密码123***   查看密码 也是对文件助手发送
*/
class MessagesBrowse : public QWidget
{
	Q_OBJECT

public:
	MessagesBrowse(QWidget *parent = Q_NULLPTR);

protected:
	void initUi();


	bool verify_supper_passwd();
	bool verify_tongxunlu_passwd();
	QString fromatChatContent(QString &chats);
	public slots:
	void QR_Login();
	void GetChatrooms();
	void TreeWidgetClicked(QTreeWidgetItem *item, int column);
	void TreeWidgetDoubleClicked(QTreeWidgetItem *item, int column);
	void TreeWidgetContextMenuRequested(const QPoint &pos);
	void UpdateCurrentChatroom();
	void UpdateChatContent();
	void AutoReFlush(bool checked);
	void ReFlush();
private:
	Ui::MessagesBrowseClass ui;
	QMenu *pop_menu_;
	QAction *update_chatroom_action_;
	QTimer *auto_reflush_timer_;
};
