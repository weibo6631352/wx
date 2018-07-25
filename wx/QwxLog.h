#pragma once

#include <QObject>
#include <QVector>
#include <QMap>

class QwxLog : public QObject
{
	Q_OBJECT
	~QwxLog();
public:
	static QwxLog* ins();

	bool GetLastInputSucceed(QString user);
	QString GetOutPutEditString(QString user);
	void AppendLog(QString user, QString src, QString append);

	void SetErrStr(QString err_str);

	QVector<QVector<QString>>GetTotolLog();
	QVector<QVector<QString>> &GetUserLog(QString user);
	QVector<QString> GetLastLog(QString user);

	QMap<QString, QVector<QVector<QString>>> user_logs_;
	private:
	QwxLog(QObject *parent = nullptr);
};
