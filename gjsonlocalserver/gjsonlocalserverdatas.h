#ifndef GJSONLOCALSERVERDATAS_H
#define GJSONLOCALSERVERDATAS_H

#include <QtCore/QSharedDataPointer>
#include <QtCore/QObject>

#include "gjsonstandardstatus.h"

class QJsonValue;

class GJsonLocalServer;
class GJsonLocalRequest;
class GJsonLocalResponse;

// class GJsonLocalMethod

#define G_SERVER(DerivedServer) \
    DerivedServer *s = qobject_cast<DerivedServer *>(server())

class GJsonLocalMethodPrivate;
class GJsonLocalMethod
{
public:
    GJsonLocalMethod();
    virtual ~GJsonLocalMethod();

public:
    virtual QString name() const = 0;
    virtual void process(const GJsonLocalRequest &req, GJsonLocalResponse &resp) = 0;
    GJsonLocalServer *server() const;

private:
    GJsonLocalMethodPrivate *d;

    friend class GJsonLocalServer;
};

// class GJsonLocalRequest

class GJsonLocalRequestPrivate;
class GJsonLocalRequest
{
public:
    GJsonLocalRequest(const QByteArray &chunk);
    GJsonLocalRequest(const GJsonLocalRequest &);
    GJsonLocalRequest &operator=(const GJsonLocalRequest &);
    ~GJsonLocalRequest();

public:
    bool isValid() const;

    QString method() const;
    QJsonValue value() const;

private:
    QSharedDataPointer<GJsonLocalRequestPrivate> d;
};

// class GJsonLocalResponse

class GJsonLocalResponsePrivate;
class GJsonLocalResponse : public GJsonStandardStatus
{
public:
    GJsonLocalResponse();
    GJsonLocalResponse(const GJsonLocalResponse &);
    GJsonLocalResponse &operator=(const GJsonLocalResponse &);
    ~GJsonLocalResponse();

public:
    bool isValid() const;
    QByteArray chunk() const;

    void setStatusCode(int statusCode);
    void setValue(const QJsonValue &value);

private:
    QSharedDataPointer<GJsonLocalResponsePrivate> d;
};

#endif // GJSONLOCALSERVERDATAS_H
