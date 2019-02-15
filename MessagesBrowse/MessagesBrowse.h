#pragma once

#include <QtWidgets/QWidget>
#include "ui_MessagesBrowse.h"
#include <QPoint>
#include <QMenu>
#include <QAction>
#include <QTimer>

class QNetworkAccessManager;

/*
1,����Ⱥ�ĵ�ͨѶ¼�������п����ղ�����Ϣ
2,ɨ�������ǵ�ǰʱ�䣬��/2+��/2+��/2+ʱ/2,  ����С����
3,����ͨѶ¼���룺��΢���ļ����ַ��� ��������123***   �鿴���� Ҳ�Ƕ��ļ����ַ���
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
