#pragma once
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/app.h>
#include <list>


class ConfigController {
public:
    ConfigController(wxApp* handler);
    wxVector<wxString>* GetAddressList();
    void WriteAddresses(wxVector<wxString>* addresses);
private:
    wxApp* handler;
    wxFileName configName;
};
