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
} HttpMethod;

// TODO: maybe this should just be removed and QPair be used instead, we don't need to serialize enableds status
typedef struct
{
    bool enabled;
    QString key;
    QString value;
} EnabledKeyValuePair;

/* REQUEST STUFF */
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

inline QString StaticHttpHeaderKeys[2] = {"Host", "Content-Length"};

HttpRequestModel *CreateDefaultHttpRequestModel();
QUrl CreateQUrlFromHttpRequestModel(HttpRequestModel &model);
QNetworkRequest HttpRequestToQtRequest(HttpRequestModel &model);

/* RESPONSE STUFF */
typedef struct
{
    uint statusCode;
    QString contentType;
    QString contentBody;
    QList<QPair<QString, QString>> headers;
    // uint sizeBytes;
} HttpResponseModel;

HttpResponseModel *QtReplyToHttpResponse(QNetworkReply &reply);

} // namespace Per
#endif // HTTP_H
