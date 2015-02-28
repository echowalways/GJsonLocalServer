#ifndef GECHOLOCALSERVER_H
#define GECHOLOCALSERVER_H

#include <GJsonLocalServer>

class GEchoLocalServer : public GJsonLocalServer
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.localServer.echoLocalServer.interface")

public:
    explicit GEchoLocalServer(QObject *parent = 0);

public:
    void echo(const QString &text);
};

#endif // GECHOLOCALSERVER_H
