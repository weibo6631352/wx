#include "QSetting.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

QSetting::QSetting()
{
	QString app_path = QApplication::applicationDirPath();
	QFile f;
	QString lineStr;
	int rowCount = 0;

	//宝物
	f.setFileName(app_path + QStringLiteral("/宝物.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			QStringList str_list = lineStr.split(QStringLiteral("="));
			if (str_list.size() == 2)
			{
				gset_.push_back(str_list[0]);
			}
		}
		f.close();
	}
	else
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("宝物.ini打开失败!"));

	//宝物_遇到不开
	f.setFileName(app_path + QStringLiteral("/宝物_遇到不开.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			gnoset_.push_back(lineStr);
		}
		f.close();
	}
	else
		QMessageBox::warning(nullptr, QStringLiteral("警告"), QStringLiteral("宝物_遇到不开.ini打开失败!"));
}

QSetting &QSetting::ins()
{
	static QSetting setting;
	return setting;
}

QSetting::~QSetting()
{
}
