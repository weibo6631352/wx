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
 		//�ӵ㣨100��150����ʼ���㣨300��150��������ȷ��һ��ֱ��
 		linearGradient.setColorAt(0, QColor(97,194,129));
 		linearGradient.setColorAt(1, QColor(222, 246, 211));
 		//��ֱ�߿�ʼ����Ϊ0���յ���Ϊ1��Ȼ��ֶ�������ɫ
 		painter.setBrush(linearGradient);
 		painter.setPen(QColor(255, 255, 255, 0));
		painter.drawRect(0, isize.height()*0.1, isize.width()*num, isize.height()*0.9);
 	}
 
 	// ���û�����ɫ
 	painter.setPen(QPen());
 
 	// ��������Ϊ��ǰ������������򣨾��У�
 	painter.drawText(rect(), Qt::AlignCenter, text_);

}

