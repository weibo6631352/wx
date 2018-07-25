#include "QProgressWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include "QSetting.h"


QProgressWidget::QProgressWidget(QWidget *parent)
	: QWidget(parent),
	totol_(0)
{
}

QProgressWidget::~QProgressWidget()
{
	
}

void QProgressWidget::SetText(QString goods, QString text, int totol)
{
	text_ = text;
	totol_ = totol;
	goods_ = goods;
}

QString QProgressWidget::GetText()
{
	return text_;
}

int QProgressWidget::GetNum()
{
	return text_.toInt();
}

void QProgressWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
 	QSize isize = size();
 	int goods_num = text_.toInt();
 
 	
 	int totol = totol_;
 	float num = 0;
 	if (totol != 0)
 		num = goods_num / ((float)totol*1.3f);
 
 	if (goods_num)
 	{
 		QLinearGradient linearGradient(0, 0, isize.width()*num, 0);
 		//从点（100，150）开始到点（300，150）结束，确定一条直线
 		linearGradient.setColorAt(0, QColor(250,177,73));
 		linearGradient.setColorAt(1, QColor(255, 237, 193));
 		//将直线开始点设为0，终点设为1，然后分段设置颜色
 		painter.setBrush(linearGradient);
 		painter.setPen(QColor(255, 255, 255, 0));
 		painter.drawRect(0, isize.height()*0.1, isize.width()*num, isize.height()*0.9);
 	}
 
 	// 设置画笔颜色
 	painter.setPen(QPen());
 
 	// 绘制区域为当前界面的整个区域（居中）
	//if (goods_ == QStringLiteral("乌鸦") || goods_ == QStringLiteral("蜜蜂") || goods_ == QStringLiteral("狗"))
	if(QSetting::ins().gnoset_.indexOf(goods_) != -1)
	{
		painter.drawText(0, isize.height()*0.1, isize.width(), isize.height()*0.9, Qt::AlignCenter, QStringLiteral("无"));
	}
	else
	{
		painter.drawText(0, isize.height()*0.1, isize.width(), isize.height()*0.9, Qt::AlignCenter, text_);
	}
	
}

