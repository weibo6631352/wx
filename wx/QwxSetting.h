#pragma once

#include <QObject>
#include <QMap>
#include <QSet>
#include <set>
#include <QVector>
struct strLenComp
{
	bool operator()(const QString& left, const QString& right) const  //���أ��������
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

	QVector<QString> gset_; //��Ʒ˳��
	QMap<QString, double> good_Profit_; //��Ʒϵ��
	QMap<QString, QSet<QString>> alias_; //��Ʒ����
	std::map<QString, QString, strLenComp> replace_; //�����滻
	std::set<QString, strLenComp> delete_; //ɾ���ֶ�
	QMap<QString, double> agencyProfit_; //����ϵ��
	QSet<QString> split_set_; //�ָ���
	std::set <QString, strLenComp> totolDict_; //�ϼ��ֵ�
	std::set<QString, strLenComp> gegeDict_; //�����ֵ�
	QString pre_goods_;	//��һ�����ı���
	QVector<QString> allGoodAliasSort_; //���е���Ʒ���������б�

	int yabao_limit_;
};
