#include "gjsonaccessmanager.h"
#include "gjsonaccessmanager_p.h"

#include <QtCore/QLoggingCategory>

#include <QtCore/QJsonValue>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>

#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusPendingCall>

#include "gjsonaccessmanagerdatas.h"

Q_LOGGING_CATEGORY(qlcJsonLocalAccessManager, "GJsonAccessManager")

// class GJsonAccessManager

GJsonAccessManager::GJsonAccessManager(QObject *parent)
    : QObject(*new GJsonAccessManagerPrivate(QDBusConnection::sessionBus()), parent)
{
}

GJsonAccessManager::GJsonAccessManager(const QDBusConnection &connection, QObject *parent)
    : QObject(*new GJsonAccessManagerPrivate(connection), parent)
{
}

GJsonResponse GJsonAccessManager::asyncCall(const GJsonRequest &request)
{
    Q_D(GJsonAccessManager);

    return GJsonResponse(request, d->call(request));
}

GJsonResponse GJsonAccessManager::call(const GJsonRequest &request)
{
    Q_D(GJsonAccessManager);

    QDBusPendingCall dbusCall = d->call(request);

    if (!dbusCall.isError()) {
        dbusCall.waitForFinished();

        if (dbusCall.isError()) {
            qCDebug(qlcJsonLocalAccessManager) << "call failed:" << dbusCall.error();
        }
    }

    return GJsonResponse(request, dbusCall);
}

// class GJsonAccessManagerPrivate

GJsonAccessManagerPrivate::GJsonAccessManagerPrivate(const QDBusConnection &connection)
    : connection(connection)
{
}

GJsonAccessManagerPrivate::~GJsonAccessManagerPrivate()
{
}

QDBusPendingCall GJsonAccessManagerPrivate::call(const GJsonRequest &request) const
{
    QDBusMessage message = QDBusMessage::createMethodCall(request.service(),
                                                          request.path(),
                                                          request.interface(),
                                                          QLatin1Literal("dispatch"));

    QJsonObject obj;
    obj.insert(QLatin1Literal("method"), request.method());
    QJsonValue data = request.value();
    if (!data.isUndefined()) {
        obj.insert(QLatin1Literal("value"), data);
    }
    QByteArray argument = QJsonDocument(obj).toBinaryData();
    message.setArguments(QList<QVariant>() << argument);

    QDBusPendingCall reply = connection.asyncCall(message);
    if (reply.isError()) {
        qCDebug(qlcJsonLocalAccessManager) << "call failed:" << reply.error();
    }
    return reply;
}
