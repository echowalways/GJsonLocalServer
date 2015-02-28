#include "gjsonlocalserver.h"
#include "gjsonlocalserver_p.h"

#include <QtCore/QLoggingCategory>

#include "gjsonstandardstatus.h"
#include "gjsonlocalserverdatas.h"

Q_LOGGING_CATEGORY(qlcJsonLocalServer, "GJsonLocalServer")
Q_LOGGING_CATEGORY(qlcJsonLocalServerDispatch, "GJsonLocalServer.dispatch")

// class GJsonLocalServer

GJsonLocalServer::GJsonLocalServer(QObject *parent)
    : QObject(*new GJsonLocalServerPrivate(QDBusConnection::sessionBus()), parent)
{
}

GJsonLocalServer::GJsonLocalServer(const QDBusConnection &connection, QObject *parent)
    : QObject(*new GJsonLocalServerPrivate(connection), parent)
{
}

void GJsonLocalServer::dispatch(const GJsonLocalRequest &req, GJsonLocalResponse &resp)
{
    Q_D(GJsonLocalServer);

    GJsonLocalMethod *method = d->methods.value(req.method());
    if (method) {
        method->process(req, resp);
    }
}

void GJsonLocalServer::setServiceName(const QString &name)
{
    Q_D(GJsonLocalServer);

    if (d->serviceName != name) {
        d->serviceName = name;

        if (d->serviceName.isEmpty()) {
            qCDebug(qlcJsonLocalServer,
                    "The new service name is empty.");
        }
    }
}

void GJsonLocalServer::setPath(const QString &path)
{
    Q_D(GJsonLocalServer);

    if (d->path != path) {
        d->path = path;

        if (d->path.isEmpty()) {
            qCDebug(qlcJsonLocalServer,
                    "The new path is empty.");
        }
    }
}

QString GJsonLocalServer::serviceName() const
{
    Q_D(const GJsonLocalServer);

    return d->serviceName;
}

QString GJsonLocalServer::path() const
{
    Q_D(const GJsonLocalServer);

    return d->path;
}

bool GJsonLocalServer::listen(QDBusConnection::RegisterOptions options)
{
    Q_D(GJsonLocalServer);

    if (d->isServiceReady
            || d->isServiceObjectReady) {
        qCDebug(qlcJsonLocalServer,
                "The service is already running.");
        return false;
    }

    Q_ASSERT_X(d->connection.isConnected(), "GJsonLocalServer", "The connection is broken.");
    if (!d->connection.isConnected()) {
        qCWarning(qlcJsonLocalServer, "The connection is broken.");
        return false;
    }

    Q_ASSERT_X(!d->serviceName.isEmpty(), "GJsonLocalServer", "The service name is empty.");
    if (d->serviceName.isEmpty()) {
        qCWarning(qlcJsonLocalServer, "The service name is empty.");
        return false;
    }

    Q_ASSERT_X(!d->path.isEmpty(), "GJsonLocalServer", "The path is empty.");
    if (d->path.isEmpty()) {
        qCWarning(qlcJsonLocalServer, "The path is empty.");
        return false;
    }

    if (!d->connection.registerService(d->serviceName)) {
        qCWarning(qlcJsonLocalServer, "Register service failed. %s",
                  qPrintable(d->serviceName));
        close();
        return false;
    }
    d->isServiceReady = true;

    if (!d->connection.registerObject(d->path, this, options)) {
        qCWarning(qlcJsonLocalServer, "Register service object failed. %s, %d",
                  qPrintable(d->path), int(options));
        close();
        return false;
    }
    d->isServiceObjectReady = true;

    qCDebug(qlcJsonLocalServer, "The service is listening on %s, %s",
            qPrintable(d->serviceName), qPrintable(d->path));

    return true;
}

bool GJsonLocalServer::listen(const QString &serviceName, const QString &path,
                              QDBusConnection::RegisterOptions options)
{
    setServiceName(serviceName);
    setPath(path);

    return listen(options);
}


void GJsonLocalServer::close()
{
    Q_D(GJsonLocalServer);

    if (d->isServiceReady || d->isServiceObjectReady) {
        if (d->isServiceObjectReady) {
            d->connection.unregisterObject(d->path);
            d->isServiceObjectReady = false;
        }

        if (d->isServiceReady) {
            d->connection.unregisterService(d->serviceName);
            d->isServiceReady = false;
        }

        qCDebug(qlcJsonLocalServer, "The service closed. %s, %s",
                qPrintable(d->serviceName), qPrintable(d->path));
    }
}

bool GJsonLocalServer::addMethod(GJsonLocalMethod *method)
{
    Q_D(GJsonLocalServer);

    Q_ASSERT_X(method, "GJsonLocalServer", "Null method.");
    if (!method) {
        qCDebug(qlcJsonLocalServer) << "Null method.";
        return false;
    }

    if (method->name().isEmpty()) {
        qCDebug(qlcJsonLocalServer) << "Empty method name.";
        return false;
    }

    delete d->methods.take(method->name());
    d->methods.insert(method->name(), method);

    method->d = reinterpret_cast<GJsonLocalMethodPrivate *>(this);

    return true;
}

void GJsonLocalServer::removeMethod(const QString &methodName)
{
    Q_D(GJsonLocalServer);

    delete d->methods.take(methodName);
}

QByteArray GJsonLocalServer::dispatch(const QByteArray &chunk)
{
    if (chunk.isEmpty()) {
        qCDebug(qlcJsonLocalServerDispatch)
                << "Empty request.";
        return QByteArray();
    }

    GJsonLocalRequest request(chunk);
    GJsonLocalResponse response;

    if (request.isValid()) {
        dispatch(request, response);
    } else {
        response.setStatusCode(GJsonLocalResponse::BadRequest);
        qCDebug(qlcJsonLocalServerDispatch) << "Bad request.";
    }

    if (!response.isValid()) {
        response.setStatusCode(GJsonLocalResponse::NotImplemented);
        qCDebug(qlcJsonLocalServerDispatch) << "Not Implemented:" << request.method();
    }
    return response.chunk();
}

// class GJsonLocalServerPrivate

GJsonLocalServerPrivate::GJsonLocalServerPrivate(const QDBusConnection &connection)
    : connection(connection)
    , isServiceReady(false)
    , isServiceObjectReady(false)
{
}

GJsonLocalServerPrivate::~GJsonLocalServerPrivate()
{
    qDeleteAll(methods);
}

// moc_gjsonlocalserver.cpp
#include "moc_gjsonlocalserver.cpp"
