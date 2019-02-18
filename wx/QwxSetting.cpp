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

	//����
	f.setFileName(app_path + QStringLiteral("/����.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������
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
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("����.ini��ʧ��!"));
		return false;
	}
		

	//��������
	f.setFileName(app_path + QStringLiteral("/alias.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������

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
					QMessageBox::information(nullptr, QStringLiteral("����ʧ��"), QStringLiteral("û�����ֱ��ȱ�����˱���,�봦������ԣ�")+str_list[0], 0);
					f.close();
					return false;
				}	
			}
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("alias.ini��ʧ��!"));
		return false;
	}
		

	//�滻�ո�
	f.setFileName(app_path + QStringLiteral("/�滻�ո�.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������
		while (!txtInput.atEnd())
		{
			replace_space_.insert(txtInput.readLine());
		}

		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("�滻�ո�.ini��ʧ��!"));
		return false;
	}
		

	//�滻�ַ�
	f.setFileName(app_path + QStringLiteral("/replace.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������
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
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("replace.ini��ʧ��!"));
		return false;
	}
		

	//// ����ϵ��
	//f.setFileName(app_path + QStringLiteral("/agencyProfit.ini"));
	//if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	//{
	//	QTextStream txtInput(&f);
	//	txtInput.setCodec("UTF-8"); //��ע������
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
	//	QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("agencyProfit.ini��ʧ��!"));
	//	return false;
	//}

	// �ϼ��ֵ�
	f.setFileName(app_path + QStringLiteral("/�ϼ��ֵ�.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������

		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			if (lineStr == QStringLiteral(" "))
			{
				QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("�ϼ��ֵ�.ini�У��������Ҫ����Ϊ�ո�!"));
				return false;
			}
			totolDict_.insert(lineStr);
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("�ϼ��ֵ�.ini��ʧ��!"));
		return false;
	}

	// �����ֵ�
	f.setFileName(app_path + QStringLiteral("/�����ֵ�.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������

		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			if (lineStr == QStringLiteral(" "))
			{
				QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("�����ֵ�.ini�С��ָ�����Ҫ����Ϊ�ո�!"));
				return false;
			}
			gegeDict_.insert(lineStr);
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("�����ֵ�.ini��ʧ��!"));
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

// �������б�����ʵ������
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
// 	// ʵ������Ҳ��ӵ�good_sort_m
// 	for (int i = 0; i < goods_v.size(); ++i)
// 	{
// 		good_sort_m[goods_v[i].length()].insert(goods_v[i]);
// 	}
// 
// 	//��ʵ�����֡������б��˳����ӵ�goods_sort_v
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

	// ��ӱ���
	for (auto iter = alias_map.constBegin(); iter != alias_map.constEnd(); ++iter)
	{
		for (auto iterr = iter.value().constBegin(); iterr != iter.value().constEnd(); ++iterr)
		{
			goods_sort.insert(*iterr);
		}
	}

	// ʵ������Ҳ��ӵ�good_sort_m
	for (int i = 0; i < goods_v.size(); ++i)
	{
		goods_sort.insert(goods_v[i]);
	}

	//��ʵ�����֡������б��˳����ӵ�goods_sort_v
	for (auto it : goods_sort)
	{
		allGoodAliasSort_.push_back(it);
	}
}

// �������б�����ʵ������
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
