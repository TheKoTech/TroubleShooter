#include "Logger.h"


Logger::Logger(wxString set_Ip, int set_Delay, int set_marker) {
    setDelay(set_Ip, set_Delay, set_marker);
}
void Logger::deletelines() { 
    wxString filename = "ping_res.csv";
    wxTextFile filein;
    filein.Open(filename);
    filein.RemoveLine(0);
    filein.Write(wxTextFileType_Unix);
    filein.Close();
}
void Logger::setDelay(wxString set_Ip, int set_Delay, int set_marker)
{
    Ip = set_Ip;
    Delay = set_Delay;
    marker = set_marker;
}

void Logger::WriteLog()
{
    wxDateTime now = wxDateTime::Now();
    wxString date1 = now.Format(wxT("%d-%m-%y;%T"));
    wxFile file;
    file.Open(wxT("ping_res.csv"), wxFile::write_append);
    file.Write(Ip + ";" + wxString::Format(wxT("%i"), Delay) + ";" + date1 + ";" + wxString::Format(wxT("%i"), marker) + "\n");
}

void Logger::Check() {
    int num;
    wxTextFile file;
    file.Open(wxT("ping_res.csv"));
    num = file.GetLineCount();
    if (num > 500) {
        deletelines();
    }
}
