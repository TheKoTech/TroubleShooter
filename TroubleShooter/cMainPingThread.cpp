#include "cMainPingThread.h"

cMainPingThread::cMainPingThread(wxApp* handler, int timeout, std::list<wxString>* addresses) : wxThread(wxTHREAD_JOINABLE)
{
	this->handler = handler;
	this->timeout = timeout;
    this->addressList = addresses;
}

wxThread::ExitCode cMainPingThread::Entry()
{
    // Exits loop, if the thread is asked to terminate
    while (!TestDestroy())
    {
        // The main thread loop
        
        // Это плохой код:
        // PingRes* res_ping = new PingRes[num_adresses];
        // MultiPing(res_ping, adresses, num_adresses, timeout);
        // Переменная результата инициализируется, и только затем заполняется в *неоднозначно* названной функции.

        // Так его можно улучшить для понимания:
        // PingRes* pingRes = MultiPing(...);

        // Или даже 
        // auto pingRes = SendICMPPackages(...);

        wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
    }

    // cleanup
    delete addressList;

    // processed by handler - cApp
    wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

int cMainPingThread::CountAddresses()
{
    return addressList->size();
}



////// Проверка работоспособности кода //////


//for (int i = 0; i < num_adresses; ++i) {
//	Logger logger(res_ping[i].adress, res_ping[i].time);
//	logger.WriteLog();
//	logger.Check();
//}
////// конец проверки работоспособности кода //////