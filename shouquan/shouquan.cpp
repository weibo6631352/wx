#include "shouquan.h"
#include <SHA1.h>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

shouquan::shouquan(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void shouquan::onclick()
{
	QString maction = ui.lineEdit->text();
	CSHA1 sha1;
	sha1.Update((unsigned char*)maction.toLocal8Bit().data(), strlen(maction.toLocal8Bit().data()));
	sha1.Final();
	unsigned char chSha1[20] = "";
	sha1.GetHash(chSha1);


	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save file"),
		"",
		tr("*.key")); //Ñ¡ÔñÂ·¾¶
	if (filename.isEmpty())
	{
		return;
	}
	else
	{

		QFile file(filename);
		if (!file.open(QIODevice::WriteOnly))
		{
			return;
		}
		QDataStream out(&file);
		out.setVersion(QDataStream::Qt_5_11);
		for (int i = 0; i < 20; ++i)
		{
			out << chSha1[i];
		}
		file.close();
	}
}
