#ifndef HTTP_H
#define HTTP_H
#include <QtCore/qlist.h>
#include <QtCore/qstring.h>
#include <QtCore/QUrlQuery.h>
#include <QtNetwork/qnetworkrequest.h>
#include <QtNetwork/qnetworkreply.h>

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
} HttpMethod_t;

// TODO: maybe this should just be removed and QPair be used instead, we don't need to serialize enableds status
typedef struct
{
    bool enabled;
    QString key;
    QString value;
} EnabledKeyValuePair_t;

/* REQUEST STUFF */
typedef struct
{
    QString name;
    QString url;
    HttpMethod_t method;
    QList<EnabledKeyValuePair_t> headers;
    QList<EnabledKeyValuePair_t> parameters;
    QString contentType;
    QString bodyContent;
} HttpRequestModel_t;

inline QString StaticHttpHeaderKeys[2] = {"Host", "Content-Length"};

HttpRequestModel_t *CreateDefaultHttpRequestModel();
QUrl CreateQUrlFromHttpRequestModel(HttpRequestModel_t &model);
QNetworkRequest HttpRequestToQtRequest(HttpRequestModel_t &model);

/* RESPONSE STUFF */
typedef struct
{
    uint statusCode;
    QString contentType;
    QString contentBody;
    QList<QPair<QString, QString>> headers;
    // uint sizeBytes;
} HttpResponseModel_t;

HttpResponseModel_t *QtReplyToHttpResponse(QNetworkReply &reply);

} // namespace Per
#endif // HTTP_H
