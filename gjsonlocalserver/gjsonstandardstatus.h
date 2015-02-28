#ifndef GJSONSTANDARDSTATUS_H
#define GJSONSTANDARDSTATUS_H

class GJsonStandardStatus
{
public:
    enum {
        // 2xx 成功.
        Ok                  = 200,

        // 4xx 客户端错误.
        BadRequest          = 400,
        Unauthorized        = 401,
        PaymentRequired     = 402,
        Forbidden           = 403,
        NotFound            = 404,
        MethodNotAllowed    = 405,
        NotAcceptable       = 406,
        ProxyAuthenticationRequired = 407,
        RequestTimeout      = 408,
        Conflict            = 409,
        Gone                = 410,

        // 5xx 服务器错误.
        InternalServerError = 500,
        NotImplemented      = 501
    };
};

#endif // GJSONSTANDARDSTATUS_H
