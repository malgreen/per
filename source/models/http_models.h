#ifndef HTTP_MODELS_H_
#define HTTP_MODELS_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
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
} HttpRequestModel;

#endif
