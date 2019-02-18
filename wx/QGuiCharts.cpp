#include "QGuiCharts.h"
#include "QProgressWidget.h"
#include "QwxSetting.h"
#include "QGoodsData.h"
#include "QwxData.h"
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QTextCodec>
#include <QResizeEvent>
#include <QIntValidator>

QGuiCharts::QGuiCharts(QString user, QWidget *parent)
	: QWidget(parent),
	user_(user)
{
	ui.setupUi(this);

	QwxSetting *wxSetting = QwxSetting::ins();

	QwxData *wxdata = QwxData::ins();
	QGoodsData* goods_data;
	if (user_ == QStringLiteral("总计"))
	{
		goods_data = wxdata->GetTotolData();
	}
	else
	{
		goods_data = wxdata->GetUserData(user_);
	}
	
	QString session = QStringLiteral("第")+goods_data->session_ + QStringLiteral("场");
 	ui.label_date->setText(goods_data->date_);
	ui.label_data_session->setText(session);
	ui.label_title->setText(QwxSetting::ins()->title_ + QStringLiteral("（") + goods_data->user_ + QStringLiteral("）"));
	
// 	ui.label_data->setText();
	//初始化viewport
	const QVector<QString>& goods_v = wxSetting->GetGoodsSet();
	int isize = goods_v.size();
	QTableWidgetItem *goods_item, *num_item, *winorlose = nullptr;
	for (int i = 0; i < isize; ++i)
	{
		QTableWidgetItem * goods_item = new QTableWidgetItem(goods_v[i]);
		QTableWidgetItem * yabao_limit_item = new QTableWidgetItem(QStringLiteral(""));
		QTableWidgetItem * yabaoshuying_limit_item = new QTableWidgetItem(QStringLiteral(""));
		QTableWidgetItem * winorlose = new QTableWidgetItem(QStringLiteral(""));
		QProgressWidget *num_item = new QProgressWidget(ui.label_yabaolimit,this);


		goods_item->setFlags(Qt::ItemIsEnabled);
		//num_item->setFlags(Qt::ItemIsEnabled);
		winorlose->setFlags(Qt::ItemIsEnabled);
		yabao_limit_item->setFlags(Qt::ItemIsEnabled);
		yabaoshuying_limit_item->setFlags(Qt::ItemIsEnabled);
		goods_item->setTextAlignment(Qt::AlignCenter);
		winorlose->setTextAlignment(Qt::AlignCenter);
		yabao_limit_item->setTextAlignment(Qt::AlignCenter);
		yabao_limit_item->setForeground(QBrush(QColor(255, 0, 0)));
		yabaoshuying_limit_item->setTextAlignment(Qt::AlignCenter);
		yabaoshuying_limit_item->setForeground(QBrush(QColor(255, 0, 0)));

		ui.tableWidget_totol->setItem(i, 0, goods_item);
		//		ui.tableWidget_totol->setItem(i, 1, num_item);
		ui.tableWidget_totol->setCellWidget(i, 1, num_item);
		ui.tableWidget_winorlose->setItem(i, 0, winorlose);
		ui.tableWidget_yabaolimit->setItem(i, 0, yabao_limit_item);
		ui.tableWidget_yabaoshuyinglimit->setItem(i, 0, yabaoshuying_limit_item);
	}


	
	InitStyle();
	//填数据
	ui.label_yabaolimit->setValidator(new QIntValidator(ui.label_yabaolimit));
	connect(ui.label_yabaolimit, &QLineEdit::textChanged, [this]() {UpdateCharts();});
	ui.label_yabaoshuyinglimit->setValidator(new QIntValidator(ui.label_yabaoshuyinglimit));
	connect(ui.label_yabaoshuyinglimit, &QLineEdit::textChanged, [this]() {UpdateCharts(); });
	UpdateCharts();
}

QGuiCharts::~QGuiCharts()
{
}


void QGuiCharts::InitStyle()
{
	//背景图片


	//setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));
	ui.tableWidget_totol->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));
	ui.tableWidget_winorlose->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));
	ui.tableWidget_yabaolimit->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));
	ui.tableWidget_yabaoshuyinglimit->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));

	QImage _image;
	_image.load(QApplication::applicationDirPath() + (QStringLiteral("/bg.jpg")));
	setAutoFillBackground(true);   // 这个属性一定要设置
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
	setPalette(pal);
}

void QGuiCharts::resizeEvent(QResizeEvent *event)
{
 	QWidget::resizeEvent(event);
}

struct goods_info
{
	goods_info() {};
	goods_info(QString _goods, int _num, int _winorlose)
	{
		goods = _goods;
		num = _num;
		winorlose = _winorlose;
	};
	QString goods;
	int num;
	int winorlose;
};
bool winlessCmpare(goods_info& p1, goods_info  &p2)
{
	return p1.winorlose < p2.winorlose;
}

// 按输赢预测排序
void QGuiCharts::UpdateCharts()
{
	
	QString pregoods = QwxSetting::ins()->GetPreGoods();
	QGoodsData *data;
	const QVector<QString> good_set = QwxSetting::ins()->GetGoodsSet();
	const QMap<QString, double> goods_profit = QwxSetting::ins()->GetGoodsProfitMap();
	int yabao_limit = ui.label_yabaolimit->text().toInt();
	int yabaos_shuyinglimit = ui.label_yabaoshuyinglimit->text().toInt();
	ui.label_yabaolimit->setText(QString::number(yabao_limit));

	if (user_ == QStringLiteral("总计"))
	{
		ui.label_title->setText(QwxSetting::ins()->title_ + QStringLiteral("（") + user_ + QStringLiteral("）"));
		if (!QwxData::ins()->GetUserCount())
		{
			return;
		}
		data = QwxData::ins()->GetTotolData();
		data->setParent(this);
	}
	else
	{
		data = QwxData::ins()->GetUserData(user_);
	}


	ui.label_total->setText(QString::number(data->totols_));


	QVector<goods_info> sort_goods;

	for (QMap<QString, int>::iterator iter = data->goods_totols_.begin(); iter != data->goods_totols_.end(); ++iter)
	{
		QString goods = iter.key();
		int goods_num = iter.value();
		int winorlose_num = 0;
		double gfit = goods_profit[goods];
		if (goods == pregoods)
			gfit = 90;
		// 遇到总计
		if (user_ == QStringLiteral("总计"))
		{
			QMap<QString, QGoodsData*> user_data = QwxData::ins()->user_data_;
			for (auto it = user_data.begin(); it != user_data.end(); ++it)
			{
				QGoodsData *userdata = it.value();
				winorlose_num += qRound(userdata->totols_*(1.0 - userdata->profit_*0.01)) - qRound(userdata->goods_totols_[goods] * gfit);
			}
		}
		else
		{
			winorlose_num = qRound(data->totols_*(1.0 - data->profit_*0.01)) - qRound(goods_num * gfit );
		}
		sort_goods.push_back(goods_info(goods, goods_num, winorlose_num));
	}
	qSort(sort_goods.begin(), sort_goods.end(), winlessCmpare);



	// 绘制每一个物品
	for (int i = 0; i < sort_goods.size(); ++i)
	{
		QString goods = sort_goods[i].goods;
		double gfit = goods_profit[goods];
		int goods_num = sort_goods[i].num;
		int winorlose_num = sort_goods[i].winorlose;
		int goods_shuying_num = winorlose_num;

		QTableWidgetItem * goods_item = ui.tableWidget_totol->item(i, 0);
		QProgressWidget * num_item = (QProgressWidget*)ui.tableWidget_totol->cellWidget(i, 1);
		QTableWidgetItem * winorlose = ui.tableWidget_winorlose->item(i, 0);
		QTableWidgetItem * yabao_limit_item = ui.tableWidget_yabaolimit->item(i, 0);
		QTableWidgetItem * yabao_shuyinglimit_item = ui.tableWidget_yabaoshuyinglimit->item(i, 0);

		if (winorlose_num < 0)
			winorlose->setForeground(QBrush(QColor(255, 0, 0)));
		else
			winorlose->setForeground(QBrush(QColor(0, 0, 0)));


		goods_item->setText(goods);
		num_item->SetText(QString::number(goods_num));
		winorlose->setText(QString::number(winorlose_num));

		int yabao_overtop = goods_num - yabao_limit;
		if (yabao_overtop > 0)
		{
			yabao_limit_item->setText(QString::number(yabao_overtop));
		}
		else
		{
			yabao_limit_item->setText(QStringLiteral(""));
		}
		int yabao_shuyingovertop = goods_shuying_num + yabaos_shuyinglimit;
		if (yabao_shuyingovertop < 0)
		{
			//yabao_shuyinglimit_item->setText(QString::number((int)(data->totols_*(1.0 - data->profit_*0.01) - (yabao_shuyingovertop)) / (int)gfit));
			double A = std::abs(yabao_shuyingovertop / gfit);
			yabao_shuyinglimit_item->setText(QString::number((int)(A+A/gfit)));
		}
		else
		{
			yabao_shuyinglimit_item->setText(QStringLiteral(""));
		}
	}
	update();
}

void QGuiCharts::on_heightlight_Goods(QString goods)
{
	for (int i = 0; i < ui.tableWidget_totol->rowCount(); ++i)
	{
		if (ui.tableWidget_totol->item(i, 0)->text() == goods)
		{
			ui.tableWidget_totol->item(i, 0)->setBackgroundColor(QColor(255, 0, 0));
		}
		else
		{
			ui.tableWidget_totol->item(i, 0)->setBackgroundColor(QColor(0, 0, 0, 0));
		}
	}
}
