#pragma once
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/app.h>
#include <list>

// 
//

class ConfigController {
public:
    ConfigController(wxApp* handler);
    std::list<wxString>* getAddressList();
    void createDefaultConfigFile();
private:
    wxApp* handler;
    wxFileName configName;
};
