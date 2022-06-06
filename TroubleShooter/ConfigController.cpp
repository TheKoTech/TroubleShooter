#include "ConfigController.h"

#include <ostream>

ConfigController::ConfigController(wxApp* handler)
{
    this->handler = handler;
    configName = wxFileName(handler->argv[0]); // executable name
}

wxVector<wxString>* ConfigController::GetAddressList()
{
    // todo: вынести в функцию
    auto addressList = new wxVector<wxString>();
    wxString str;
    // this monstrocity gets path to the executable
    auto config = new wxFileConfig("", "", configName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + "config.ini");

    if (!config->Read(wxString("lan"), &str)) {
        str = wxString("192.168.1.1");
        config->Write(wxString("lan"), wxString("192.168.1.1"));
    }
    addressList->push_back(str);

    if (!config->Read(wxString("isp"), &str)) {
        str = wxString("");
        config->Write(wxString("isp"), wxString(""));
    }
    addressList->push_back(str);

    if (!config->Read(wxString("dns"), &str)) {
        str = wxString("8.8.8.8");
        config->Write(wxString("dns"), wxString("8.8.8.8"));
    }
    addressList->push_back(str);

    if (!config->Read(wxString("host"), &str)) {
        str = wxString("yandex.ru");
        config->Write(wxString("host"), wxString("yandex.ru"));
    }
    addressList->push_back(str);

    config->Flush();
    delete config;

    return addressList;
}

void ConfigController::WriteAddresses(wxVector<wxString>* addresses)
{
    //todo: create a map instead of this bs
    auto config = new wxFileConfig("", "", configName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + "config.ini");

    for (unsigned int i = 0; i < addresses->size(); i++) {
        wxString key;
        switch (i) {
        case 0:
            key = wxString("lan");
            break;
        case 1:
            key = wxString("isp");
            break;
        case 2:
            key = wxString("dns");
            break;
        case 3:
            key = wxString("host");
            break;
        default:
            key = wxString();
            break;
        }
        config->Write(key, addresses->at(i));
    }
    config->Flush();
    delete config;
}