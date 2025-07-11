#ifndef HTTP_H
#define HTTP_H
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>

namespace Per
{
typedef enum
{
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    HEAD,
    OPTIONS,
    // CONNECT,
    // TRACE,
} HttpMethod;

typedef struct
{
    bool enabled;
    QString key;
    QString value;
} EnabledKeyValuePair;

typedef struct
{
    QString name;
    QString url;
    HttpMethod method;
    QList<EnabledKeyValuePair> headers;
    QList<EnabledKeyValuePair> parameters;
    QString contentType;
    QString bodyContent;
} HttpRequestModel;

static QString StaticHttpHeaderKeys[2] = {"Host", "Content-Length"};

static void SetDefaultHttpHeaders(HttpRequestModel *model)
{
    // TODO: better way to init headers?
    model->headers = {
        {true, "Host", "<set during send>"},
        {true, "Content-Length", "<set during send>"},
        {true, "Content-Type", model->contentType},
        {true, "User-Agent", "Chrome/138.0.0.0"}, // could be something else
        {true, "Accept", "application/json"},
        {true, "Accept-Encoding", "gzip, deflate, br"},
        {true, "Connection", "keep-alive"},
    };
}

} // namespace Per
#endif // HTTP_H
