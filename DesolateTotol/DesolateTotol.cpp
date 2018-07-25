#include "DesolateTotol.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidgetItem>
#include "QProgressWidget.h"
#include <QSetting.h>


class CommonHelper
{
public:
	static void setStyle(QWidget *widget, const QString &style) {
		QFile qss(style);
		qss.open(QFile::ReadOnly);
		widget->setStyleSheet(qss.readAll());
		qss.close();
	}
};


DesolateTotol::DesolateTotol(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.lineEdit_title_days->setValidator(new QIntValidator(0, std::numeric_limits<int>::max(), ui.lineEdit_title_days));

	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/viewport.qss"));
	ui.tableWidget_totol->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);



	ui.tableWidget_totol->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));

	QImage _image;
	_image.load(QApplication::applicationDirPath() + (QStringLiteral("/desolateTotolbg.jpg")));
	ui.widget_chart->setAutoFillBackground(true);   // �������һ��Ҫ����
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	ui.widget_chart->setPalette(pal);
}

void DesolateTotol::setfilepath()
{
	
	QString relative_fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open File"), QApplication::applicationDirPath(), QStringLiteral("txt (*.txt *.dat)"));
	if (relative_fileName.isEmpty())
	{
		return;
	}

	ui.pushButton_export->setEnabled(true);
	totol_goods_.clear();

	QFileInfo fi(relative_fileName);
	QString filePath =  fi.absoluteFilePath();
	QString fileName =  fi.baseName();
	ui.lineEdit_filepath->setText(filePath);
	ui.lineEdit_title->setText(fileName);


	QFile f;
	QString lineStr;
	int rowCount = 0;
	//��������
	f.setFileName(filePath);
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
			else if (-1!=lineStr.indexOf(QStringLiteral("��")))
			{
				QStringList strList = lineStr.split(QStringLiteral("��"));
				int cur_day = strList[0].toInt();
				QStringList good_list = strList[1].split(QStringLiteral(","));
				QVector<QVector<QString>> goods_v;
				for (int i = 0; i < good_list.count(); ++i)
				{
					QVector<QString> t;
					QStringList new_good_list = good_list[i].split(QStringLiteral("&"));
					for (int j = 0; j < new_good_list.size(); ++j)
					{
						t.push_back(new_good_list[j]);
					}
					goods_v.push_back(t);
				}
				totol_goods_[cur_year][cur_month][cur_day] = goods_v;
			}
			else
			{
				QMessageBox::warning(this, QStringLiteral(""), QStringLiteral("�����ϸ�ʽ����,�Ѻ��ԣ�") + lineStr);
			}
			
		}
		f.close();
	}
	else
		QMessageBox::warning(nullptr, QStringLiteral("����"), filePath + QStringLiteral("��ʧ��!"));

	UpdateDesolateView();
}

void DesolateTotol::on_export()
{
	QFileInfo fi(ui.lineEdit_filepath->text());
	if(!fi.exists())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("�༭�����·������ȷ"), 0);
		return;
	}
	QString fileDir = fi.absolutePath();
	QString fileName = fi.baseName();

	QPixmap pixmap(ui.widget_chart->size());
	ui.widget_chart->render(&pixmap);
	pixmap.save(fileDir +(QStringLiteral("/")) + fileName + QStringLiteral("�䱦ͳ��.jpg"));

	QMessageBox::information(this, QStringLiteral("����ɹ�"), fileDir + (QStringLiteral("/")) + fileName + QStringLiteral("�䱦ͳ��.jpg"), 0);
}

void DesolateTotol::DataInit()
{
	desolate_nums_.clear();
	const QVector<QString>& gset = QSetting::ins().gset_;
	for (auto goods : gset)
	{
		desolate_nums_[goods] = 0;
	}

	desolate_day_nums_.clear();
	for (auto goods : gset)
	{
		desolate_day_nums_[goods] = 0;
	}
}

void DesolateTotol::DataDispose()
{
	int totol_days = ui.lineEdit_title_days->text().toInt();

	QVector<QVector<QVector<QString>>>days_goods;
		//ÿ��
	
	for (auto iter = totol_goods_.begin(); iter != totol_goods_.end(); ++iter)
	{
		const QMap<int, QMap<int, QVector<QVector<QString>>>> months = iter.value();
		for (auto it = months.begin(); it != months.end(); ++it)
		{
			const QMap<int, QVector<QVector<QString>>> &days = it.value();
			for (auto itt = days.begin(); itt != days.end(); ++itt)
			{
				days_goods.push_back(itt.value());
			}
		}
	}


	int day_step = totol_days;
	int i = 0;
	if (days_goods.count() > totol_days)
	{
		 i = days_goods.count() - totol_days;
	}
	else
	{
		i = 0;
	}

	for (; i < days_goods.count(); ++i) //������
	{
		QSet<QString> day_goods;
		for (int j = 0; j < days_goods[i].count(); ++j) //���г�
		{
			QSet<QString> sesson_goods;
			for (auto iter = desolate_nums_.begin(); iter != desolate_nums_.end(); ++iter) //ÿһ��
			{
				if (days_goods[i][j].contains(iter.key()))
				{
					sesson_goods.insert(iter.key());
					day_goods.insert(iter.key());
				}
			}
			for (auto iter = desolate_nums_.begin(); iter != desolate_nums_.end(); ++iter) //ÿһ��
			{
				if (sesson_goods.contains(iter.key()))
				{
					iter.value() = 0;
				}
				else
				{
					iter.value()++;
				}
			}
		}
		for (auto iter = desolate_day_nums_.begin(); iter != desolate_day_nums_.end(); ++iter) //ÿһ��
		{
			if (day_goods.contains(iter.key()))
			{
				iter.value() = 0;
			}
			else
			{
				iter.value()++;
			}
		}
	}
}

bool myCmpare(QPair<QString, int> &p1, QPair<QString, int> &p2)
{
	return p1.second > p2.second;
}


void DesolateTotol::UpdateDesolateView()
{
	int totol_days = ui.lineEdit_title_days->text().toInt();

	QVector<QVector<QString>> days_goods;
	DataInit();
	DataDispose();

	QString year;
	QString month;
	QString day;
	if (totol_goods_.count())
	{
		auto &last_yearit = (totol_goods_.end() - 1);
		year = QString::number(last_yearit.key());
		if (last_yearit.value().count())
		{
			month = QString::number((last_yearit.value().end() - 1).key());
			auto & days_map = (last_yearit.value().end() - 1).value();
			if (days_map.count())
			{
				day = QString::number((days_map.end() - 1).key());
			}
		}
	}
// 	ui.label_title->setText(ui.lineEdit_title->text() + QStringLiteral("����") + month + QStringLiteral("��") + day + QStringLiteral("�յ�") + session
// 		+ QStringLiteral("���䱦ͳ��"));
	ui.label_title->setText(ui.lineEdit_title->text() + QStringLiteral("����") + month + QStringLiteral("��") + day + QStringLiteral("��") 
		+ QStringLiteral("�䱦ͳ��"));

	QVector<QPair<QString, int>> taboo_nums_v;
	QVector<QPair<QString, int>> desolate_nums_v;
	for (auto it = desolate_nums_.begin(); it != desolate_nums_.end(); ++it)
	{
		QString goods = it.key();
		//if (goods == QStringLiteral("��ѻ") || goods == QStringLiteral("�۷�") || goods == QStringLiteral("��"))
		if (QSetting::ins().gnoset_.indexOf(goods) != -1)
		{
			taboo_nums_v.push_back(QPair<QString, int>(it.key(), it.value()));
		}
		else
		{
			desolate_nums_v.push_back(QPair<QString, int>(it.key(), it.value()));
		}
	}
	qSort(desolate_nums_v.begin(), desolate_nums_v.end(), myCmpare);
	int rowStep = 0;
	for (; rowStep < desolate_nums_v.count(); ++rowStep)
	{
		QString goods = desolate_nums_v[rowStep].first;
		int sessions = desolate_nums_v[rowStep].second;
		int des_day = desolate_day_nums_[goods];
		QString sessions_str = QString::number(sessions);
		QTableWidgetItem* name_item = new QTableWidgetItem(goods);
		QTableWidgetItem* days_item = new QTableWidgetItem(QString::number(des_day) + QStringLiteral("��"));
		QProgressWidget * session_item = new QProgressWidget(ui.tableWidget_totol);

		session_item->SetText(goods, sessions_str, totol_days * 3);
		name_item->setTextAlignment(Qt::AlignCenter);
		days_item->setTextAlignment(Qt::AlignCenter);

		name_item->setTextColor(QColor(246, 172, 152));
		days_item->setTextColor(QColor(246, 172, 152));

		ui.tableWidget_totol->setItem(rowStep, 0, name_item);
		ui.tableWidget_totol->setCellWidget(rowStep, 1, session_item);
		ui.tableWidget_totol->setItem(rowStep, 2, days_item);
	}
	for (int i = 0; i < taboo_nums_v.count(); ++i, ++rowStep)
	{
		QString goods = taboo_nums_v[i].first;
		int sessions = 0;
		QString sessions_str = QString::number(sessions);
		QTableWidgetItem* name_item = new QTableWidgetItem(goods);
		QTableWidgetItem* days_item = new QTableWidgetItem(QStringLiteral("��"));
		QProgressWidget * session_item = new QProgressWidget(ui.tableWidget_totol);

		//session_item->SetText(goods, sessions_str, totol_days * 3);
		name_item->setTextAlignment(Qt::AlignCenter);
		days_item->setTextAlignment(Qt::AlignCenter);

		name_item->setTextColor(QColor(246, 172, 152));
		days_item->setTextColor(QColor(246, 172, 152));

		ui.tableWidget_totol->setItem(rowStep, 0, name_item);
		ui.tableWidget_totol->setCellWidget(rowStep, 1, session_item);
		ui.tableWidget_totol->setItem(rowStep, 2, days_item);
	}

}

