#include "ConfigController.h"

std::list<wxString>* SettingsController::getAddressList() {
    auto file = new wxFileConfig();
    file->
    auto addressList = new std::list<wxString>();
    if (!file->Open(wxString("config.json"), wxFile::read)) {
        createDefaultConfigFile();
    }
    addressList->push_back(wxString("helo"));
    file->Close();
    delete file;    
    return addressList;
}

void SettingsController::createDefaultConfigFile() {
    auto file = new wxFile();
    file->Create(wxString("config.json"));
    file->Write(wxString("{\n    \"LAN\": \"192.168.1.1\"\n}"));

    file->Close();
    delete file;
}
