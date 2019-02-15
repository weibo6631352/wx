#include "Login.h"
#include "HttpInterface.h"
#include <QImage>
#include <QPixmap>

Login::Login(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	QImage img = HttpInterface::Bytes2Image(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/WXGetQR")));
	ui.label_qr->setPixmap(QPixmap::fromImage(img));
	ui.label_qr->setScaledContents(true);
}

Login::~Login()
{
}

void Login::Verify()
{
	QString res = HttpInterface::Bytes2String(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/WXConfirmQR")));
	if (res == QStringLiteral("succeed"))
		nickname_ = HttpInterface::Bytes2String(HttpInterface::HttpGet(QStringLiteral("http://45.78.75.226:5000/username")));
		close();
}
