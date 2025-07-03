#ifndef HTTP_MODELS_H_
#define HTTP_MODELS_H_

#include <wx/wx.h>
#include <map>

// TODO: this whole enum + map may be a waste of time, possibly just an array of strings?
typedef enum
{
    GET,
    POST,
    PUT
} HttpMethod;

static std::map<HttpMethod, const wxString> HttpMethodMap = {
    {HttpMethod::GET, "GET"},

};

typedef struct
{
    HttpMethod method;
    wxString url;
    std::map<wxString, wxString> parameters;
    std::map<wxString, wxString> headers;
    wxString contentType;
    wxString bodyContent;
} HttpRequestModel;

static void SetDefaultHttpHeaders(HttpRequestModel &model)
{
    model.headers["Host"] = "<will be set during send>";
    model.headers["Content-Length"] = "<will be set during send>";
    model.headers["Content-Type"] = model.contentType; // should probabily implement XML support
    model.headers["User-Agent"] = "PostmanRuntime/1.0";       // TODO: temp?
    model.headers["Accept"] = "application/json";             // should it just be */*?
    model.headers["Accept-Encoding"] = "gzip, deflate, br";   // no idea if this works
    model.headers["Connection"] = "keep-alive";
}

#endif
