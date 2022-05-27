#include "ConfigController.h"

ConfigController::ConfigController(wxApp* handler)
{
    this->handler = handler;
    configName = wxFileName(handler->argv[0]); // executable name
}

std::list<wxString>* ConfigController::getAddressList()
{
    auto addressList = new std::list<wxString>();
    // this monstrocity gets path to the executable
    auto config = new wxFileConfig("", "", configName.GetPath(wxPATH_GET_VOLUME | wxPATH_GET_SEPARATOR) + "config.ini");
    config->Write(wxString("brl"), wxString("brlbrlbrllbrrlblbr"));


    config->Flush();

    delete config;    
    return addressList;
}

void ConfigController::createDefaultConfigFile() 
{
    
}
