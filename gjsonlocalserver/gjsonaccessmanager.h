#ifndef GJSONACCESSMANAGER_H
#define GJSONACCESSMANAGER_H

#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>

class GJsonRequest;
class GJsonResponse;

class GJsonAccessManagerPrivate;
class GJsonAccessManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GJsonAccessManager)

public:
    explicit GJsonAccessManager(QObject *parent = 0);
    GJsonAccessManager(const QDBusConnection &connection, QObject *parent = 0);

public:
    GJsonResponse asyncCall(const GJsonRequest &request);
    GJsonResponse call(const GJsonRequest &request);
};

#endif // GJSONACCESSMANAGER_H
