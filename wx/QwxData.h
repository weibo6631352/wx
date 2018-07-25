#pragma once

#include <QObject>
#include "QGoodsData.h"

class QwxData : public QObject
{
	Q_OBJECT

public:
	static QwxData*ins();
	~QwxData();
	void AddUserData(QString user, QString date, QString session);
	QGoodsData* GetUserData(QString user);
	int GetUserCount();
	QGoodsData* GetTotolData();
	QMap<QString, QGoodsData*> user_data_;
	void ClearUsersData();

private:
	QwxData(QObject *parent = nullptr);

};
