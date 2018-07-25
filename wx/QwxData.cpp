#include "QwxData.h"
#include "QwxSetting.h"

QwxData::QwxData(QObject *parent)
	: QObject(parent)
{
}

QwxData* QwxData::ins()
{
	static QwxData *qwxData = new QwxData;
	return qwxData;
}

QwxData::~QwxData()
{
}

void QwxData::AddUserData(QString user, QString date, QString session)
{
	user_data_[user] = new QGoodsData(user, date, session);
}

QGoodsData* QwxData::GetUserData(QString user)
{
	return user_data_[user];
}

int QwxData::GetUserCount()
{
	return user_data_.size();
}

QGoodsData* QwxData::GetTotolData()
{
	QGoodsData *totol;
	int totol_num = 0;
	QwxSetting* wxsetting = QwxSetting::ins();
	totol = new QGoodsData(QStringLiteral("总计"), wxsetting->date_, wxsetting->session_);
	totol->totols_ = 0;


	if (user_data_.count())
	{
		for (auto it = user_data_.begin(); it != user_data_.end(); ++it)
		{
			QGoodsData* data = it.value();

			//复制数据
			for (auto itt = data->goods_totols_.begin(); itt != data->goods_totols_.end(); ++itt)
			{
				totol->goods_totols_[itt.key()] += itt.value();
			}
			totol_num += data->totols_;
		}
		totol->totols_ = totol_num;
	}


	return totol;
}

void QwxData::ClearUsersData()
{
	for (auto it = user_data_.begin(); it != user_data_.end(); ++it)
	{
		QGoodsData* data = it.value();
		data->ClaerUserData();
	}
}
