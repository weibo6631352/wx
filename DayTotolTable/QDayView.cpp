#include "QDayView.h"
#include <QStyleOption>
#include <QPainter>

QDayView::QDayView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	table = ui.tableWidget_day_totol;



	
	table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->setStyleSheet(QStringLiteral("background-color:rgba(0,0,0,0)"));
	table->setStyleSheet("QTableWidget{ gridline-color : rgb(170, 170, 170)}");


	QPalette pll = table->palette();
	pll.setBrush(QPalette::Base, QBrush(QColor(255, 255, 255, 0)));
	table->setPalette(pll);

	QImage _image;
	_image.load(QApplication::applicationDirPath() + (QStringLiteral("/日结统计背景.jpg")));
	setAutoFillBackground(true);   // 这个属性一定要设置
	QPalette pal(palette());
	pal.setBrush(QPalette::Window, QBrush(_image.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
	setPalette(pal);
}

void QDayView::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

QDayView::~QDayView()
{
}

QTableWidget* QDayView::getTable()
{
	return table;
}