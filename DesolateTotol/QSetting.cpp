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

	//����
	f.setFileName(app_path + QStringLiteral("/����.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������
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
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("����.ini��ʧ��!"));

	//����_��������
	f.setFileName(app_path + QStringLiteral("/����_��������.ini"));
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //��ע������
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();
			gnoset_.push_back(lineStr);
		}
		f.close();
	}
	else
		QMessageBox::warning(nullptr, QStringLiteral("����"), QStringLiteral("����_��������.ini��ʧ��!"));
}

QSetting &QSetting::ins()
{
	static QSetting setting;
	return setting;
}

QSetting::~QSetting()
{
}
