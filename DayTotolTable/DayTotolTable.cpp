#include "DayTotolTable.h"
#include <QPainter>
#include <QFileDialog>
#include <QDirIterator>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
#include <QLineEdit>
#include <QJsonDocument>
#include <QJsonObject> 
#include <QJsonArray>
#include <QDebug> 
#include <QTabWidget>
#include <QHBoxLayout>

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
    , QMap<QString, QMap<QString, QMap<QString, double>>> &daili_session_profit
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
    else
    {
        QMessageBox::information(this, QStringLiteral("����ʧ��"), QStringLiteral("agencyProfit.ini��ȡʧ��"), 0);
        return false;
    }

    QByteArray json; // json ������
    QFile file(app_path + QStringLiteral("/�����س���ϵ��.json")); // Json ���ļ�
    
    // [1] ��ȡ Json �ļ�����
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        json = file.readAll();
    }
    else {
        QMessageBox::information(this, QStringLiteral("����ʧ��"), QStringLiteral("�����س���ϵ��.json"), 0);
        return false;
    }
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(json, &jsonError);
    file.close();

    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonArray array = doucment.array();
        int nSize = array.size();
        for (int i = 0; i < nSize; ++i)
        {
            QJsonObject changdi = array.at(i).toObject();
            QJsonObject changci_array = changdi.value(QStringLiteral("����")).toObject();
            QStringList changci_keys = changci_array.keys();
            for (int j = 0; j < changci_keys.size(); ++j)
            {
                QString changci_key = changci_keys[j];
                QJsonObject daili_array = changci_array.value(changci_key).toObject();

                QStringList daili_keys = daili_array.keys();
                for (int j = 0; j < daili_keys.size(); ++j)
                {
                    QString daili_key = daili_keys[j];
                    double daili_xishu = daili_array.value(daili_key).toDouble();
                    daili_session_profit[changdi.value(QStringLiteral("����")).toString()]
                        [changci_key][daili_key] = daili_xishu;
                }
            }
        }
    }
    else
    {
        QMessageBox::information(this, QStringLiteral("����ʧ��"), QStringLiteral("�����س���ϵ��.json"), 0);
        return false;
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
						for (int j = 0; j < good_list.count(); ++j)
						{
							goods_v.push_back(good_list[j]);
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

void DayTotolTable::totol()
{
	QString log;
	// ��������

    QTabWidget *changdi_tab_widget = new QTabWidget();
    changdi_tab_widget->setAttribute(Qt::WA_DeleteOnClose);
    changdi_tab_widget->setWindowTitle(QStringLiteral("�սᱨ��"));
    changdi_tab_widget->resize(QSize(785,390));
	for (auto local_it = locals_info.begin(); local_it != locals_info.end(); ++local_it)
	{
        QString local_name = local_it.key();  // ������
        QTabWidget *daili_tab_widget = new QTabWidget(changdi_tab_widget);
        changdi_tab_widget->addTab(daili_tab_widget, local_name);

        // ��������
		for (auto daili_it = local_it.value().begin(); daili_it != local_it.value().end(); ++daili_it)
		{
			QString daili_name = daili_it.key();

			if (!daili_it.value().size())
			{
				log += QStringLiteral("\n") + local_name + daili_name + QStringLiteral("����Ϊ��");
				continue;
			}

            // ����ϵ��
			double dailifit = daili_profit.find(daili_name) != daili_profit.end() ?
				daili_profit[daili_name] : daili_profit[QStringLiteral("Ĭ��")];

            QDayView *daili_view = new QDayView(daili_tab_widget);
            daili_tab_widget->addTab(daili_view, daili_name);
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
			QTableWidgetItem *dailiren_value = new QTableWidgetItem();
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
				int row = 2 + i + 1;
				QTableWidgetItem *changci_value = new QTableWidgetItem(QStringLiteral("��") + QString::number(i + 1) + QStringLiteral("��"));
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
            double dailifit_session = 0;
			for (auto session_it = daili_it.value().begin(); session_it != daili_it.value().end(); ++session_it)
			{
                QString session_str = session_it.key();
				int session_int = ExtractNum(session_str).toInt();
				int row = 2 + session_int;
				if (kaibao_result[local_name].size() < session_int)
				{
					log += QStringLiteral("\n") + date + local_name
						+ session_str + QStringLiteral("δ���ÿ�����Ʒ");
					continue;
				}
				session_size++;
				QString kaibao_str = kaibao_result[local_name][session_int - 1];

				std::map<QString, int>& data = locals_info[local_name][daili_name][session_str].data;
				int yazhuzonge_int = 0;
				for (auto data_it = data.begin(); data_it != data.end(); data_it++)
				{
					yazhuzonge_int += data_it->second;
				}

                dailifit_session = daili_session_profit[local_name][daili_name].find(QString(session_str).remove(QStringLiteral("��"))) != daili_session_profit[local_name][daili_name].end() ?
                    daili_session_profit[local_name][daili_name][QString(session_str).remove(QStringLiteral("��"))] : daili_profit[QStringLiteral("Ĭ��")];
                log += QStringLiteral("\n") + local_name + daili_name + session_str + QStringLiteral("����ϵ������Ϊ��, ϵͳʹ�õ���Ĭ�ϴ���ϵ��!");
				int dailifei_int = yazhuzonge_int*dailifit_session / 100.0;
				QString zongyazhue_str = QString::number(yazhuzonge_int);
				int zhongbaoxuyaopeideqian_int = data[kaibao_str] * good_Profit[kaibao_str];

				tabel->item(row, 1)->setText(kaibao_str);
				tabel->item(row, 2)->setText(zongyazhue_str);
				tabel->item(row, 3)->setText(QString::number(dailifei_int));
				tabel->item(row, 4)->setText(QString::number(data[kaibao_str]));
				tabel->item(row, 5)->setText(QString::number(zhongbaoxuyaopeideqian_int));
				tabel->item(row, 6)->setText(QString::number(yazhuzonge_int - dailifei_int - zhongbaoxuyaopeideqian_int));
			}
            dailiren_value->setText(daili_name + "-" + local_name + "-" + QString::number(dailifit_session) + "%");
			int yazhuzonge_int = tabel->item(3, 2)->text().toInt() + tabel->item(4, 2)->text().toInt() + tabel->item(5, 2)->text().toInt();


			QTableWidgetItem *hejichangci = new QTableWidgetItem(QStringLiteral("�ϼ�") + QString::number(session_size) + QStringLiteral("��"));
			hejichangci->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *yazhuzonge_totol = new QTableWidgetItem(QString::number(yazhuzonge_int));
			yazhuzonge_totol->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *dailifei_totol = new QTableWidgetItem(QString::number(tabel->item(3, 3)->text().toInt() + tabel->item(4, 3)->text().toInt() + tabel->item(5, 3)->text().toInt()));
			dailifei_totol->setTextAlignment(Qt::AlignCenter);
			QTableWidgetItem *zhongbaoyazhu_totol = new QTableWidgetItem(QString::number(tabel->item(3, 4)->text().toInt() + tabel->item(4, 4)->text().toInt() + tabel->item(5, 4)->text().toInt()));
			zhongbaoyazhu_totol->setTextAlignment(Qt::AlignCenter);

			QTableWidgetItem *zhongbaoxuyaopeideqian_totol = new QTableWidgetItem(QString::number(tabel->item(3, 5)->text().toInt() + tabel->item(4, 5)->text().toInt() + tabel->item(5, 5)->text().toInt()));
			zhongbaoxuyaopeideqian_totol->setTextAlignment(Qt::AlignCenter);

			QTableWidgetItem *heji_totol;
			int heji_totol_int = tabel->item(3, 6)->text().toInt() + tabel->item(4, 6)->text().toInt() + tabel->item(5, 6)->text().toInt();
			if (heji_totol_int > 0)
				heji_totol = new QTableWidgetItem(QStringLiteral("�����踶�ϼ�") + QString::number(heji_totol_int));
			else
				heji_totol = new QTableWidgetItem(QStringLiteral("�ϼ��踶����") + QString::number(-heji_totol_int));
			heji_totol->setBackgroundColor(QColor(106, 168, 79, 200));
			heji_totol->setFont(QFont("Microsoft YaHei", 10, 30));
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

			tabel->resizeRowsToContents();
			
		}
	}
    changdi_tab_widget->show();
	log += QStringLiteral("\n") + date  + QStringLiteral("ͳ����ɣ�");
	ui.label->setText(log);
}

void DayTotolTable::on_change_date()
{
	locals_info.clear();
	kaibao_result.clear();
	daili_profit.clear();
	daili_session_profit.clear();
	good_Profit.clear();
	date.clear();
	dir_path.clear();


    dir_path = QApplication::applicationDirPath() + QStringLiteral("\\����");
    if (!QDir(dir_path).exists())
        dir_path = QApplication::applicationDirPath();
    dir_path = QFileDialog::getExistingDirectory(this, QStringLiteral("��ѡ�������ļ���..."), dir_path);

	if (dir_path.isEmpty())
	{
		return;
	}

	ui.label_date_path->setText(dir_path);


    if (!getUserSetting(dir_path, locals_info, kaibao_result, daili_session_profit, good_Profit, date))
		return;

    totol();

// 	QDialog dlg(this);
// 	dlg.setWindowTitle(QStringLiteral("���ô�����ϵ��"));
// 	QGridLayout * layout = new QGridLayout(&dlg);
// 	int daili_step = 0;
// 	for (auto it = daili_profit.begin(); it != daili_profit.end(); ++it, ++daili_step)
// 	{
// 		layout->addWidget(new QLabel(it.key()), daili_step, 0);
// 		for (int i = 0; i < 3; ++i)
// 		{
// 			QLineEdit* Edit = new QLineEdit(QString::number(it.value()));
// 			QIntValidator* IntValidator = new QIntValidator;
// 			IntValidator->setRange(-50, 50);
// 			Edit->setValidator(IntValidator);
// 			layout->addWidget(Edit, daili_step, i+1);
// 		}
// 	}
// 
// 	dlg.exec();
//  	for (int i = 0; i < layout->rowCount(); ++i)
//  	{
// 		QString daili = ((QLabel*)(layout->itemAtPosition(i, 0)->widget()))->text();
//  		for (int j = 0; j < 3; ++j)
//  		{
// 			daili_session_profit[daili][QString::number(j+1)+QStringLiteral("��")] = ((QLineEdit*)(layout->itemAtPosition(i, j+1)->widget()))->text().toInt();
//  		}
//  	}
}

