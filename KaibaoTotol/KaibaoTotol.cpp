#include "KaibaoTotol.h"
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QApplication>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>


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

KaibaoTotol::KaibaoTotol(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/viewport.qss"));

	ui.tableWidget_totol->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


	ui.tableWidget_totol->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));

	QImage _image;
	_image.load(QApplication::applicationDirPath() + (QStringLiteral("/desolateTotolbg.jpg")));
	ui.widget_chart->setAutoFillBackground(true);   // �������һ��Ҫ����
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	ui.widget_chart->setPalette(pal);

	
}

void KaibaoTotol::setfilepath()
{

	QString relative_fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open File"), QApplication::applicationDirPath(), QStringLiteral("txt (*.txt *.dat)"));
	if (relative_fileName.isEmpty())
	{
		return;
	}

	ui.pushButton_export->setEnabled(true);
	totol_goods_.clear();

	QFileInfo fi(relative_fileName);
	QString filePath = fi.absoluteFilePath();
	QString fileName = fi.baseName();
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
				//				totol_goods_[cur_month] = QMap<int ,QVector<QString>>();

			}
			else if (-1 != lineStr.indexOf(QStringLiteral("��")))
			{
				cur_month = lineStr.split(QStringLiteral("��"))[0].toInt();
				//				totol_goods_[cur_month] = QMap<int ,QVector<QString>>();

			}
			else if (-1 != lineStr.indexOf(QStringLiteral("��")))
			{
				QStringList strList = lineStr.split(QStringLiteral("��"));
				int cur_day = strList[0].toInt();
				QStringList good_list = strList[1].split(QStringLiteral(","));
				QVector<QString> goods_v;
				for (int i = 0; i < good_list.count(); ++i)
				{
					goods_v.push_back(good_list[i]);
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

	
	InitYearMonth();
}

void KaibaoTotol::InitYearMonth()
{
	ui.comboBox_month->clear();
	ui.comboBox_year->clear();
	for (auto ii = totol_goods_.begin(); ii != totol_goods_.end(); ++ii)
	{
		ui.comboBox_year->addItem(QString::number(ii.key()));
	}
	
}

void KaibaoTotol::InitMonth()
{
	ui.comboBox_month->clear();
	QString year = ui.comboBox_year->currentText();
	int iyear = year.toInt();

	for (auto iii = totol_goods_[iyear].begin(); iii != totol_goods_[iyear].end(); ++iii)
	{
		ui.comboBox_month->addItem(QString::number(iii.key()));
	}

}

void KaibaoTotol::on_export()
{
	QFileInfo fi(ui.lineEdit_filepath->text());
	if (!fi.exists())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("�༭�����·������ȷ"), 0);
		return;
	}
	QString fileDir = fi.absolutePath();
	QString fileName = fi.baseName();

	QPixmap pixmap(ui.widget_chart->size());
	ui.widget_chart->render(&pixmap);
	pixmap.save(fileDir + (QStringLiteral("/")) + fileName + QStringLiteral("����ͳ��.jpg"));

	QMessageBox::information(this, QStringLiteral("����ɹ�"), fileDir + (QStringLiteral("/")) + fileName + QStringLiteral("����ͳ��.jpg"), 0);
}

void KaibaoTotol::on_year_change(QString year)
{
	if (year.isEmpty())
	{
		return;
	}
	InitMonth();
}

void KaibaoTotol::on_month_change(QString mon)
{
	if (mon.isEmpty())
	{
		return;
	}
	UpdateKaibaoView();
}

void KaibaoTotol::UpdateKaibaoView()
{
	int year = ui.comboBox_year->currentText().toInt();
	int mon = ui.comboBox_month->currentText().toInt();
	int days = GetMouthDays();
	ui.tableWidget_totol->clear();
	ui.tableWidget_totol->setRowCount(days);

	ui.label_title->setText(ui.lineEdit_title->text() + QString::number(mon) + QStringLiteral("�·ݿ�����¼"));
	ui.label_year->setText(QString::number(year) + QStringLiteral("��"));

	
	for (int day_step = 0; day_step <= days; ++day_step)
	{
		auto iitem = new QTableWidgetItem(QString::number(mon) + QStringLiteral("��") + QString::number(day_step + 1) + QStringLiteral("��"));
		iitem->setTextAlignment(Qt::AlignCenter);
		iitem->setTextColor(QColor(246, 172, 152));
		ui.tableWidget_totol->setItem(day_step, 0, iitem);
	}

	auto &month_data = totol_goods_[year][mon];
	for (auto &it = month_data.begin(); it != month_data.end(); ++it)
	{
		int cur_day = it.key();
		int cur_row = cur_day - 1;
		auto &day_sesson = it.value();
		if (cur_day > days)
		{
			QMessageBox::information(this, QStringLiteral(""), QStringLiteral("�Ѻ������ƴ�������ڣ�")+QString::number(mon) + QStringLiteral("��") + QString::number(it.key()) + QStringLiteral("��"));
			continue;
		}
		for (int i = 0; i < day_sesson.count() && i < 3; ++i)
		{
			auto iitem = new QTableWidgetItem(day_sesson[i]);
			iitem->setTextAlignment(Qt::AlignCenter);
			iitem->setTextColor(QColor(246, 172, 152));
			ui.tableWidget_totol->setItem(cur_row, i+1, iitem);
		}
	}
}

int KaibaoTotol::GetMouthDays()
{
	int year = ui.comboBox_year->currentText().toInt();
	int mon = ui.comboBox_month->currentText().toInt();
	
	if (mon == 2)//�ж����Ҫ��== 
		if (year % 4 == 0)
			if (year % 100 == 0)
				if (year % 400 == 0)
					return  29;//�⼸���ط��ú������ĸ���28���ĸ���29
				else return 28 ;
			else  29 ;
		else return 28 ;
	else
		switch (mon)
		{
		case 1:case 3:case 5:case 7:case 8:case 10:case 12:
			return 31 ;
			break;
		case 4:case 6:case 9:case 11:
			return 30 ;
			break;
		default: return 0;
			break;
		}
	return 0;
}