#include "gecholocalserver.h"

#include <QtCore/QLoggingCategory>

#include <QtCore/QJsonValue>

#include <GJsonLocalMethod>
#include <GJsonLocalRequest>
#include <GJsonLocalResponse>

Q_LOGGING_CATEGORY(qlcEchoLocalServer, "GEchoLocalServer")

// class GEchoLocalServerEchoMethod

class GEchoLocalServerEchoMethod : public GJsonLocalMethod
{
public:
    QString name() const { return QLatin1Literal("echo"); }
    void process(const GJsonLocalRequest &req, GJsonLocalResponse &resp)
    {
        G_SERVER(GEchoLocalServer);

        // 处理具体操作.
        s->echo(req.value().toString());

        // 设置回应内容.
        resp.setValue(req.value());
        resp.setStatusCode(GJsonLocalResponse::Ok);
    }
};

// class GEchoLocalServer

GEchoLocalServer::GEchoLocalServer(QObject *parent)
    : GJsonLocalServer(parent)
{
    addMethod(new GEchoLocalServerEchoMethod());
}

void GEchoLocalServer::echo(const QString &text)
{
    qCDebug(qlcEchoLocalServer) << text;
}
