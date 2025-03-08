#ifndef HTTP_MODELS_H_
#define HTTP_MODELS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <map>

typedef enum
{
    GET,
    POST,
    PUT
} HttpMethod;

static std::map<HttpMethod, char const *> HttpMethodMap = {
    {HttpMethod::GET, "GET"},

};

typedef struct
{
    HttpMethod method;
    wxString url;
} HttpRequestModel;

#endif