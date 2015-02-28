#ifndef GJSONLOCALSERVER_P_H
#define GJSONLOCALSERVER_P_H

#include <QtCore/private/qobject_p.h>
#include <QtDBus/QDBusConnection>

#include "gjsonlocalserver.h"

class GJsonLocalServerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GJsonLocalServer)

public:
    GJsonLocalServerPrivate(const QDBusConnection &connection);
    virtual ~GJsonLocalServerPrivate();

public:
    QDBusConnection connection;

    QString serviceName;
    QString path;

    bool isServiceReady;
    bool isServiceObjectReady;

    QHash<QString, GJsonLocalMethod *> methods;
};

#endif // GJSONLOCALSERVER_P_H

