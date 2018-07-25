#pragma once
#include <QString>
#include <QVector>

class QSetting 
{

public:
	static QSetting &ins();
	QSetting();
	~QSetting();
	QVector<QString> gset_;
	QVector<QString> gnoset_;
};
