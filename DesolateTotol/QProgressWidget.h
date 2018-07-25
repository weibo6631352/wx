#pragma once

#include <QWidget>
#include <QLabel>

class QProgressWidget : public QWidget
{
	Q_OBJECT

public:
	QProgressWidget(QWidget *parent = Q_NULLPTR);
	~QProgressWidget();

	void SetText(QString goods, QString text, int totol);
	QString GetText();

	int GetNum();
protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	QString text_;
	int totol_;
	QString goods_;
};
