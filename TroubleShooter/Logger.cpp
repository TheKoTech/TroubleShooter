#include "Logger.h"


Logger::Logger(wxString set_Ip, int set_Delay) {
    setDelay(set_Ip, set_Delay);
}
void Logger::deletelines() { //удаляем старые  строки
    wxString filename = "D:\\Labs_n_Other\\3Vorob\\TS2\\ping_res.txt";
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
    file.Open(wxT("D:\\Labs_n_Other\\3Vorob\\TS2\\ping_res.txt"), wxFile::write_append);
    file.Write(Ip + " " + wxString::Format(wxT("%i"), Delay) + " " + date1 + "\n");

}

void Logger::Check() { //проверка кол-ва строк
    int num;
    wxTextFile file;
    file.Open(wxT("D:\\Labs_n_Other\\3Vorob\\TS2\\ping_res.txt"));
    num = file.GetLineCount();
    if (num > 500) {
        deletelines();
    }
}
