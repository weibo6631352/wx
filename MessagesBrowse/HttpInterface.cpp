#include "HttpInterface.h"
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QEventLoop>


QByteArray HttpInterface::HttpGet(const QString &url)
{
	// 不确定reply是否需要释放。
	QNetworkAccessManager proc_manager;
	QNetworkReply *reply = proc_manager.get(QNetworkRequest(QUrl(url)));
	QByteArray responseData;
	QEventLoop eventLoop;
	QObject::connect(&proc_manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
	eventLoop.exec();
	return reply->readAll();
}

QImage HttpInterface::Bytes2Image(QByteArray &bytes)
{
	QImage img;
	img.loadFromData(bytes);
	return img;
}

QString HttpInterface::Bytes2String(QByteArray &bytes)
{
	QString res = QString::fromUtf8(bytes);
	if (res.startsWith(QStringLiteral("<!DOCTYPE HTML PUBLIC")))
		return QString();
	return QString::fromUtf8(bytes);
}
