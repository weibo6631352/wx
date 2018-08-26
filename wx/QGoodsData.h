#pragma once

#include <QObject>
#include  <QMap>

//用户数据类
class QGoodsData : public QObject
{
	Q_OBJECT

public:
	QGoodsData(QString user, QString date, QString session, QObject *parent = nullptr);
	bool InputData(QString input, QString &err_label);
	void RevocationData(QString src_str, QString input);
	QString ExtractNum(QString &str);
	QString ExtracChinese(QString &str);
	int GetTotol();
	void InitGoodTotol();
	~QGoodsData();

	void InitUserData();
	void ClaerUserData();

	QString SubNumber(QString str);
	QMap<QString, int> goods_totols_;
	int totols_;
	QString user_;
	QString date_;
	QString session_;
	double profit_;
private:
	
};
