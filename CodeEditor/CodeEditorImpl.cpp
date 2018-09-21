#include "CodeEditorImpl.h"

#include "include\wrapper\cef_zip_archive.h"
#include "include\wrapper\cef_stream_resource_handler.h"

#include "ICodeEditorRenderer.h"
#include "MimeMapping.h"
#include "resource.h"

#include <algorithm>
#include <string>

static const char* schemeName = "app";

template <typename T, size_t N>
constexpr size_t count_of(T (&)[N])
{
    return N;
}

CodeEditorImpl::CodeEditorImpl(ICodeEditorRenderer* renderer)
    : m_renderer(renderer)
{
    CefSettings settings;

    static const wchar_t browserSubprocessPath[] = L"CodeEditor.BrowserSubprocess.exe";
    cef_string_set(
        browserSubprocessPath,
        count_of(browserSubprocessPath),
        &settings.browser_subprocess_path,
        false);

    settings.windowless_rendering_enabled = true;
    settings.no_sandbox = true;

    CefInitialize({}, settings, this, nullptr);

    LoadResourcesArchive();
}

void CodeEditorImpl::OnContextInitialized()
{
    CefRegisterSchemeHandlerFactory(schemeName, {}, this);
}

void CodeEditorImpl::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
    registrar->AddCustomScheme(
        schemeName,
        false /*is_standard*/,
        true /*is_local*/,
        false /*id_diplay_isolated*/,
        false /*is_secure*/,
        false /*is_cors_enabled*/,
        true /*is_csp_bypassing*/);
}

bool CodeEditorImpl::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
    return false;
}

void CodeEditorImpl::OnPaint(
    CefRefPtr<CefBrowser> /*browser*/,
    PaintElementType /*type*/,
    const RectList& /*dirtyRects*/,
    const void* buffer,
    int width,
    int height)
{
}

CefRefPtr<CefResourceHandler> CodeEditorImpl::Create(
    CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    const CefString& scheme_name,
    CefRefPtr<CefRequest> request)
{
    if (scheme_name == schemeName)
    {
        auto url = request->GetURL().ToString().substr(6);
        auto fragmentIndex = url.find_last_of('#');
        if (fragmentIndex != std::string::npos)
        {
            url = url.substr(0, fragmentIndex);
        }

        std::transform(url.begin(), url.end(), url.begin(), tolower);
        new CefStreamResourceHandler(
            MimeMapping::GetMimeMapping(url.c_str()),
            GetResource(url.c_str()));
    }

    return nullptr;
}

void CodeEditorImpl::LoadResourcesArchive()
{
    HMODULE module = GetModuleHandle(NULL);
    HRSRC resourceInfo = FindResource(module, MAKEINTRESOURCE(IDR_WEBZIP), MAKEINTRESOURCE(0));
    HGLOBAL resourceData = LoadResource(module, resourceInfo);
    auto size = SizeofResource(module, resourceInfo);
    void* data = LockResource(resourceData);

    m_resourcesArchive = new CefZipArchive();

    auto resourcesArchiveStream = CefStreamReader::CreateForData(data, size);
    m_resourcesArchive->Load(resourcesArchiveStream, {}, true);
}

CefRefPtr<CefStreamReader> CodeEditorImpl::GetResource(const char* url)
{
    return m_resourcesArchive->GetFile(url)->GetStreamReader();
}