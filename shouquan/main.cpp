#include "shouquan.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	shouquan w;
	w.show();
	return a.exec();
}
