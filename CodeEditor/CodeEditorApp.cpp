#include "CodeEditorApp.h"

#include "MimeMapping.h"
#include "resource.h"

#include "include\wrapper\cef_zip_archive.h"
#include "include\wrapper\cef_stream_resource_handler.h"

#include <algorithm>

static const char* schemeName = "app";
static const size_t schemeAndDelimiterLength = strlen(schemeName) + strlen("://");

CodeEditorApp::CodeEditorApp()
{
    LoadResourcesArchive();
}

void CodeEditorApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
    registrar->AddCustomScheme(
        schemeName,
        true /*is_standard*/,
        false /*is_local*/,
        false /*is_diplay_isolated*/,
        true /*is_secure*/,
        true /*is_cors_enabled*/,
        false /*is_csp_bypassing*/);
}

CefRefPtr<CefBrowserProcessHandler> CodeEditorApp::GetBrowserProcessHandler()
{
    return this;
}

void CodeEditorApp::OnContextInitialized()
{
    CefRegisterSchemeHandlerFactory(schemeName, {}, this);
}

CefRefPtr<CefResourceHandler> CodeEditorApp::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request)
{
    if (scheme_name == schemeName)
    {
        auto url = request->GetURL().ToString().substr(schemeAndDelimiterLength);
        auto fragmentIndex = url.find_last_of('#');
        if (fragmentIndex != std::string::npos)
        {
            url = url.substr(0, fragmentIndex);
        }

        std::transform(url.begin(), url.end(), url.begin(), tolower);
        return new CefStreamResourceHandler(
            MimeMapping::GetMimeMapping(url.c_str()),
            GetResource(url.c_str()));
    }

    return nullptr;
}

void CodeEditorApp::LoadResourcesArchive()
{
    HMODULE module = GetModuleHandle(NULL);
    HRSRC resourceInfo = FindResource(module, MAKEINTRESOURCE(IDR_WEB_ZIP), RT_RCDATA);
    HGLOBAL resourceData = LoadResource(module, resourceInfo);
    auto size = SizeofResource(module, resourceInfo);
    void* data = LockResource(resourceData);

    m_resourcesArchive = new CefZipArchive();

    auto resourcesArchiveStream = CefStreamReader::CreateForData(data, size);
    m_resourcesArchive->Load(resourcesArchiveStream, {}, true);
}

CefRefPtr<CefStreamReader> CodeEditorApp::GetResource(const char* url)
{
    return m_resourcesArchive->GetFile(url)->GetStreamReader();
}
