#include "PingController.h"

PingController::PingController(wxApp* handler, int timeout, std::list<wxString>* addresses) : wxThread()
{
	this->handler = handler;
	this->timeout = timeout;
    this->addressList = addresses;
    if (Run() != wxTHREAD_NO_ERROR) {
        wxLogError("Can't create the thread!");
    }
}

wxThread::ExitCode PingController::Entry()
{
    // Exits loop, if the thread is asked to terminate
    while (!TestDestroy())
    {
        // The main thread loop
        wxThread::This()->Sleep(10);

        ////////////////

        // ������ ����� ����� �����:
        // 1. ���������� ������ ����
        // 2. ����� Timeout ���������� ����� ������ �������� ��� ���������� ������ ������
        // 3. (���� ������.) ���������� wxQueueEvent, ���������� ��������� � ���������� ��� ����������� ����� ������ � �������
        wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
        
        // 4. ���������� ����������� ��������


        ////////////////

        // ��� ������ ���:
        // PingRes* res_ping = new PingRes[num_adresses];
        // MultiPing(res_ping, adresses, num_adresses, timeout);
        // ���������� ���������� ����������������, � ������ ����� ����������� � *������������* ��������� �������.

        // ��� ��� ����� �������� ��� ���������:
        // PingRes* pingRes = MultiPing(...);

        // ��� ���� 
        // auto pingResults = SendPingPackages(...);
    }

    // processed by handler - cApp
    wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

PingController::~PingController()
{
    delete addressList;
    handler = nullptr;
}

int PingController::CountAddresses()
{
    return addressList->size();
}



////// �������� ����������������� ���� //////


//for (int i = 0; i < num_adresses; ++i) {
//	Logger logger(res_ping[i].adress, res_ping[i].time);
//	logger.WriteLog();
//	logger.Check();
//}
////// ����� �������� ����������������� ���� //////