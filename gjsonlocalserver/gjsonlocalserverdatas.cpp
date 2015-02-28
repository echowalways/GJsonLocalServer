#include "gjsonlocalserverdatas.h"

#include <QtCore/QLoggingCategory>

#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

Q_LOGGING_CATEGORY(qlcJsonLocalServerDatas, "GJsonLocalServer.datas")

// class GJsonLocalMethod

GJsonLocalMethod::GJsonLocalMethod()
    : d(0)
{
}

GJsonLocalMethod::~GJsonLocalMethod()
{
}

GJsonLocalServer *GJsonLocalMethod::server() const
{
    return reinterpret_cast<GJsonLocalServer *>(d);
}

// class GJsonLocalRequestPrivate

class GJsonLocalRequestPrivate : public QSharedData
{
public:
    QString method;
    QJsonValue value;
};

// class GJsonLocalRequest

GJsonLocalRequest::GJsonLocalRequest(const QByteArray &chunk)
    : d(new GJsonLocalRequestPrivate())
{
    QJsonDocument doc = QJsonDocument::fromBinaryData(chunk);
    if (!doc.isObject()) {
        qCDebug(qlcJsonLocalServerDatas, "Invalid chunk.");
        return;
    }

    QJsonObject obj = doc.object();
    d->method = obj.value(QLatin1Literal("method")).toString();
    if (d->method.isEmpty()) {
        qCDebug(qlcJsonLocalServerDatas, "Invalid method name.");
        return;
    }

    d->value = obj.value(QLatin1Literal("value"));
}

GJsonLocalRequest::GJsonLocalRequest(const GJsonLocalRequest &rhs)
    : d(rhs.d)
{
}

GJsonLocalRequest &GJsonLocalRequest::operator=(const GJsonLocalRequest &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

GJsonLocalRequest::~GJsonLocalRequest()
{
}

bool GJsonLocalRequest::isValid() const
{
    return !d->method.isEmpty();
}

QString GJsonLocalRequest::method() const
{
    return d->method;
}

QJsonValue GJsonLocalRequest::value() const
{
    return d->value;
}

// class GJsonLocalResponsePrivate

class GJsonLocalResponsePrivate : public QSharedData
{
public:
    GJsonLocalResponsePrivate()
        : statusCode(-1)
    {
    }

public:
    int statusCode;
    QJsonValue value;
};

// class GJsonLocalResponse

GJsonLocalResponse::GJsonLocalResponse()
    : d(new GJsonLocalResponsePrivate())
{
}

GJsonLocalResponse::GJsonLocalResponse(const GJsonLocalResponse &rhs)
    : d(rhs.d)
{
}

GJsonLocalResponse &GJsonLocalResponse::operator=(const GJsonLocalResponse &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

GJsonLocalResponse::~GJsonLocalResponse()
{
}

bool GJsonLocalResponse::isValid() const
{
    return d->statusCode != -1;
}

QByteArray GJsonLocalResponse::chunk() const
{
    if (d->statusCode == -1) {
        return QByteArray();
    }

    QJsonObject obj;
    obj.insert(QLatin1Literal("statusCode"), d->statusCode);
    if (!d->value.isUndefined()) {
        obj.insert(QLatin1Literal("value"), d->value);
    }
    return QJsonDocument(obj).toBinaryData();
}

void GJsonLocalResponse::setStatusCode(int statusCode)
{
    d->statusCode = statusCode;
}

void GJsonLocalResponse::setValue(const QJsonValue &value)
{
    d->value = value;
}
