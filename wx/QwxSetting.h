#pragma once

#include <QObject>
#include <QMap>
#include <QSet>
#include <set>
#include <QVector>
struct strLenComp
{
	bool operator()(const QString& left, const QString& right) const  //重载（）运算符
	{
		if (left.length() == right.length())
		{
			return left > right;
		}
		else
		{
			return left.length() > right.length();
		}
	}

};
class QwxSetting : public QObject
{
	Q_OBJECT

public:
	
	~QwxSetting();
	static QwxSetting* ins();

	bool ReadIni(QString path = QString());

	const QMap<QString, QSet<QString>>& GetAliasMap();
	const std::map<QString, QString, strLenComp>& GetReplaceMap();
	const std::set<QString, strLenComp>& GetDeleteSet();
	const QMap<QString, double>& GetAgencyProfitMap();
	const QMap<QString, double>& GetGoodsProfitMap();
	const QSet<QString>& GetSplitSet();
	const QString GetPreGoods();
	const std::set <QString, strLenComp>& GetTotolDict();
	const std::set<QString, strLenComp>& GetGegeDict();

	void SetPreGoods(QString goods);
	void SetAliasMap(QMap<QString, QSet<QString>> &alias_map);
	void SetReplaceMap(const std::map<QString, QString, strLenComp> &replace_map);
	void SetDeleteSet(std::set<QString, strLenComp> &delete_set);

	
	QVector<QString> GetAllGoodsAliasStrsort();
	QString GetRealNameByAlias(QString &alias);
	const QVector<QString>& GetGoodsSet();

	QString title_;
	QString date_;
	QString session_;

protected:
	void SortAllGoodsAlias();
private:
	QwxSetting(QObject *parent = nullptr);

	QVector<QString> gset_; //物品顺序
	QMap<QString, double> good_Profit_; //物品系数
	QMap<QString, QSet<QString>> alias_; //物品别名
	std::map<QString, QString, strLenComp> replace_; //别名替换
	std::set<QString, strLenComp> delete_; //删除字段
	QMap<QString, double> agencyProfit_; //代理系数
	QSet<QString> split_set_; //分隔符
	std::set <QString, strLenComp> totolDict_; //合计字典
	std::set<QString, strLenComp> gegeDict_; //各个字典
	QString pre_goods_;	//上一场开的宝物
	QVector<QString> allGoodAliasSort_; //所有的物品别名排序列表

	int yabao_limit_;
};
