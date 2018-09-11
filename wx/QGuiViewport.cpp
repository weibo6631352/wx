#include "QGuiViewport.h"
#include "QProgressWidget.h"
#include "QwxSetting.h"
#include "QGoodsData.h"
#include "QwxData.h"
#include "QGuiCharts.h"
#include "QwxLog.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QDateTime>
#include <QTextStream>
#include <QIntValidator>
#include <QSettings>
#include <QClipboard>

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

QGuiViewport::QGuiViewport(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
	ui.setupUi(this);
	InitUi();

	QObject::connect(ui.tabWidget, &QTabWidget::currentChanged, [&]() {
		HeightLightGoods(ui.lineEdit_serach->text());
	});

}

QGuiViewport::~QGuiViewport()
{
}

void QGuiViewport::InitUi()
{
	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/viewport.qss"));
	auto user_data = QwxData::ins()->user_data_;
	ui.tabWidget->insertTab(ui.tabWidget->count(), new QGuiCharts(QStringLiteral("总计")), QStringLiteral("总计"));
	for (auto it = user_data.begin(); it != user_data.end(); ++it)
	{
		QGoodsData* data = it.value();
		QGuiCharts *charts = new QGuiCharts(it.key());
		ui.tabWidget->insertTab(ui.tabWidget->count(), charts, it.key());
	}

	// 重宝功能
	QVector<QString> goods = QwxSetting::ins()->GetGoodsSet();
	ui.combobox_chongbao->addItem(QString());
	for (int i = 0; i < goods.size(); ++i)
	{
		ui.combobox_chongbao->addItem(goods[i]);
	}

	ui.tabWidget->setCurrentIndex(0);
	ui.tabWidget->setStyleSheet("QTabWidget:pane {border:1px solid #e8f3f9;background:  transparent; }");

}

void QGuiViewport::on_export()
{
	QString local = QwxSetting::ins()->title_;
	if (!QwxData::ins()->GetUserCount())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("无用户数据"), 0);
		return;
	}

	int curindex = ui.tabWidget->currentIndex();
 	

	QwxData *wxData = QwxData::ins();
	for (auto it = wxData->user_data_.begin(); it != wxData->user_data_.end(); ++it)
	{
		QGoodsData *data = it.value();
		QString exp_dir_path = QApplication::applicationDirPath() + QStringLiteral("/导出/");
		QString date_dir_path = exp_dir_path + data->date_ + QStringLiteral("/");
		QString local_path = date_dir_path + local + QStringLiteral("/");
		QString session_dir_path = local_path + data->session_ + QStringLiteral("场/");
		QString user_dir_path = session_dir_path + data->user_ + QStringLiteral("/");
		if (CreateDir(exp_dir_path))
		{
			if (CreateDir(date_dir_path))
			{
				if (CreateDir(local_path))
				{
					if (CreateDir(session_dir_path))
					{
						if (CreateDir(user_dir_path))
						{
							on_exportTotolPic(user_dir_path, data->user_);
							on_exportTotolTxt(user_dir_path, data->user_);
							on_exportLog(user_dir_path, data->user_);
						}
					}
				}
			}
		}
	}

	QGoodsData *data = QwxData::ins()->GetTotolData();
	QString exp_dir_path = QApplication::applicationDirPath() + QStringLiteral("/导出/");
	QString date_dir_path = exp_dir_path + data->date_ + QStringLiteral("/");
	QString local_path = date_dir_path + local + QStringLiteral("/");
	QString session_dir_path = local_path + data->session_ + QStringLiteral("场/");
	QString user_dir_path = session_dir_path + data->user_ + QStringLiteral("/");


	if (CreateDir(exp_dir_path))
	{
		if (CreateDir(date_dir_path))
		{
			if (CreateDir(local_path))
			{
				if (CreateDir(session_dir_path))
				{
					if (CreateDir(user_dir_path))
					{
						on_exportTotolPic(user_dir_path, data->user_);
						on_exportTotolTxt(user_dir_path, data->user_);
						on_exportLog(user_dir_path, data->user_);
					}
				}
			}
		}
	}

	ui.tabWidget->setCurrentIndex(curindex);
	QMessageBox::information(this, QStringLiteral("保存成功"), session_dir_path, 0);
}

void QGuiViewport::on_updateCharts()
{
	int curindex = ui.tabWidget->currentIndex();
	for (int i = 0;i < ui.tabWidget->count(); ++i)
	{
		ui.tabWidget->setCurrentIndex(i);

		QGuiCharts* chart = (QGuiCharts*)ui.tabWidget->widget(i);
		chart->UpdateCharts();
	}
	ui.tabWidget->setCurrentIndex(curindex);
}



void QGuiViewport::on_preGoods_change(const QString& goods)
{
	QwxSetting::ins()->SetPreGoods(goods);
	on_updateCharts();
}

void QGuiViewport::on_rendpic()
{
	QPixmap p(ui.tabWidget->currentWidget()->size());
	ui.tabWidget->currentWidget()->render(&p);
	QApplication::clipboard()->setPixmap(p);
}

void QGuiViewport::on_serach_change(const QString& goods)
{
	HeightLightGoods(goods);
}

int QGuiViewport::GetTabIndex(QString title)
{
	for (int i = 0;i < ui.tabWidget->count(); ++i)
	{
		if (ui.tabWidget->tabText(i) == title)
			return i;
	}
	return -1;
}

void QGuiViewport::on_exportTotolPic(QString path, QString user)
{
	int index = GetTabIndex(user);
	if (-1 != index)
	{
		ui.tabWidget->setCurrentIndex(index);
		QGuiCharts* chart = (QGuiCharts*)ui.tabWidget->widget(index);
// 		QRect chart_rc = chart->ui.widget_chart->geometry();
// 		QRect totol_rc = chart->ui.widget_winorlose->geometry();
// 		chart_rc = chart_rc.united(totol_rc);
		QPixmap pixmap(chart->size());

		chart->render(&pixmap, QPoint(0,0), QRegion(chart->geometry()));
		pixmap.save(path + QTime::currentTime().toString(QStringLiteral("hh-mm-ss")) + QStringLiteral("统计.jpg"));
	}
}

void QGuiViewport::on_exportTotolTxt(QString path, QString user)
{
	QGoodsData *data;
	if (user == QStringLiteral("总计"))
	{
		if (QwxData::ins()->GetUserCount())
		{
			data = QwxData::ins()->GetTotolData();
		}  
		else
		{
			return;
		}
	}
	else
	{
		data = QwxData::ins()->GetUserData(user);
	}


	QString erport_str;

	for (auto itt = data->goods_totols_.begin(); itt != data->goods_totols_.end(); ++itt)
	{
		QString goods = itt.key();
		QString num_str = QString::number(itt.value());
		erport_str = erport_str + goods + QStringLiteral(" ") + num_str + QStringLiteral("\n");
	}


	QString file_path = path + QTime::currentTime().toString(QStringLiteral("hh-mm-ss")) +  QStringLiteral("统计.txt");
	QFile file(file_path);

	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("打开文件失败:") + file_path, 0);
		return;
	}

	QTextStream in(&file);
	in.setCodec("UTF-8"); //请注意这行

	in << erport_str;
	file.close();
}

void QGuiViewport::on_exportLog(QString path, QString user)
{
	QwxLog *wxLog = QwxLog::ins();

	QVector<QVector<QString>> data ;
	if (user == QStringLiteral("总计"))
	{
		if (QwxData::ins()->GetUserCount())
		{
			data = wxLog->GetTotolLog();
		}
		else
		{
			return;
		}
	}
	else
	{
		data = wxLog->GetUserLog(user);
	}


	QString file_path = path + QTime::currentTime().toString(QStringLiteral("hh-mm-ss")) + QStringLiteral("录入日志.csv");
	QFile file(file_path);

	if (!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("打开文件失败:") + file_path, 0);
		return;
	}


	QTextStream in(&file);
	//in.setCodec("UTF-8"); //请注意这行
	QString log;
	for (int i = 0; i < data.size(); ++i)
	{
		log = log + data[i][0] + QStringLiteral(",") + data[i][1] + QStringLiteral(",") + data[i][2] ;
		if (data[i].size() == 4)
			log = log + QStringLiteral(",") + data[i][3];
		log + QStringLiteral("\r\n");
	}
	in << log;
	file.close();
}

bool QGuiViewport::CreateDir(QString fullPath)
{
	QDir dir(fullPath);
	if (!dir.exists())
	{
		bool ok = dir.mkdir(fullPath);//只创建一级子目录，即必须保证上级目录存在
		if (!ok)
		{
			QMessageBox::information(this, QStringLiteral(""), QStringLiteral("创建文件夹失败：") + fullPath, 0);
			return false;
		}
	}
	return true;
}

void QGuiViewport::SetCurrentIndex(int index)
{
	if(ui.tabWidget->count())
		ui.tabWidget->setCurrentIndex(index);
}

QString QGuiViewport::GetPreGoods()
{
	return ui.combobox_chongbao->currentText();
}

void QGuiViewport::HeightLightGoods(QString goods )
{
	QGuiCharts* guiCharts = (QGuiCharts*)ui.tabWidget->currentWidget();
	guiCharts->on_heightlight_Goods(goods);
}
