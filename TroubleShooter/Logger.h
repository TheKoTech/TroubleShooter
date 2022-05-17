#pragma once

#include <wx/file.h>
#include <wx/textfile.h >
#include <wx/datetime.h>
#include <wx/numformatter.h>

class Logger
{

private:
    wxString Ip;  //  ip
    int Delay;  //задержка
public:
    Logger(wxString set_Ip, int set_Delay);
    void deletelines();
    void setDelay(wxString set_Ip, int set_Delay);
    void WriteLog();
    void Check();
};