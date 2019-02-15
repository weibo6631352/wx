#pragma once

#include <QImage>
class HttpInterface
{
public:
	static QByteArray HttpGet(const QString &url);
	static QImage Bytes2Image(QByteArray &bytes);
	static QString Bytes2String(QByteArray &bytes);
};

