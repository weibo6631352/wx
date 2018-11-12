#include "jiqima.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	jiqima w;
	w.show();
	return a.exec();
}
