#include "Logger.h"


Logger::Logger(wxString set_Ip, int set_Delay) {
    setDelay(set_Ip, set_Delay);
}
void Logger::deletelines() { //удаляем старые  строки
    wxString filename = "ping_res.csv";
    wxTextFile filein;
    filein.Open(filename);
    filein.RemoveLine(0);
    filein.Write(wxTextFileType_Unix);
    filein.Close();
}
void Logger::setDelay(wxString set_Ip, int set_Delay)
{
    Ip = set_Ip;
    Delay = set_Delay;

}

void Logger::WriteLog() //запись строки в файл
{
    wxDateTime now = wxDateTime::Now();
    wxString date1 = now.Format(wxT("%a %T"));
    wxFile file;
    file.Open(wxT("ping_res.csv"), wxFile::write_append);
    file.Write(Ip + ";" + wxString::Format(wxT("%i"), Delay) + ";" + date1 + "\n");

}

void Logger::Check() { //проверка кол-ва строк
    int num;
    wxTextFile file;
    file.Open(wxT("ping_res.csv"));
    num = file.GetLineCount();
    if (num > 3) {
        deletelines();
    }
}
