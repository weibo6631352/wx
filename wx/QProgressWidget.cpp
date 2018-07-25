#include "QProgressWidget.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QwxSetting.h>

QProgressWidget::QProgressWidget(QLineEdit *limit_edit, QWidget *parent)
	: QWidget(parent)
	, limit_edit_(limit_edit)
{
}

QProgressWidget::~QProgressWidget()
{
	
}

void QProgressWidget::SetText(QString text)
{
	text_ = text;
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
	int yabaoLimit = limit_edit_->text().toInt();
 
 	

 	float num = goods_num / (float)yabaoLimit;
 
 	if (goods_num)
 	{
 		QLinearGradient linearGradient(0, 0, isize.width()*num, 0);
 		//从点（100，150）开始到点（300，150）结束，确定一条直线
 		linearGradient.setColorAt(0, QColor(97,194,129));
 		linearGradient.setColorAt(1, QColor(222, 246, 211));
 		//将直线开始点设为0，终点设为1，然后分段设置颜色
 		painter.setBrush(linearGradient);
 		painter.setPen(QColor(255, 255, 255, 0));
		painter.drawRect(0, isize.height()*0.1, isize.width()*num, isize.height()*0.9);
 	}
 
 	// 设置画笔颜色
 	painter.setPen(QPen());
 
 	// 绘制区域为当前界面的整个区域（居中）
 	painter.drawText(rect(), Qt::AlignCenter, text_);

}

