#ifndef GJSONLOCALSERVER_H
#define GJSONLOCALSERVER_H

#include <QtCore/QObject>
#include <QtDBus/QDBusConnection>

class GJsonLocalMethod;
class GJsonLocalRequest;
class GJsonLocalResponse;

class GJsonLocalServerPrivate;
class GJsonLocalServer : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(GJsonLocalServer)

public:
    explicit GJsonLocalServer(QObject *parent = 0);
    GJsonLocalServer(const QDBusConnection &connection, QObject *parent = 0);

protected:
    virtual void dispatch(const GJsonLocalRequest &req, GJsonLocalResponse &resp);

public:
    void setServiceName(const QString &name);
    void setPath(const QString &path);

    QString serviceName() const;
    QString path() const;

    bool listen(QDBusConnection::RegisterOptions options = QDBusConnection::ExportAllSlots);
    bool listen(const QString &serviceName, const QString &path,
                QDBusConnection::RegisterOptions options = QDBusConnection::ExportAllSlots);

    void close();

public:
    bool addMethod(GJsonLocalMethod *method);
    void removeMethod(const QString &methodName);

public Q_SLOTS:
    QByteArray dispatch(const QByteArray &chunk);
};

#endif // GJSONLOCALSERVER_H
