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
                                        {true, "Host", "<set during send>"},
                                        {true, "Content-Length", "<set during send>"},
                                        {true, "Content-Type", ""},
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
    for (const auto &header: model.headers)
    {
        if (!header.enabled || header.key.isEmpty()) continue;
        request.headers().append(header.key, header.value);
    }

    request.setUrl(CreateQUrlFromHttpRequestModel(model));

    request.setRawHeader("Content-Type", model.contentType.toUtf8());
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
    // reply.rea
    model->contentBody = reply.readAll();
    return model;
}

} // namespace Per