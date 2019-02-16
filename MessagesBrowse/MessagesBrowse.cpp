#include "MessagesBrowse.h"
#include <QDoubleValidator>
#include <QIntValidator>
#include "HttpInterface.h"
#include <QDateTime>
#include "Login.h"
#include <QTimer>
#include <memory>
#include <QMessageBox>
#include "mbdef.h"

//QString ss = QString::fromUtf8(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/WXQueryDb?sql=select * from groupmsg")));
//QMessageBox::information(this, ss, ss, 0);
MessagesBrowse::MessagesBrowse(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	

	initUi();
	
}

void MessagesBrowse::initUi()
{
	ui.treeWidget->setColumnCount(1);
	ui.treeWidget->setHeaderHidden(true);
	ui.lineEdit_pretime->setValidator(new QDoubleValidator(0.0, 120.0, 2, this));
	ui.lineEdit_autorefresh->setValidator(new QIntValidator(5, 1000, this));

	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, &MessagesBrowse::TreeWidgetClicked);
	connect(ui.treeWidget, &QTreeWidget::itemDoubleClicked, this, &MessagesBrowse::TreeWidgetDoubleClicked);
	connect(ui.treeWidget, &QTreeWidget::customContextMenuRequested, this, &MessagesBrowse::TreeWidgetContextMenuRequested);

	pop_menu_ = new QMenu(ui.treeWidget);
	update_chatroom_action_ = new QAction(QStringLiteral("更新"), pop_menu_);
	pop_menu_->addAction(update_chatroom_action_);
	connect(update_chatroom_action_, SIGNAL(triggered()), this, SLOT(UpdateCurrentChatroom()));
	
	auto_reflush_timer_ = new QTimer(this);
	connect(auto_reflush_timer_, SIGNAL(timeout()), this, SLOT(UpdateChatContent()));
}

bool MessagesBrowse::verify_supper_passwd()
{
	QString password = ui.lineEdit_qrpassword->text();
	if (password.length() != 10)
		return false;
	QDateTime current_time = QDateTime::currentDateTime();
	if (password.mid(0, 4).toInt() != current_time.date().year() / 2)
		return false;
	if (password.mid(4, 2).toInt() != current_time.date().month() / 2)
		return false;
	if (password.mid(6, 2).toInt() != current_time.date().day() / 2)
		return false;
	if (password.mid(8, 2).toInt() != current_time.time().hour() / 2)
		return false;
	return true;
}

bool MessagesBrowse::verify_tongxunlu_passwd()
{
	QString password = ui.lineEdit_tongxunlupassword->text();
	QString server_passwd = HttpInterface::Bytes2String(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/GetVerification")));
	return password == server_passwd;
}

QString MessagesBrowse::fromatChatContent(QString &chats)
{
	QString res;

	QStringList lines = chats.split(QStringLiteral("%%%\n"));
	for (int i = lines.size() -1; i >=0; --i)
	{
		QString line = lines[i];
		if (line.isEmpty())
			continue;
		QStringList field_list = line.split(QStringLiteral("\t$$$"));
		QString head_str = QStringLiteral("<font color=blue>") + field_list[2] + QDateTime::fromTime_t(field_list[4].toInt()).time().toString("hh:mm:ss")
			+ QStringLiteral("</font><br>");
		QString body_str = field_list[3].replace(QStringLiteral("\n"), QStringLiteral("<br>")) + QStringLiteral("<br>");

		res += (head_str + body_str);
	}
	return res;
}

void MessagesBrowse::QR_Login()
{
	if (!verify_supper_passwd())
		return;
	Login lg(this);
	lg.setWindowTitle(QStringLiteral("登录"));
	lg.exec();

	ui.label_self->setText(lg.nickname_);
}

void MessagesBrowse::GetChatrooms()
{
	if (!verify_supper_passwd() && !verify_tongxunlu_passwd())
		return;

	QStringList chatrooms = HttpInterface::Bytes2String(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/GetUserGroups"))).split('\n');
	if (!chatrooms.size())
	{
		QMessageBox::information(this, QStringLiteral("失败"), QStringLiteral("获取通讯录失败"), 0);
		return;
	}
	

	ui.treeWidget->clear();
	for (int i = 0; i < chatrooms.size(); ++i)
	{
		if(chatrooms[i].isEmpty())
			continue;
		QStringList line = chatrooms[i].split(QStringLiteral("\t$$$"));

		//QMessageBox::information(this, chatrooms[i], chatrooms[i], 0);
		//QMessageBox::information(this, line[0], line[0], 0);
		//QMessageBox::information(this, line[1], line[1], 0);
		QString chatroomname = line[0];
		QString chatroomid = line[1];
		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		pItem->setText(0, chatroomname); 
		pItem->setData(0, Qt::ToolTipRole, chatroomid);
		pItem->setExpanded(false);
		ui.treeWidget->addTopLevelItem(pItem);
	}
}

void MessagesBrowse::TreeWidgetClicked(QTreeWidgetItem *item, int column)
{
	UpdateChatContent();
}

void MessagesBrowse::TreeWidgetDoubleClicked(QTreeWidgetItem *item, int column)
{
	//item->setExpanded(!item->isExpanded());
	if (item->childCount() == 0 && 0 == item->parent())
	{
		QString chatroom_id = item->data(0, Qt::ToolTipRole).toString();
		QString url_head = QStringLiteral("http://45.78.75.226:5000/GetActiveMember?chatroom=");
		QStringList friends = HttpInterface::Bytes2String(HttpInterface::HttpGet(url_head + chatroom_id)).split('\n');
		for (int i = 0; i < friends.size(); ++i)
		{
			if (friends[i].isEmpty())
				continue;
			
			QStringList py = friends[i].split(QStringLiteral("\t$$$"));
			

			QString name = py[0];
			QString id = py[1];
			QString flag = py[2];

			QTreeWidgetItem* pItem = new QTreeWidgetItem();
			pItem->setText(0, name);
			pItem->setData(0, Qt::ToolTipRole, id);

			if (flag == QStringLiteral("0"))
				pItem->setForeground(0, QBrush(Qt::red));

			item->addChild(pItem);
		}
	}
}

void MessagesBrowse::TreeWidgetContextMenuRequested(const QPoint &pos)
{
	pop_menu_->exec(QCursor::pos());
}

void MessagesBrowse::UpdateCurrentChatroom()
{
	QTreeWidgetItem *item = ui.treeWidget->currentItem();
	if (item->parent())
		return;

	int child_count = item->childCount();
	for (int i = 0; i < child_count; ++i)
	{
		item->removeChild(item->child(i));
	}


	QString chatroom_id = ui.treeWidget->currentItem()->data(0, Qt::ToolTipRole).toString();
	QString url_head = QStringLiteral("http://45.78.75.226:5000/GetActiveMember?chatroom=");
	QStringList friends = HttpInterface::Bytes2String(HttpInterface::HttpGet(url_head + chatroom_id)).split('\n');
	for (int i = 0; i < friends.size(); ++i)
	{
		if (friends[i].isEmpty())
			continue;

		QStringList py = friends[i].split(QStringLiteral("\t$$$"));


		QString name = py[0];
		QString id = py[1];
		QString flag = py[2];

		QTreeWidgetItem* pItem = new QTreeWidgetItem();
		pItem->setText(0, name);
		pItem->setData(0, Qt::ToolTipRole, id);

		if (flag == QStringLiteral("0"))
			pItem->setForeground(0, QBrush(Qt::red));

		item->addChild(pItem);
	}
}

void MessagesBrowse::UpdateChatContent()
{
	QTreeWidgetItem *item = ui.treeWidget->currentItem();

	QString chatroomid, userid, chatroomname, username;
	if (item->parent())
	{
		chatroomname = item->parent()->text(0);
		username = item->text(0);
		chatroomid = item->parent()->data(0, Qt::ToolTipRole).toString();
		userid = item->data(0, Qt::ToolTipRole).toString();
	}
	else
	{
		chatroomname = item->text(0);
		chatroomid = item->data(0, Qt::ToolTipRole).toString();
	}

	int preseconds = ui.lineEdit_pretime->text().toDouble()*3600;
	int time_flag = QDateTime::currentDateTime().toTime_t() - preseconds;
	QString sql;
	if (userid.isEmpty())
	{
		sql = QStringLiteral("select * from groupmsg where time > ") + QString::number(time_flag) + QStringLiteral(" and chatroomid =\'") + chatroomid + QStringLiteral("\'");
	}
	else
	{
		sql = QStringLiteral("select * from groupmsg where time > ") + QString::number(time_flag) + QStringLiteral(" and chatroomid =\'") + chatroomid
			+ QStringLiteral("\' and userid = \'") + userid + QStringLiteral("\'");
	}

	QString url_head = QStringLiteral("http://45.78.75.226:5000/WXQueryDb?sql=");
	QString res = HttpInterface::Bytes2String(HttpInterface::HttpGet(url_head + sql));
	if (res.isEmpty())
	{
		ui.textEdit->setText(QStringLiteral("查询结果为空"));
	}
	else
	{
		ui.textEdit->setHtml(fromatChatContent(res));
	}

	ui.label_group->setText(chatroomname);
	ui.label_friend->setText(username);
}

void MessagesBrowse::AutoReFlush(bool checked)
{
	if (checked)
		auto_reflush_timer_->start(ui.lineEdit_autorefresh->text().toInt() * 1000);
	else
		auto_reflush_timer_->stop();
}

void MessagesBrowse::ReFlush()
{
	UpdateChatContent();
}
