#include <QtCore>
#include <QtTest>

#include <GJsonAccessManager>
#include <GJsonRequest>
#include <GJsonResponse>

#include <GEchoLocalServer>

class GJsonLocalServerTest : public QObject
{
    Q_OBJECT

public:
    GJsonLocalServerTest();

private Q_SLOTS:
    void testRequestLoop();
};

GJsonLocalServerTest::GJsonLocalServerTest()
{
}

void GJsonLocalServerTest::testRequestLoop()
{
    GEchoLocalServer localServer;
    localServer.listen(QLatin1Literal("com.localServer.echoLocalServer"),
                       QLatin1Literal("/com/localServer/echo"));

    GJsonAccessManager accessManager;

    // 测试全内容.
    qDebug() << "测试全内容";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QLatin1Literal("/com/localServer/echo"),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"),
                         QLatin1Literal("echo"), QLatin1Literal("Hello."));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(!resp.isError());
        QVERIFY(resp.statusCode() == GJsonResponse::Ok);
        qDebug() << resp.request().value().toString() << resp.value().toString();
    } while (false);

    // 测试不带值.
    qDebug() << "测试不带值";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QLatin1Literal("/com/localServer/echo"),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"),
                         QLatin1Literal("echo"));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(!resp.isError());
        QVERIFY(resp.statusCode() == GJsonResponse::Ok);
        qDebug() << resp.value().toString();
    } while (false);

    // 测试不设置方法名.
    qDebug() << "测试不设置方法名";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QLatin1Literal("/com/localServer/echo"),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(!resp.isError());
        QVERIFY(resp.statusCode() == GJsonResponse::BadRequest);
    } while (false);

    // 测试设置无意义方法名.
    qDebug() << "测试设置无意义方法名";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QLatin1Literal("/com/localServer/echo"),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"),
                         QLatin1Literal("unknownMethod"));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(!resp.isError());
        QVERIFY(resp.statusCode() == GJsonResponse::NotImplemented);
    } while (false);

    // 测试不指定服务.
    qDebug() << "测试不指定服务";
    do {
        GJsonRequest req(QString(),
                         QLatin1Literal("/com/localServer/echo"),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"),
                         QLatin1Literal("echo"), QLatin1Literal("Hello."));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(resp.isError());
    } while (false);

    // 测试不指定路径.
    qDebug() << "测试不指定路径";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QString(),
                         QLatin1Literal("com.localServer.echoLocalServer.interface"),
                         QLatin1Literal("echo"), QLatin1Literal("Hello."));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(resp.isError());
    } while (false);

    // 测试不指定接口.
    qDebug() << "测试不指定接口";
    do {
        GJsonRequest req(QLatin1Literal("com.localServer.echoLocalServer"),
                         QLatin1Literal("/com/localServer/echo"),
                         QString(),
                         QLatin1Literal("echo"), QLatin1Literal("Hello."));

        GJsonResponse resp = accessManager.call(req);
        QVERIFY(!resp.isError());
        qDebug() << resp.value().toString();
    } while (false);
}

QTEST_GUILESS_MAIN(GJsonLocalServerTest)

#include "tst_gjsonlocalservertest.moc"
