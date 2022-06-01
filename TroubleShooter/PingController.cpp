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
        pings_results_prev = *(new std::vector<PingRes>(pings_results.begin(), pings_results.end()));
        pings_results.erase(pings_results.begin(), pings_results.end());
        auto pt = new SinglePingThread[CountAddresses()];
        for (int i = 0; i < CountAddresses(); ++i) {
            pings_results.emplace_back();
            pings_results.at(i).address = addressVector->at(i);
            pt[i].SetParams(&pings_results.at(i), addressVector->at(i), timeout);
            pt[i].Create();
            pt[i].Run();
        }
        
        wxThread::This()->Sleep(timeout);
        
        for (int i = 0; i < CountAddresses(); ++i) {
            if (pt[i].IsAlive()) {
                pt[i].Delete();
                pings_results.at(i).time = -1;
            }
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

std::vector<PingRes> PingController::GetPingResults()
{
    return pings_results_prev;
}

void PingController::SetAddresses(wxVector<wxString>* newAddresses)
{
    addressVector->at(0) = newAddresses->at(0);
    addressVector->at(1) = newAddresses->at(1);
    addressVector->at(2) = newAddresses->at(2);
    addressVector->at(3) = newAddresses->at(3);
}