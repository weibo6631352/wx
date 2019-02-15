#include "QWxLogin.h"
#include <QMessageBox>
#include <QFile>

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
QWxLogin::QWxLogin(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
// 	QPalette palette;
// 	palette.setBrush(QPalette::Window, QBrush(QColor(232, 241, 252)));
// 	setPalette(palette);

	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/viewport.qss"));
	ui.dateEdit_setdate->setDate(QDate::currentDate());

	
	//场次控件
	ui.lineEdit_setsession->setValidator(new QIntValidator(0, 100, this));

	//QTime cur = QTime::currentTime();
	//if (cur > QTime(10, 40, 0) && cur < QTime(16, 40, 0))
	//	ui.lineEdit_setsession->setText(QStringLiteral("1"));
	//else if (cur > QTime(16, 40, 0) && cur < QTime(23, 40, 0))
	//	ui.lineEdit_setsession->setText(QStringLiteral("2"));
	//else if (cur > QTime(23, 40, 0) || cur < QTime(10, 40, 0))
	//	ui.lineEdit_setsession->setText(QStringLiteral("3"));
	//else
	//	ui.lineEdit_setsession->setText(QStringLiteral(""));
}

QWxLogin::~QWxLogin()
{
}

void QWxLogin::accept()
{
	date_ = QString::number(ui.dateEdit_setdate->date().year()) + QStringLiteral("-");
	date_ = date_ + QString::number(ui.dateEdit_setdate->date().month()) + QStringLiteral("-");
	date_ = date_ + QString::number(ui.dateEdit_setdate->date().day()) ;

	session_ = ui.lineEdit_setsession->text();

	if (!session_.isEmpty())
	{
		QDialog::accept();
	}
	else
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("标题或者场次为空"), 0);
	}
}
