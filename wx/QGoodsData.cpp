#include "QGoodsData.h"
#include "QwxSetting.h"
#include "QwxLog.h"
#include <QTime>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QGoodsData::QGoodsData(QString user, QString date, QString session, QObject *parent)
	: QObject(parent),
	user_(user),
	date_(date),
	session_(session),
	totols_(0),
	profit_(0)
{
	InitGoodTotol();
	InitUserData();
}

QGoodsData::~QGoodsData()
{
}

void QGoodsData::InitUserData()
{
	QMap<QString, double> agencyProfit = QwxSetting::ins()->GetAgencyProfitMap();
	auto it = agencyProfit.find(user_);
	if (it != agencyProfit.end())
	{
		profit_ = it.value();
	}
	else
	{
		profit_ = agencyProfit[QStringLiteral("Ĭ��")]; 
	}
}

void QGoodsData::ClaerUserData()
{
	InitGoodTotol();
	totols_ = 0;
}

bool checkError(QString line, QwxLog *wxLogs)
{
	/**
	*	Check 1
	*��鶺�ź���������
	*/
	QString temp_line = line;
	temp_line.remove(" ");

	QSet<QString> splits = QwxSetting::ins()->GetSplitSet();
	for (auto split_item : splits)
	{
		if (!split_item.isEmpty() && temp_line.contains(split_item))
		{
			QStringList split_str = temp_line.split(split_item);
			for (int j = 0; j < split_str.size(); ++j)
			{
				QString block = split_str[j];
				if (!block.isEmpty() )
				{
					if (block.at(0).isDigit())
					{
						wxLogs->SetErrStr(QStringLiteral("���ָ����ֶκ��������ֿ�ͷ!��") + block);
						return false;
					}
				}
			}
		}
	}


	// 2������Ƿ������ֿո�+���ֵĸ�ʽ
	QRegExp rx(QStringLiteral("[0-9]+\\s+[0-9]+"));//
	if (rx.indexIn(line,0) != -1)
	{
		wxLogs->SetErrStr(QStringLiteral("��⵽����+�ո�+���ֵ��������ֹ¼�룡") + line);
		return false;
	}

	return true;
}

bool QGoodsData::InputData(QString input)
{
	QwxSetting *wxSetting = QwxSetting::ins();
	QwxLog *wxLogs = QwxLog::ins();
	const QMap<QString, QSet<QString>>& alias_map = wxSetting->GetAliasMap();
	const std::map<QString, QString, strLenComp> &replace_map = wxSetting->GetReplaceMap();
	const std::set<QString, strLenComp> &remove_set = wxSetting->GetDeleteSet();
	const QSet<QString> &split_set = wxSetting->GetSplitSet();
	const std::set<QString, strLenComp> &totolDict = wxSetting->GetTotolDict();
	const std::set<QString, strLenComp> &gegeDict = wxSetting->GetGegeDict();


	//�ָ���  
	QStringList split_list = input.split('\n');
	QString tip_str;
	
	bool hasfuhao = false;
	for (auto it = split_list.begin(); it!=split_list.end(); )
	{
		QString s = *it;
		if (s.remove(' ').isEmpty())
			split_list.erase(it);
		else
			++it;
	}
	if (split_list.size() > 1)
	{
		for (int i = 0; i < split_list.size(); ++i)
		{
			if (split_list[i].indexOf('-') > -1 || split_list[i].indexOf(QStringLiteral("��")) > -1)
			{
				wxLogs->SetErrStr(QStringLiteral("¼���������ʱ���ܼ��м�������!��") + split_list[i]);
				return false;
			}
		}
	}


	//����ÿһ��
	int totol = 0;
	QMap<QString, int> goods_totol;
	QVector<QVector<QPair<QString, QString>>> output_v;
	for (auto iter : split_list)
	{
		QVector<QPair<QString, QString>> line_output;

		//ÿ����Ʒ

		QString line_str = iter;


		//������ڸ�ʽ   �����Ĵ��������Ƿ���ȷ
		QRegExp rx(QStringLiteral("(0?[0-9]|1[0-9]|2[0-3]):(0[0-9]|[12345][0-9])"));
		QString str = line_str;
		int poss = 0;
		if ((poss = rx.indexIn(str, poss)) != -1)
		{
			if (user_ == str.replace(rx.cap(0), QStringLiteral("")))
			{
				wxLogs->SetErrStr(QStringLiteral("�Ѻ��ԣ���ȷ������") + line_str);
				continue;
			}
			else
			{
				wxLogs->SetErrStr(QStringLiteral("��ƥ�䵽ʱ�䣬���Ǵ��������񲻶ԣ�����ֹ¼��!��") + line_str);
				return false;
			}
		}


		if (checkError(line_str, wxLogs) == false)
			return false;

		//�滻�ַ�
		for (auto iter = replace_map.cbegin(); iter != replace_map.cend(); ++iter)
		{
			line_str.replace(iter->first, iter->second);
		}
		
		line_str.replace(QStringLiteral("="), QStringLiteral("�ϼ�"));
		line_str.replace(QStringLiteral("��"), QStringLiteral("�ϼ�"));
		if (false == checkError(line_str, wxLogs))
			return false;

		//ɾ���ַ�
 		//line_str.replace(QStringLiteral(" "), QStringLiteral(""));
 		line_str.replace(QStringLiteral("\t"), QStringLiteral(" "));
		for (auto iter = split_set.begin(); iter != split_set.end(); ++iter)
		{
			line_str.replace(*iter, QStringLiteral(" "));
		}
		for (auto iter = remove_set.begin(); iter != remove_set.end(); ++iter)
		{
			line_str.replace(*iter, QStringLiteral(" "));
		}

		line_str.replace(QStringLiteral("--"), QStringLiteral(""));
		line_str.replace(QStringLiteral("--"), QStringLiteral(""));

		QString residue = line_str;


		// �Ƿ��и���
		QString gegestr;
		for (auto gege : gegeDict)
		{
			if (line_str.contains(gege))
			{
				gegestr = gege;
				break;
			}
		}

		// �Ƿ��и���
		if (!gegestr.isEmpty())
		{
			QString left;
			QString right;
			if (line_str.contains(gegestr))
			{
				QStringList list = line_str.split(gegestr);
				if (list.size() != 2)
				{
					wxLogs->SetErrStr(QStringLiteral("����ʶ��") + line_str);
					return false;
				}
				left = list[0];
				right = list[1];
			}

			QSet<QString> good_set;

			if (left.isEmpty() || ExtractNum(right).isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("����ʶ��") + line_str);
				return false;
			}

			QVector<QString> goods_sort_v = wxSetting->GetAllGoodsAliasStrsort();

			// �ж����б����б���
			for (int i = 0; i < goods_sort_v.size(); ++i)
			{
				//���������м��������ı���
				QRegExp rx(goods_sort_v[i]);
				int pos = 0;
				if ((pos = rx.indexIn(left, pos)) != -1) {
					QString name;
					if (wxSetting->GetGoodsSet().contains(goods_sort_v[i]))		//ʵ��
					{
						name = goods_sort_v[i];
					}
					else//����
					{
						name = wxSetting->GetRealNameByAlias(goods_sort_v[i]);
					}

 					// �Ƿ��Ѿ������ֱ�����
 					if (good_set.contains(name))
 					{
 						wxLogs->SetErrStr(QStringLiteral("�ظ�����Ʒ����ȷ���Ƿ������ˣ�") + line_str);
 						return false;
 					}
					left.remove(pos, rx.matchedLength());
					pos += rx.matchedLength();
					good_set.insert(name);
				}
			}

			if (!good_set.size())
			{
				wxLogs->SetErrStr(QStringLiteral("û�п�ʶ��ı�,�������ظ��ı���") + line_str);
				return false;
			}
			if (!left.isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("����ʶ����������ظ��ı���") + left);
				return false;
			}


			//���Ϲ�����  ֱ���޸�Դ����
			QString num_strr = right;
			
			for (auto iter : totolDict)
			{
				QRegExp rx(QStringLiteral("(-?") + iter + QStringLiteral("\\d+)"));
				QString str = num_strr;
				int pos = 0;
				while ((pos = rx.indexIn(str, pos)) != -1) {
					num_strr.replace(rx.cap(0), QStringLiteral(""));
					pos += rx.matchedLength();
				}
			}


			QString sub_num = SubNumber(num_strr);
			if (!sub_num.isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("���������������") + right);
				return false;
			}

			line_str = QStringLiteral("");
			for (auto iter : good_set)
			{
				line_str = line_str + iter + num_strr;
			}
			residue = line_str;
		}

		// ͬ�в����м��м������磺�� 100�ڹ�15����15����-30
		bool preAdd = false;	//��һ���Ƿ��ǼӺ�
		bool curAdd = true;
		bool firstWhile = true;

		// �������
 		QRegExp element_re(QStringLiteral("-?[\u4e00-\u9fa5]+-?\\+?\\d+"));
		int pos = element_re.indexIn(line_str);
		while ((pos = element_re.indexIn(line_str, pos)) != -1)
		{
			QString goods_num = line_str.mid(pos, element_re.matchedLength());
			QString goods = ExtracChinese(goods_num);
			QString num_str = goods_num;
			num_str.replace(goods, QStringLiteral(""));
			int num = num_str.toInt();
			if (num == 0)
			{
				if (QMessageBox::No == QMessageBox::question(nullptr, goods + QStringLiteral("����Ϊ0����ȷ������?"), goods + QStringLiteral("����Ϊ0����ȷ������?"), QMessageBox::Yes, QMessageBox::No))
				{
					return false;
				}
			}

			curAdd = num >= 0 ? true : false;
			if (firstWhile)
			{
				firstWhile = false;
			}
			else
			{
				if (preAdd != curAdd)
				{
					wxLogs->SetErrStr(QStringLiteral("����ͬ���ڼ��м����м���") + goods);
					return false;
				}
			}
			preAdd = curAdd;

			residue.replace(goods_num, QStringLiteral(""));

			// �������кϼ�
			bool totolDcitflag = false;
			for (auto iter : totolDict)
			{
				if (goods == iter)
				{
					pos += element_re.matchedLength();
					totolDcitflag = true;
					break;
				}
			}
			if (totolDcitflag)
			{
				continue;
			}
				

			if (!wxSetting->GetGoodsSet().contains(goods))	//�����ʶ��.����¼��
			{
				QString find_goods = wxSetting->GetRealNameByAlias(goods);
				if (find_goods.isEmpty())
				{
					wxLogs->SetErrStr(QStringLiteral("����ʶ��") + goods);
					return false;
				}
				goods = find_goods;
			}

			line_output.push_back(QPair<QString, QString>(goods, num_str));
			goods_totol[goods] += num;
			pos += element_re.matchedLength();
			totol += num;

			//line_str = line_str.mid(element_re.matchedLength(), line_str.length());

		}


		if (!residue.isEmpty())
		{
			tip_str = tip_str + residue + QStringLiteral("\n");
		}
		output_v.push_back(line_output);
	}


	if (!goods_totol.size())
	{
		wxLogs->SetErrStr(QStringLiteral("����ʶ��!"));
		return false;
	}
	if (!tip_str.remove(' ').remove('\t').remove('\n').isEmpty())
	{
		wxLogs->SetErrStr(QStringLiteral("����ʶ��") + tip_str);
		return false;
	}


	//ƥ��ɹ����������ݺ͵����¼
	QString appendlog;
	for (QMap<QString, int>::iterator iter = goods_totol.begin(); iter != goods_totol.end(); ++iter)
	{
		appendlog = appendlog + iter.key() + QString::number(iter.value());
		goods_totols_[iter.key()] += iter.value();
	}
	appendlog = appendlog + QStringLiteral(" �ϼ�")+QString::number(totol);
	totols_ += totol;
	

	//¼���¼
	wxLogs->AppendLog(user_, input, appendlog);
	return true;
}


void QGoodsData::RevocationData(QString src_str, QString input)
{
	// �õ�����
	QwxSetting *wxSetting = QwxSetting::ins();
	QwxLog *wxLogs = QwxLog::ins();
	const QMap<QString, QSet<QString>>& alias_map = wxSetting->GetAliasMap();
	const std::map<QString, QString, strLenComp> &replace_map = wxSetting->GetReplaceMap();
	const std::set<QString, strLenComp > &remove_set = wxSetting->GetDeleteSet();
	const std::set<QString, strLenComp> &totolDict = wxSetting->GetTotolDict();
	const std::set<QString, strLenComp> &gegeDict = wxSetting->GetGegeDict();

	QStringList split_list = input.split('\n');
	QString tip_str;
	//ÿһ��
	int totol = 0;
	QMap<QString, int> goods_totol;
	QVector<QVector<QPair<QString, QString>>> output_v;
	for (auto iter : split_list)
	{
		QVector<QPair<QString, QString>> line_output;

		//ÿ����Ʒ
		QString line_str = iter;
		

		QRegExp rx(QStringLiteral("(0?[0-9]|1[0-9]|2[0-3]):(0[0-9]|[12345][0-9])"));
		QString str = line_str;
		int poss = 0;
		if ((poss = rx.indexIn(str, poss)) != -1)
		{
			if (user_ == str.replace(rx.cap(0), QStringLiteral("")))
			{
				wxLogs->SetErrStr(QStringLiteral("�Ѻ��ԣ���ȷ������") + line_str);
				continue;
			}
			else
			{
				wxLogs->SetErrStr(QStringLiteral("��ƥ�䵽ʱ�䣬���Ǵ��������񲻶ԣ�����ֹ¼��!��") + line_str);
				return;
			}
		}


		//�滻�ַ�
		for (auto iter = replace_map.cbegin(); iter != replace_map.cend(); ++iter)
		{
			line_str.replace(iter->first, iter->second);
		}
		
		line_str.replace(QStringLiteral("="), QStringLiteral("�ϼ�"));
		line_str.replace(QStringLiteral("��"), QStringLiteral("�ϼ�"));
		if (false == checkError(line_str, wxLogs))
			return;

		//ɾ���ַ�
 		//line_str.replace(QStringLiteral(" "), QStringLiteral(""));
 		line_str.replace(QStringLiteral("\t"), QStringLiteral(" "));
		for (auto iter = split_list.begin(); iter != split_list.end(); ++iter)
		{
			line_str.replace(*iter, QStringLiteral(" "));
		}
		for (auto iter = remove_set.begin(); iter != remove_set.end(); ++iter)
		{
			line_str.replace(*iter, QStringLiteral(" "));
		}

		line_str.replace(QStringLiteral("--"), QStringLiteral(""));
		line_str.replace(QStringLiteral("--"), QStringLiteral(""));

		QString residue = line_str;



		// �Ƿ��и���
		QString gegestr;
		for (auto gege : gegeDict)
		{
			if (line_str.contains(gege))
			{
				gegestr = gege;
				break;
			}
		}

		// �Ƿ��и���
		if (!gegestr.isEmpty())
		{
			QString left;
			QString right;
			if (line_str.contains(gegestr))
			{
				QStringList list = line_str.split(gegestr);
				if (list.size() != 2)
				{
					wxLogs->SetErrStr(QStringLiteral("����ʶ��") + line_str);
					return;
				}
				left = list[0];
				right = list[1];
			}

			QSet<QString> good_set;

			if (left.isEmpty() || ExtractNum(right).isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("����ʶ��") + line_str);
				return;
			}

			QVector<QString> goods_sort_v = wxSetting->GetAllGoodsAliasStrsort();


			// �ж����б����б���
			for (int i = 0; i < goods_sort_v.size(); ++i)
			{
				//���������м��������ı���
				QRegExp rx(goods_sort_v[i]);
				int pos = 0;
				if ((pos = rx.indexIn(left, pos)) != -1) {
					QString name;
					if (wxSetting->GetGoodsSet().contains(goods_sort_v[i]))		//ʵ��
					{
						name = goods_sort_v[i];
					}
					else//����
					{
						name = wxSetting->GetRealNameByAlias(goods_sort_v[i]);
					}

					// �Ƿ��Ѿ������ֱ�����
					if (good_set.contains(name))
					{
						wxLogs->SetErrStr(QStringLiteral("�ظ�����Ʒ����ȷ���Ƿ������ˣ�") + line_str);
						return;
					}
					left.remove(pos, rx.matchedLength());
					pos += rx.matchedLength();
					good_set.insert(name);
				}
			}

			if (!good_set.size())
			{
				wxLogs->SetErrStr(QStringLiteral("û�п�ʶ��ı���") + line_str);
				return;
			}
			if (!left.isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("����ʶ��������ظ��ı���") + left);
				return;
			}

			
			//���Ϲ�����  ֱ���޸�Դ����
 			QString num_strr = right;

			for (auto iter : totolDict)
			{
				QRegExp rx(QStringLiteral("(-?") + iter + QStringLiteral("\\d+)"));
				QString str = num_strr;
				int pos = 0;
				while ((pos = rx.indexIn(str, pos)) != -1) {
					num_strr.replace(rx.cap(0), QStringLiteral(""));
					pos += rx.matchedLength();
				}
			}


			QString sub_num = SubNumber(num_strr);
			if (!sub_num.isEmpty())
			{
				wxLogs->SetErrStr(QStringLiteral("���������������") + right);
				return;
			}
			line_str = QStringLiteral("");
			for (auto iter : good_set)
			{
				line_str = line_str + iter + num_strr;
			}
			residue = line_str;
		}


		QRegExp element_re(QStringLiteral("-?[\u4e00-\u9fa5]+-?\\+?\\d+"));
		int pos = element_re.indexIn(line_str);
		while ((pos = element_re.indexIn(line_str, pos)) != -1)
		{
			QString goods_num = line_str.mid(pos, element_re.matchedLength());
			QString goods = ExtracChinese(goods_num);
			QString num_str = goods_num;
			num_str.replace(goods, QStringLiteral(""));
			int num = num_str.toInt();
			if (num == 0)
			{
				if (QMessageBox::No == QMessageBox::question(nullptr, goods + QStringLiteral("����Ϊ0����ȷ������?"), goods + QStringLiteral("����Ϊ0����ȷ������?"), QMessageBox::Yes, QMessageBox::No))
				{
					return;
				}
			}

			residue.replace(goods_num, QStringLiteral(""));

			// �������кϼ�
			bool totolDcitflag = false;
			for (auto iter : totolDict)
			{
				if (goods == iter)
				{
					pos += element_re.matchedLength();
					totolDcitflag = true;
					break;
				}
			}
			if (totolDcitflag)
			{
				continue;
			}

			if (!wxSetting->GetGoodsSet().contains(goods))	//�����ʶ��.����¼��
			{
				QString find_goods = wxSetting->GetRealNameByAlias(goods);
				if (find_goods.isEmpty())
				{
					wxLogs->SetErrStr(QStringLiteral("����ʶ��") + goods);
					return;
				}
				goods = find_goods;
			}

			// ¼����+�� ������-
			line_output.push_back(QPair<QString, QString>(goods, num_str));
			goods_totol[goods] -= num;
			pos += element_re.matchedLength();
			totol -= num;

			//line_str = line_str.mid(element_re.matchedLength(), line_str.length());

		}


		if (!residue.isEmpty())
		{
			tip_str = tip_str + residue + QStringLiteral("\n");
		}
		output_v.push_back(line_output);
	}


	if (!goods_totol.size())
	{
		wxLogs->SetErrStr(QStringLiteral("����ʶ��!"));
		return;
	}
	if (!tip_str.remove(' ').remove('\t').remove('\n').isEmpty())
	{
		wxLogs->SetErrStr(QStringLiteral("����ʶ��") + tip_str);
		return ;
	}


	//ƥ��ɹ����������ݺ͵����¼
	QString appendlog;
	for (QMap<QString, int>::iterator iter = goods_totol.begin(); iter != goods_totol.end(); ++iter)
	{
		appendlog = appendlog + iter.key() + QString::number(iter.value());
		goods_totols_[iter.key()] += iter.value();
	}
	appendlog = appendlog + QStringLiteral(" �ϼ�") + QString::number(totol);
	totols_ += totol;


	//¼���¼
	wxLogs->AppendLog(user_, input, appendlog);
}

QString QGoodsData::ExtractNum(QString &str)
{
	QString tmp;
	int len = str.length();
	for (int i = 0; i < len; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
			tmp.append(str[i]);
	}
	return tmp;
}

QString QGoodsData::SubNumber(QString str)
{
	QString tmp;
	bool fristNum = false;
	int len = str.length();
	for (int i = 0; i < len; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			fristNum = true;
		}
		else if (str[i] == '-')
		{
			if (fristNum == true)
			{
				tmp.append(str[i]);
			}
		}
		else
		{
			tmp.append(str[i]);
		}
			
	}
	return tmp;
}

QString QGoodsData::ExtracChinese(QString &str)
{
	QString chineseStr;
	int nCount = str.count();
	for (int i = 0; i < nCount; i++)
	{
		QChar cha = str.at(i);
		ushort uni = cha.unicode();
		if (uni >= 0x4E00 && uni <= 0x9FA5)
		{
			chineseStr.append(uni);
		}
	}
	return chineseStr;
}

int QGoodsData::GetTotol()
{
	return totols_;
}

void QGoodsData::InitGoodTotol()
{
	QwxSetting *wxSetting = QwxSetting::ins();
	const QVector<QString>& good_set = wxSetting->GetGoodsSet();
	for (auto goods : good_set)
	{
		goods_totols_[goods] = 0;
	}
}