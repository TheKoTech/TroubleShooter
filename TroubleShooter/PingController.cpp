#include "PingController.h"

PingController::PingController(wxApp* handler, int timeout, std::vector<wxString>* addresses) : wxThread()
{
    this->handler = handler;
    this->timeout = timeout;
    this->addressVector = addresses;
}

wxThread::ExitCode PingController::Entry()
{
    // Exits loop, if the thread is asked to terminate
    while (!TestDestroy())
    {
        pings_results = new PingRes[CountAddresses()];
        auto pt = new SinglePingThread[CountAddresses()];
        for (int i = 0; i < CountAddresses(); ++i) {
            pings_results[i].address = addressVector->at(i);
            pt[i].SetParams(&pings_results[i], addressVector->at(i), timeout);
            pt[i].Create();
            pt[i].Run();
        }

        wxThread::This()->Sleep(timeout);

        for (int i = 0; i < CountAddresses(); ++i) {
            if (pt[i].IsAlive()) {
                pt[i].Delete();
                pings_results[i].time = -1;
            }
        }

        for (int i = 0; i < 4; ++i) {
            Logger logger(pings_results[i].address, pings_results[i].time, i + 1);
            logger.WriteLog();
            logger.Check();
        }

        wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
    }

    wxQueueEvent(handler, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

PingController::~PingController()
{
    delete addressVector;
    handler = nullptr;
}

int PingController::CountAddresses()
{
    return addressVector->size();
}

PingRes* PingController::GetPingResults()
{
    return pings_results;
}
