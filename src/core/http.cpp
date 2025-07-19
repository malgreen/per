#include "http.h"
#include <QtNetwork/QNetworkAccessManager>

namespace Per
{

HttpRequestModel_t *CreateDefaultHttpRequestModel()
{
    return new HttpRequestModel_t{.name = "New request",
                                .url = "https://www.json.org/",
                                .method = GET,
                                .headers =
                                    {
                                        {true, "Host", ""},
                                        {true, "Content-Length", ""},
                                        {true, "Content-Type", "application/json"},
                                        {true, "User-Agent", "Chrome/138.0.0.0"}, // could be something else
                                        {true, "Accept", "application/json"},
                                        {true, "Accept-Encoding", "gzip, deflate, br"},
                                        {true, "Connection", "keep-alive"},
                                    },
                                .parameters = {},
                                .contentType = "",
                                .bodyContent = ""};
}

QUrl CreateQUrlFromHttpRequestModel(HttpRequestModel_t &model)
{
    auto url = QUrl(model.url);
    auto query = QUrlQuery(url);
    for (const auto &parameter : model.parameters)
    {
        if (!parameter.enabled) continue;
        query.addQueryItem(parameter.key, parameter.value);
    }
    url.setQuery(query);
    return url;
}

QNetworkRequest HttpRequestToQtRequest(HttpRequestModel_t &model)
{
    auto request = QNetworkRequest();

    // non-static headers
    for (const auto &header: model.headers)
    {
        if (!header.enabled || header.key.isEmpty() || StaticHttpHeaderKeys.contains(header.key)) continue;
        request.headers().append(header.key, header.value);
    }

    // static headers TODO: is "Host" header set automatically?
    request.setRawHeader("Content-Type", model.contentType.toUtf8());
    request.setRawHeader("Content-Length", QByteArray::number(model.bodyContent.length()));

    // url + params
    request.setUrl(CreateQUrlFromHttpRequestModel(model));

    return request;
}

HttpResponseModel_t *QtReplyToHttpResponse(QNetworkReply &reply)
{
    auto model = new HttpResponseModel_t;
    model->statusCode = reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toUInt(); // or maybe just .toInt();
    model->headers = {};
    for (const auto &header : reply.rawHeaderPairs())
    {
        model->headers.append(header);
    }
    model->contentType = reply.rawHeader("Content-Type");
    model->contentBody = reply.readAll();
    return model;
}

} // namespace Per