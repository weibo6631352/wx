#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
class QProgressWidget : public QWidget
{
	Q_OBJECT

public:
	QProgressWidget(QLineEdit *limit_edit, QWidget *parent = Q_NULLPTR);
	~QProgressWidget();

	void SetText(QString text);
	QString GetText();

	int GetNum();
protected:
	virtual void paintEvent(QPaintEvent *event);

private:
	QString text_;
	QLineEdit *limit_edit_;
};
