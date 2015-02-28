#ifndef GJSONACCESSMANAGERDATAS_H
#define GJSONACCESSMANAGERDATAS_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QJsonValue>

#include <QtDBus/QDBusPendingCallWatcher>

#include "gjsonstandardstatus.h"

class QJsonValue;
class QDBusPendingCall;

// class GJsonRequest

class GJsonRequestPrivate;
class GJsonRequest
{
public:
    GJsonRequest(const GJsonRequest &);
    GJsonRequest &operator=(const GJsonRequest &);
    ~GJsonRequest();

public:
    GJsonRequest(const QString &service, const QString &path, const QString &interface);
    GJsonRequest(const QString &service, const QString &path, const QString &interface, const QString &method);
    GJsonRequest(const QString &service, const QString &path, const QString &interface, const QString &method, const QJsonValue &value);

public:
    QString service() const;
    QString path() const;
    QString interface() const;

    QString method() const;
    QJsonValue value() const;

private:
    QSharedDataPointer<GJsonRequestPrivate> d;
};

// class GJsonResponse

class GJsonResponsePrivate;
class GJsonResponse : public GJsonStandardStatus
{
public:
    GJsonResponse(const GJsonResponse &);
    GJsonResponse &operator=(const GJsonResponse &);
    ~GJsonResponse();

public:
    GJsonRequest request() const;
    QDBusPendingCall call() const;

    bool isError() const;
    bool isFinished() const;
    bool isValid() const;

    int statusCode() const;
    QJsonValue value() const;

private:
    friend class GJsonAccessManager;

    GJsonResponse(const GJsonRequest &request,
                  const QDBusPendingCall &call);

    QExplicitlySharedDataPointer<GJsonResponsePrivate> d;
};

// class GJsonResponseWatcher

class GJsonResponseWatcher : public QDBusPendingCallWatcher
{
    Q_OBJECT

public:
    explicit GJsonResponseWatcher(const GJsonResponse &response, QObject *parent = 0)
        : QDBusPendingCallWatcher(response.call(), parent)
        , _q_response(response)
    {
    }

public:
    GJsonRequest request() const { return _q_response.request(); }
    GJsonResponse response() const { return _q_response; }

private:
    GJsonResponse _q_response;
};

#endif // GJSONACCESSMANAGERDATAS_H
