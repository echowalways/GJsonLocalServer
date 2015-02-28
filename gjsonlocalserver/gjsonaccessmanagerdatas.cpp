#include "gjsonaccessmanagerdatas.h"

#include <QtCore/QLoggingCategory>

#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

#include <QtDBus/QDBusPendingCall>

Q_LOGGING_CATEGORY(qlcJsonLocalAccessManagerDatas, "GJsonAccessManager.datas")

// class GJsonRequestPrivate

class GJsonRequestPrivate : public QSharedData
{
public:
    GJsonRequestPrivate(const QString &service,
                        const QString &path,
                        const QString &interface,
                        const QString &method,
                        const QJsonValue &value)
        : service(service)
        , path(path)
        , interface(interface)
        , method(method)
        , value(value)
    {
    }

public:
    QString service;
    QString path;
    QString interface;

    QString method;
    QJsonValue value;
};

// class GJsonRequest

GJsonRequest::GJsonRequest(const GJsonRequest &rhs)
    : d(rhs.d)
{
}

GJsonRequest &GJsonRequest::operator=(const GJsonRequest &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

GJsonRequest::~GJsonRequest()
{
}

GJsonRequest::GJsonRequest(const QString &service, const QString &path, const QString &interface)
    : d(new GJsonRequestPrivate(service, path, interface, QString(), QJsonValue()))
{
}

GJsonRequest::GJsonRequest(const QString &service, const QString &path, const QString &interface, const QString &method)
    : d(new GJsonRequestPrivate(service, path, interface, method, QJsonValue()))
{
}

GJsonRequest::GJsonRequest(const QString &service, const QString &path, const QString &interface, const QString &method, const QJsonValue &value)
    : d(new GJsonRequestPrivate(service, path, interface, method, value))
{
}

QString GJsonRequest::service() const
{
    return d->service;
}

QString GJsonRequest::path() const
{
    return d->path;
}

QString GJsonRequest::interface() const
{
    return d->interface;
}

QString GJsonRequest::method() const
{
    return d->method;
}

QJsonValue GJsonRequest::value() const
{
    return d->value;
}

// class GJsonResponsePrivate

class GJsonResponsePrivate : public QSharedData
{
public:
    GJsonResponsePrivate(const GJsonRequest &request,
                         const QDBusPendingCall &call)
        : request(request)
        , pendingCall(call)
        , statusCode(-1)
    {
    }

public:
    GJsonRequest request;
    QDBusPendingCall pendingCall;

    int statusCode;
    QJsonValue value;

public:
    void updateDatas();
};

void GJsonResponsePrivate::updateDatas()
{
    if (pendingCall.isFinished() && statusCode == -1) {
        QByteArray chunk = pendingCall.reply().arguments().value(0).toByteArray();
        QJsonObject obj = QJsonDocument::fromBinaryData(chunk).object();

        statusCode = obj.value(QLatin1Literal("statusCode")).toInt(-1);
        if (statusCode != -1) {
            value = obj.value(QLatin1Literal("value"));
        }
    }
}

// class GJsonResponse

GJsonResponse::GJsonResponse(const GJsonRequest &request,
                             const QDBusPendingCall &call)
    : d(new GJsonResponsePrivate(request, call))
{
}

GJsonResponse::GJsonResponse(const GJsonResponse &rhs)
    : d(rhs.d)
{
}

GJsonResponse &GJsonResponse::operator=(const GJsonResponse &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

GJsonResponse::~GJsonResponse()
{
}

GJsonRequest GJsonResponse::request() const
{
    return d->request;
}


QDBusPendingCall GJsonResponse::call() const
{
    return d->pendingCall;
}

bool GJsonResponse::isError() const
{
    return d->pendingCall.isError();
}

bool GJsonResponse::isFinished() const
{
    return d->pendingCall.isFinished();
}

bool GJsonResponse::isValid() const
{
    return d->pendingCall.isValid();
}

int GJsonResponse::statusCode() const
{
    if ((d->statusCode == -1)
            && d->pendingCall.isFinished()) {
        const_cast<GJsonResponsePrivate *>(d.data())->updateDatas();
    }
    return d->statusCode;
}

QJsonValue GJsonResponse::value() const
{
    if ((d->statusCode == -1)
            && d->pendingCall.isFinished()) {
        const_cast<GJsonResponsePrivate *>(d.data())->updateDatas();
    }
    return d->value;
}
