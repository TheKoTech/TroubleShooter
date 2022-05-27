#pragma once
#include "wx/fileconf.h"
#include <list>

// 
//

class SettingsController {
public:
    std::list<wxString>* getAddressList();
    void createDefaultConfigFile();
};
