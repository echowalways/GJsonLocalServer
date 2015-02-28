#ifndef GJSONACCESSMANAGER_P_H
#define GJSONACCESSMANAGER_P_H

#include <QtCore/private/qobject_p.h>
#include <QtDBus/QDBusConnection>

#include "gjsonaccessmanager.h"

class GJsonAccessManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(GJsonAccessManager)

public:
    GJsonAccessManagerPrivate(const QDBusConnection &connection);
    virtual ~GJsonAccessManagerPrivate();

public:
    QDBusConnection connection;

public:
    QDBusPendingCall call(const GJsonRequest &request) const;
};

#endif // GJSONACCESSMANAGER_P_H

