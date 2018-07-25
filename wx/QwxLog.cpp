#include "QwxLog.h"
#include <QTime>
#include <QMessageBox>

QwxLog::QwxLog(QObject *parent)
	: QObject(parent)
{
}

QwxLog::~QwxLog()
{
}

QwxLog* QwxLog::ins()
{
	static QwxLog* wxLog = new QwxLog;
	return wxLog;
}

// void QwxLog::NewLog(QString src, QString user)
// {
// 	// 新建一条log
// 	QVector<QString> log;
// 
// 	QString time_str = QTime::currentTime().toString(QStringLiteral("hh:mm:ss"));
// 
// 	log.push_back(time_str); //时间
// 	log.push_back(user);  // 代理名
// 	log.push_back(src);  // 原始数据
// 
// 	user_logs_[user].push_back(log);
// }

bool QwxLog::GetLastInputSucceed(QString user)
{
	return user_logs_[user][user_logs_[user].size() - 1].size() == 4;
}

QString QwxLog::GetOutPutEditString(QString user)
{
	QString frmatStr;
	if (user_logs_[user][user_logs_[user].size() - 1].size() == 4)
	{
		int npos = user_logs_[user][user_logs_[user].size() - 1][3].indexOf(QStringLiteral("合计"));
		if (-1!= npos)
		{
			QStringList logstr = user_logs_[user][user_logs_[user].size() - 1][3].split(QStringLiteral("合计"));
			QString html_totol_num = QStringLiteral(" <span style=' color:#ff0000;'>合计") + logstr[1] +QStringLiteral("</span>");
			frmatStr = user_logs_[user][user_logs_[user].size() - 1][0] + QStringLiteral("\t") + user + QStringLiteral("<br/>") + logstr[0] + html_totol_num + QStringLiteral("<br/>");

		}
		else
		{
			frmatStr = user_logs_[user][user_logs_[user].size() - 1][0] + QStringLiteral("\t") + user + QStringLiteral("\n") + user_logs_[user][user_logs_[user].size() - 1][3] + QStringLiteral("\n");

		}

	}
	return frmatStr;
}

void QwxLog::AppendLog(QString user, QString src, QString append)
{
	// 新建一条log
	QVector<QString> log;

	QString time_str = QTime::currentTime().toString(QStringLiteral("hh:mm:ss"));

	log.push_back(time_str);  // 时间
	log.push_back(user);  // 代理名
	log.push_back(src);  // 原始数据
	log.push_back(append);  // 录入数据

	user_logs_[user].push_back(log);
}

void QwxLog::SetErrStr(QString err_str)
{
	QMessageBox::information(nullptr, QStringLiteral(""), err_str, 0);
}

bool myCmpare(QVector<QString> p1, QVector<QString> p2)
{
	return p1[0] < p2[1];
}

QVector<QVector<QString>> QwxLog::GetTotolLog()
{
	QVector<QVector<QString>> totol;
	for (auto it = user_logs_.begin(); it != user_logs_.end(); ++it)
	{
		totol.append(it.value());
	}

	qSort(totol.begin(), totol.end(), myCmpare);
	return totol;
}

QVector<QVector<QString>> & QwxLog::GetUserLog(QString user)
{
	return user_logs_[user];
}

QVector<QString> QwxLog::GetLastLog(QString user)
{
	return user_logs_[user][user_logs_[user].size() - 1];
}

