#include "QwxSetting.h"
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QTextStream>
#include <iostream>
#include <exception>

QwxSetting::QwxSetting(QObject *parent)
	: QObject(parent),
	yabao_limit_(0)
{
}

QwxSetting::~QwxSetting()
{
}

QwxSetting* QwxSetting::ins()
{
	static QwxSetting* g_goodsData = new QwxSetting();
	return g_goodsData;
}

bool QwxSetting::ReadIni(QString path /*= QString()*/)
{
	QString app_path = QApplication::applicationDirPath();
	QFile f;
	QString lineStr;
	int rowCount = 0;

	//宝物
	f.setFileName(app_path + QStringLiteral("/宝物.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			QStringList str_list = lineStr.split(QStringLiteral("="));
			if (str_list.size() == 2)
			{
				gset_.push_back(str_list[0]);
				good_Profit_[str_list[0]] = str_list[1].toDouble();
			}
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("宝物.ini打开失败!"));
		return false;
	}
		

	//别名设置
	f.setFileName(app_path + QStringLiteral("/alias.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行

		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();

			QSet<QString> split_set;
			QStringList str_list = lineStr.split(QStringLiteral("="));
			if (str_list.size() == 2)
			{

				QStringList splitList = str_list[1].split(QStringLiteral(","));
				for (auto i : splitList)
				{
					split_set.insert(i);
				}
				if (gset_.indexOf(str_list[0]) != -1)
					alias_[str_list[0]] = split_set;
				else
				{
					QMessageBox::information(nullptr, QStringLiteral("启动失败"), QStringLiteral("没有这种宝物，缺设置了别名,请处理后再试！")+str_list[0], 0);
					f.close();
					return false;
				}	
			}
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("alias.ini打开失败!"));
		return false;
	}
		

	//替换空格
	f.setFileName(app_path + QStringLiteral("/替换空格.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		while (!txtInput.atEnd())
		{
			replace_space_.insert(txtInput.readLine());
		}

		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("替换空格.ini打开失败!"));
		return false;
	}
		

	//替换字符
	f.setFileName(app_path + QStringLiteral("/replace.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			QStringList str_list = lineStr.split(QStringLiteral("="));
			if (str_list.size() == 2)
			{
				replace_[str_list[0]] = str_list[1];
			}
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("replace.ini打开失败!"));
		return false;
	}
		

	//// 代理系数
	//f.setFileName(app_path + QStringLiteral("/agencyProfit.ini"));
	//if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	//{
	//	QTextStream txtInput(&f);
	//	txtInput.setCodec("UTF-8"); //请注意这行
	//
	//	while (!txtInput.atEnd())
	//	{
	//		lineStr = txtInput.readLine();
	//
	//		QSet<QString> split_set;
	//		QStringList str_list = lineStr.split(QStringLiteral("="));
	//		if (str_list.size() == 2)
	//		{
	//			agencyProfit_[str_list[0]] = str_list[1].toDouble();
	//		}
	//	}
	//	f.close();
	//}
	//else
	//{
	//	QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("agencyProfit.ini打开失败!"));
	//	return false;
	//}

	// 合计字典
	f.setFileName(app_path + QStringLiteral("/合计字典.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行

		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			if (lineStr == QStringLiteral(" "))
			{
				QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("合计字典.ini中，间隔符不要设置为空格!"));
				return false;
			}
			totolDict_.insert(lineStr);
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("合计字典.ini打开失败!"));
		return false;
	}

	// 各个字典
	f.setFileName(app_path + QStringLiteral("/各个字典.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行

		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			if (lineStr == QStringLiteral(" "))
			{
				QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("各个字典.ini中。分隔符不要设置为空格!"));
				return false;
			}
			gegeDict_.insert(lineStr);
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("各个字典.ini打开失败!"));
		return false;
	}
		
	SortAllGoodsAlias();

	return true;
}

const QMap<QString, QSet<QString>>& QwxSetting::GetAliasMap()
{
	return alias_;
}

const std::map<QString, QString, strLenComp>& QwxSetting::GetReplaceMap()
{
	return replace_;
}

const std::set<QString, strLenComp>& QwxSetting::GetReplaceSpaceSet()
{
	return replace_space_;
}

const QMap<QString, double>& QwxSetting::GetAgencyProfitMap()
{
	return agencyProfit_;
}

void QwxSetting::SetAgencyProfitMap(QMap<QString, double>& agencyProfit)
{
	agencyProfit_ = agencyProfit;
}

void QwxSetting::SetAgencyProfit(QString user, double profit)
{
	agencyProfit_[user] = profit;
}

const QMap<QString, double>& QwxSetting::GetGoodsProfitMap()
{
	return good_Profit_;
}

const QString QwxSetting::GetPreGoods()
{
	return pre_goods_;
}

const std::set <QString, strLenComp>& QwxSetting::GetTotolDict()
{
	return totolDict_;
}

const std::set<QString, strLenComp>& QwxSetting::GetGegeDict()
{
	return gegeDict_;
}

void QwxSetting::SetPreGoods(QString goods)
{
	pre_goods_ = goods;
}

void QwxSetting::SetAliasMap(QMap<QString, QSet<QString>> &alias_map)
{
	alias_ = alias_map;
}

void QwxSetting::SetReplaceMap(const std::map<QString, QString, strLenComp> &replace_map)
{
	replace_ = replace_map;
}

void QwxSetting::SetReplaceSpaceSet(std::set<QString, strLenComp> &delete_set)
{
	replace_space_ = delete_set;
}

// 返回所有别名和实际名字
// QVector<QString> QwxSetting::GetAllGoodsAliasStrsort()
// {
// 	const QMap<QString, QSet<QString>>&alias_map = GetAliasMap();
// 	const QVector<QString>&goods_v = GetGoodsSet();
// 
// 	// 
// 	QVector<QString> goods_sort_v;
// 	QMap<int, QSet<QString>> good_sort_m;
// 	for (auto iter = alias_map.constBegin(); iter != alias_map.constEnd(); ++iter)
// 	{
// 		QString goods = iter.key();
// 		for (auto iterr = iter.value().constBegin(); iterr != iter.value().constEnd(); ++iterr)
// 		{
// 			QString alias = *iterr;
// 			good_sort_m[alias.length()].insert(alias);
// 		}
// 	}
// 
// 	// 实际名字也添加到good_sort_m
// 	for (int i = 0; i < goods_v.size(); ++i)
// 	{
// 		good_sort_m[goods_v[i].length()].insert(goods_v[i]);
// 	}
// 
// 	//以实际名字、别名列表的顺序添加到goods_sort_v
// 	for (auto iter = good_sort_m.constBegin(); iter != good_sort_m.constEnd(); ++iter)
// 	{
// 		for (auto iterr = iter.value().constBegin(); iterr != iter.value().constEnd(); ++iterr)
// 		{
// 			goods_sort_v.push_back(*iterr);
// 		}
// 	}
// 	return goods_sort_v;
// }

void QwxSetting::SortAllGoodsAlias()
{
	const QMap<QString, QSet<QString>>&alias_map = GetAliasMap();
	const QVector<QString>&goods_v = GetGoodsSet();


	std::set<QString, strLenComp> goods_sort;

	// 添加别名
	for (auto iter = alias_map.constBegin(); iter != alias_map.constEnd(); ++iter)
	{
		for (auto iterr = iter.value().constBegin(); iterr != iter.value().constEnd(); ++iterr)
		{
			goods_sort.insert(*iterr);
		}
	}

	// 实际名字也添加到good_sort_m
	for (int i = 0; i < goods_v.size(); ++i)
	{
		goods_sort.insert(goods_v[i]);
	}

	//以实际名字、别名列表的顺序添加到goods_sort_v
	for (auto it : goods_sort)
	{
		allGoodAliasSort_.push_back(it);
	}
}

// 返回所有别名和实际名字
QVector<QString> QwxSetting::GetAllGoodsAliasStrsort()
{
	return allGoodAliasSort_;
}

QString QwxSetting::GetRealNameByAlias(QString &alias)
{
	const QMap<QString, QSet<QString>>& alias_map = GetAliasMap();
	for (auto iter = alias_map.constBegin(); iter != alias_map.constEnd(); ++iter)
	{
		if (iter.value().contains(alias))
			return iter.key();
	}
	return QString();
}

const QVector<QString>& QwxSetting::GetGoodsSet()
{
	return gset_;
}
