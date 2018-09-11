#include "DayTotolTable.h"
#include <QPainter>
#include <QFileDialog>
#include <QDirIterator>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>

DayTotolTable::DayTotolTable(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void DayTotolTable::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QString ExtracChinese(QString &str)
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

QString ExtractNum(QString &str)
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

bool DayTotolTable::getUserSetting(QString file_path, QMap<QString, QMap<QString, QMap<QString, DataInfo>>> &locals_info
	, QMap<QString, QVector<QString>> &kaibao_result
	, QMap<QString, double> &daili_profit
	, QMap<QString, double> &good_Profit
	, QString &date)
{
	std::vector<QString> locals;
	QString app_path = QApplication::applicationDirPath();
	QDir dir(file_path);
	dir.setFilter(QDir::Dirs);
	QString datename = dir.dirName();

	int year;
	int month;
	int day;
	sscanf(datename.toLocal8Bit().data(), "%d-%d-%d", &year, &month, &day);
	date = datename;
	// locals_info
	foreach(QFileInfo localDir, dir.entryInfoList())
	{
		if (localDir.fileName() == "." || localDir.fileName() == "..") continue;
		QString local_folder = localDir.absoluteFilePath();
		dir.setPath(local_folder);
		foreach(QFileInfo sessionDir, dir.entryInfoList())
		{
			if (sessionDir.fileName() == "." || sessionDir.fileName() == "..") continue;
			QString session_folder = sessionDir.absoluteFilePath();
			dir.setPath(session_folder);
			foreach(QFileInfo dailiDir, dir.entryInfoList())
			{
				if (dailiDir.fileName() == "." || dailiDir.fileName() == ".." || dailiDir.fileName() == QStringLiteral("�ܼ�")) continue;
				QString daili_folder = dailiDir.absoluteFilePath();

				dir.setPath(daili_folder);
				dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
				dir.setSorting(QDir::Name | QDir::Reversed);
				QStringList filter;
				filter << "*.txt";
				QFileInfoList files = dir.entryInfoList(filter, QDir::Files);
				
				if (files.size() < 1)
					continue;

				QString file_name = files[0].absoluteFilePath();

				DataInfo datalinfo;
				datalinfo.date = datename;
				datalinfo.localc = localDir.fileName();
				datalinfo.session = sessionDir.fileName();
				datalinfo.daili = dailiDir.fileName();
				datalinfo.filename = file_name;
				
				
				QFile file(file_name);
				file.open(QIODevice::ReadOnly | QIODevice::Text);
				while (!file.atEnd()) {
					QByteArray line = file.readLine();
					QString str(line);
					datalinfo.data[ExtracChinese(str)] = ExtractNum(str).toInt();
				}
				file.close();
				locals_info[datalinfo.localc][datalinfo.daili][datalinfo.session] = datalinfo;
				locals.push_back(localDir.fileName());
			}
		}
	}


	//daili_profit
	QFile f;
	QString lineStr;
	// ����ϵ��
	f.setFileName(app_path + QStringLiteral("/agencyProfit.ini"));
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
				daili_profit[str_list[0]] = str_list[1].toDouble();
			}
		}
		f.close();
	}


 	for (int i = 0; i < locals.size(); ++i)
 	{
 		QString local_result_path = app_path + "/" + locals[i] + ".txt";
 		f.setFileName(local_result_path);
 		if (f.open(QIODevice::ReadOnly | QIODevice::Text))
 		{
 			QTextStream txtInput(&f);
 			txtInput.setCodec("UTF-8"); //��ע������
 
 			int cur_year = 0;
 			int cur_month = 0;
 			while (!txtInput.atEnd())
 			{
 				lineStr = txtInput.readLine();
 				lineStr.replace(QStringLiteral(" "), QStringLiteral(""));
 				lineStr.replace(QStringLiteral("\t"), QStringLiteral(""));
 				if (lineStr.isEmpty())
 				{
 					continue;
 				}

 				else if (-1 != lineStr.indexOf(QStringLiteral("��")))
 				{
 					cur_year = lineStr.split(QStringLiteral("��"))[0].toInt();
 				}
 				else if (-1 != lineStr.indexOf(QStringLiteral("��")))
 				{
 					cur_month = lineStr.split(QStringLiteral("��"))[0].toInt();
 				}
 				else if (-1 != lineStr.indexOf(QStringLiteral("��")))
 				{
 					QStringList strList = lineStr.split(QStringLiteral("��"));
 					int cur_day = strList[0].toInt();

					if (year == cur_year && month == cur_month && day == cur_day)
					{
						QStringList good_list = strList[1].split(QStringLiteral(","));
						QVector<QString> goods_v;
						for (int i = 0; i < good_list.count(); ++i)
						{
							goods_v.push_back(good_list[i]);
						}
						kaibao_result[locals[i]] = goods_v;
						break;
					}
 				}
 				else
 				{
 					QMessageBox::warning(this, QStringLiteral(""), QStringLiteral("�����ϸ�ʽ����,�Ѻ��ԣ�") + lineStr);
 					f.close();
 					return false;
 				}
 			}
 			f.close();
 		}
 		else
 		{
 			QMessageBox::warning(nullptr, QStringLiteral("����"), local_result_path + QStringLiteral("��ʧ��!"));
 			return false;
 		}
 			
 	}




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
				good_Profit[str_list[0]] = str_list[1].toDouble();
			}
		}
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("����.ini��ʧ��!"));
		return false;
	}

	return true;
}

void DayTotolTable::on_change_date()
{
	QString dir_path = QFileDialog::getExistingDirectory(this, QStringLiteral("��ѡ�������ļ���..."), QApplication::applicationDirPath());
	if (dir_path.isEmpty())
	{
		return;
	}
	QString log;
	ui.label_date_path->setText(dir_path);

	// ���� ���� ����
	QMap<QString, QMap<QString, QMap<QString, DataInfo>>> locals_info;
	QMap<QString, QVector<QString>> kaibao_result;
	QMap<QString, double> daili_profit;
	QMap<QString, double> good_Profit;
	QString date;
	if (!getUserSetting(dir_path, locals_info, kaibao_result, daili_profit, good_Profit,date))
		return;



	// ��������
	for (auto local_it = locals_info.begin(); local_it != locals_info.end(); ++local_it)
	{
		QString local_name = local_it.key();
		for (auto daili_it = local_it.value().begin(); daili_it != local_it.value().end(); ++daili_it)
		{
			QString daili_name = daili_it.key();
			if (!daili_it.value().size())
			{
				log += QStringLiteral("\n") + local_name + daili_name + QStringLiteral("����Ϊ��");
				continue;
			}
				
			double dailifit = daili_profit.find(daili_name) != daili_profit.end() ?
				daili_profit[daili_name] : daili_profit[QStringLiteral("Ĭ��")];

			
			QDayView *daili_view = new QDayView;
			QTableWidget* tabel = daili_view->getTable();

			QFont head_font(daili_view->font());
			head_font.setPixelSize(22);

			QFont nomal_font("Microsoft YaHei", 14, 30);
			tabel->setFont(nomal_font);

			
			QTableWidgetItem *biaoti = new QTableWidgetItem(QStringLiteral("С�����սᵥ"));
			biaoti->setBackground(QColor(128, 128, 128, 180));
			biaoti->setForeground(QColor(222, 222, 222));
			biaoti->setFont(head_font);
			biaoti->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *changmingcheng = new QTableWidgetItem(QStringLiteral("�����ơ�"));
			changmingcheng->setBackground(QColor(128, 128, 128, 180));
			changmingcheng->setForeground(QColor(222, 222, 222));
			changmingcheng->setFont(head_font);
			changmingcheng->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *changmingcheng_value = new QTableWidgetItem(local_name);
			changmingcheng_value->setBackground(QColor(128, 128, 128, 180));
			changmingcheng_value->setForeground(QColor(222, 222, 222));
			changmingcheng_value->setFont(head_font);
			changmingcheng_value->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *dailiren = new QTableWidgetItem(QStringLiteral("�����ˡ�"));
			dailiren->setBackground(QColor(128, 128, 128, 180));
			dailiren->setForeground(QColor(222, 222, 222));
			dailiren->setFont(head_font);
			dailiren->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *dailiren_value = new QTableWidgetItem(daili_name + "-" + local_name + "-" + QString::number(dailifit) + "%");
			dailiren_value->setBackground(QColor(128, 128, 128, 180));
			dailiren_value->setForeground(QColor(222, 222, 222));
			dailiren_value->setFont(head_font);
			dailiren_value->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *riqi = new QTableWidgetItem(QStringLiteral("���ڡ�"));
			riqi->setBackground(QColor(128, 128, 128, 180));
			riqi->setForeground(QColor(222, 222, 222));
			riqi->setFont(head_font);
			riqi->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *riqi_value = new QTableWidgetItem(date);
			riqi_value->setBackground(QColor(128, 128, 128, 180));
			riqi_value->setForeground(QColor(222, 222, 222));
			riqi_value->setFont(head_font);
			riqi_value->setTextAlignment(Qt::AlignCenter);


			tabel->setSpan(0, 0, 1, tabel->columnCount());
			tabel->setItem(0, 0, biaoti);
			tabel->setItem(1, 0, changmingcheng);
			tabel->setItem(1, 1, changmingcheng_value);
			tabel->setItem(1, 2, dailiren);
			tabel->setSpan(1, 3, 1, 2);
			tabel->setItem(1, 3, dailiren_value);
			tabel->setItem(1, 5, riqi);
			tabel->setItem(1, 6, riqi_value);


			QTableWidgetItem *changci = new QTableWidgetItem(QStringLiteral("����"));
			changci->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *kaibao = new QTableWidgetItem(QStringLiteral("����"));
			kaibao->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *zongyazhue = new QTableWidgetItem(QStringLiteral("��Ѻע��"));
			zongyazhue->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *dailifei = new QTableWidgetItem(QStringLiteral("�����"));
			dailifei->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *zhongbaoyazhu = new QTableWidgetItem(QStringLiteral("�б�Ѻע"));
			zhongbaoyazhu->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *zhongbaoxuyaopeideqian = new QTableWidgetItem(QStringLiteral("�б���Ǯ"));
			zhongbaoxuyaopeideqian->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *heji = new QTableWidgetItem(QStringLiteral("�ϼ�"));
			heji->setTextAlignment(Qt::AlignCenter);
			tabel->setItem(2, 0, changci);
			tabel->setItem(2, 1, kaibao);
			tabel->setItem(2, 2, zongyazhue);
			tabel->setItem(2, 3, dailifei);
			tabel->setItem(2, 4, zhongbaoyazhu);
			tabel->setItem(2, 5, zhongbaoxuyaopeideqian);
			tabel->setItem(2, 6, heji);

			for (int i = 0; i < 3; ++i)
			{
				int row = 2 + i+1;
				QTableWidgetItem *changci_value = new QTableWidgetItem(QStringLiteral("��") + QString::number(i+1) + QStringLiteral("��"));
				changci_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *kaibao_value = new QTableWidgetItem(QStringLiteral("δ��"));
				kaibao_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *zongyazhue_value = new QTableWidgetItem(QStringLiteral(""));
				zongyazhue_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *dailifei_value = new QTableWidgetItem(QStringLiteral(""));
				dailifei_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *zhongbaoyazhu_value = new QTableWidgetItem(QStringLiteral(""));
				zhongbaoyazhu_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *zhongbaoxuyaopeideqian_value = new QTableWidgetItem(QStringLiteral(""));
				zhongbaoxuyaopeideqian_value->setTextAlignment(Qt::AlignCenter);
				QTableWidgetItem *heji_value = new QTableWidgetItem(QStringLiteral(""));
				heji_value->setTextAlignment(Qt::AlignCenter);
				tabel->setItem(row, 0, changci_value);
				tabel->setItem(row, 1, kaibao_value);
				tabel->setItem(row, 2, zongyazhue_value);
				tabel->setItem(row, 3, dailifei_value);
				tabel->setItem(row, 4, zhongbaoyazhu_value);
				tabel->setItem(row, 5, zhongbaoxuyaopeideqian_value);
				tabel->setItem(row, 6, heji_value);
			}

			int session_size = 0;
			for (auto session_it = daili_it.value().begin(); session_it != daili_it.value().end(); ++session_it)
			{
				QString session_str = session_it.key();
				int session_int = ExtractNum(session_str).toInt();
				int row = 2 + session_int;
				if (kaibao_result[local_name].size() < session_int)
				{
					log += QStringLiteral("\n") + date+ local_name
						+ session_str + QStringLiteral("δ���ÿ�����Ʒ");
					continue;
				}
				session_size++;
				QString kaibao_str = kaibao_result[local_name][session_int-1];
				
				std::map<QString, int>& data = locals_info[local_name][daili_name][session_str].data;
				int yazhuzonge_int = 0;
				for (auto data_it = data.begin(); data_it != data.end(); data_it++)
				{
					yazhuzonge_int += data_it->second;
				}
				int dailifei_int = yazhuzonge_int*daili_profit[daili_name] / 100;
				QString zongyazhue_str = QString::number(yazhuzonge_int);
				int zhongbaoxuyaopeideqian_int= data[kaibao_str] * good_Profit[kaibao_str];

				tabel->item(row, 1)->setText(kaibao_str);
				tabel->item(row, 2)->setText(zongyazhue_str);
				tabel->item(row, 3)->setText(QString::number(dailifei_int));
				tabel->item(row, 4)->setText(QString::number(data[kaibao_str]));
				tabel->item(row, 5)->setText(QString::number(zhongbaoxuyaopeideqian_int));
				tabel->item(row, 6)->setText(QString::number(yazhuzonge_int - dailifei_int - zhongbaoxuyaopeideqian_int));
			}
			int yazhuzonge_int = tabel->item(3, 2)->text().toInt() + tabel->item(4, 2)->text().toInt() + tabel->item(5, 2)->text().toInt();

			
 			QTableWidgetItem *hejichangci = new QTableWidgetItem(QStringLiteral("�ϼ�")+QString::number(session_size)+QStringLiteral("��"));
 			hejichangci->setTextAlignment(Qt::AlignCenter);
 			QTableWidgetItem *yazhuzonge_totol = new QTableWidgetItem(QString::number(yazhuzonge_int));
 			yazhuzonge_totol->setTextAlignment(Qt::AlignCenter);
 			QTableWidgetItem *dailifei_totol = new QTableWidgetItem(QString::number(tabel->item(3, 3)->text().toInt() + tabel->item(4, 3)->text().toInt() + tabel->item(5, 3)->text().toInt()));
			dailifei_totol->setTextAlignment(Qt::AlignCenter);
 			QTableWidgetItem *zhongbaoyazhu_totol = new QTableWidgetItem(QString::number(tabel->item(3, 4)->text().toInt() + tabel->item(4, 4)->text().toInt() + tabel->item(5, 4)->text().toInt()));
			zhongbaoyazhu_totol->setTextAlignment(Qt::AlignCenter);
 			
 			QTableWidgetItem *zhongbaoxuyaopeideqian_totol = new QTableWidgetItem(QString::number(tabel->item(3, 5)->text().toInt() + tabel->item(4, 5)->text().toInt() + tabel->item(5, 5)->text().toInt()));
			zhongbaoxuyaopeideqian_totol->setTextAlignment(Qt::AlignCenter);
 			
			QTableWidgetItem *heji_totol = new QTableWidgetItem(/*QStringLiteral("�����踶�ϼ�") +*/ QString::number(tabel->item(3, 6)->text().toInt() + tabel->item(4, 6)->text().toInt() + tabel->item(5, 6)->text().toInt()));
			heji_totol->setBackgroundColor(QColor(106,168,79,200));
			heji_totol->setForeground(QColor(255, 255, 0));
			heji_totol->setTextAlignment(Qt::AlignCenter);
			

			tabel->setSpan(6, 0, 1, 2);
			tabel->setItem(6, 0, hejichangci);
			tabel->setItem(6, 2, yazhuzonge_totol);
			tabel->setItem(6, 3, dailifei_totol);
			tabel->setItem(6, 4, zhongbaoyazhu_totol);
			tabel->setItem(6, 5, zhongbaoxuyaopeideqian_totol);
			tabel->setItem(6, 6, heji_totol);

			tabel->setSpan(7, 0, 1, tabel->columnCount());
			QTableWidgetItem *guanggao = new QTableWidgetItem(QStringLiteral("��棺��Ѷ���ֵ�ͳ�Ʒ��������΢�ţ�AzhangZihua"));
			guanggao->setTextAlignment(Qt::AlignCenter);
			tabel->setItem(7, 0, guanggao);
			daili_view->show();
			daili_view->setAttribute(Qt::WA_DeleteOnClose);
		}
	}

	ui.label->setText(log);
}


