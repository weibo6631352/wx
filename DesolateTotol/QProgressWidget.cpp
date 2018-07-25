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
 		//�ӵ㣨100��150����ʼ���㣨300��150��������ȷ��һ��ֱ��
 		linearGradient.setColorAt(0, QColor(250,177,73));
 		linearGradient.setColorAt(1, QColor(255, 237, 193));
 		//��ֱ�߿�ʼ����Ϊ0���յ���Ϊ1��Ȼ��ֶ�������ɫ
 		painter.setBrush(linearGradient);
 		painter.setPen(QColor(255, 255, 255, 0));
 		painter.drawRect(0, isize.height()*0.1, isize.width()*num, isize.height()*0.9);
 	}
 
 	// ���û�����ɫ
 	painter.setPen(QPen());
 
 	// ��������Ϊ��ǰ������������򣨾��У�
	//if (goods_ == QStringLiteral("��ѻ") || goods_ == QStringLiteral("�۷�") || goods_ == QStringLiteral("��"))
	if(QSetting::ins().gnoset_.indexOf(goods_) != -1)
	{
		painter.drawText(0, isize.height()*0.1, isize.width(), isize.height()*0.9, Qt::AlignCenter, QStringLiteral("��"));
	}
	else
	{
		painter.drawText(0, isize.height()*0.1, isize.width(), isize.height()*0.9, Qt::AlignCenter, text_);
	}
	
}

