#include "KaibaoTotol.h"
#include <QtWidgets/QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	if (/*ReadKey(QApplication::applicationDirPath() + QStringLiteral("/desolate.key")) == GetHDMd5()*/1)
	{
		KaibaoTotol w;
		w.show();
		return a.exec();
	}
	else
	{
		QMessageBox::warning(0, QStringLiteral(""), QStringLiteral("��Ȩʧ�ܣ�����ϵ����Ա��"));
	}
}
