#include "wx.h"
#include <QtWidgets/QApplication>
#include <QDate>
#include <QFile>
#include <QMessageBox>
#include "QWxLogin.h"
#include "QwxSetting.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QWxLogin login;
	if (login.exec() == QDialog::Accepted)
	{
		QwxSetting * wx_setting = QwxSetting::ins();
		if (wx_setting->ReadIni())
		{
			wx_setting->date_ = login.date_;
			wx_setting->title_ = login.title_;
			wx_setting->session_ = login.session_;
			wx w;
			w.show();
			return a.exec();
		}
	}

	return 0;
}
