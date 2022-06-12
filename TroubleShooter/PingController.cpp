#include "PingController.h"

#pragma region Constructor

PingController::PingController(wxApp* parent, wxVector<wxString>* domains, const int timeout) : wxThread()
{
    parent_ = parent;
    domains_ = domains;
    domains_size_ = static_cast<int>(domains_->size());
    ping_threads_ = new wxVector<SinglePingThread*>();
    timeout_ = timeout;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), new WSAData()) != 0) { throw; }

    // Initialize timer
    timer_.SetOwner(this);
    Bind(wxEVT_TIMER, &PingController::OnTimer, this);
    timer_.Start(timeout_);
}

PingController::~PingController()
{
    Unbind(wxEVT_TIMER, &PingController::OnTimer, this);
    timer_.Stop();
    WSACleanup();

    wxQueueEvent(parent_, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    
    parent_ = nullptr;
    delete parent_;
    delete ping_threads_;
    delete domains_;

    Exit();
}

#pragma endregion Constructor

wxThread::ExitCode PingController::Entry()
{
    while(!TestDestroy())
    {
        Sleep(timeout_);
    }
    return nullptr;
}

// ReSharper disable once CppInconsistentNaming
// ReSharper disable once CppMemberFunctionMayBeConst
void PingController::OnTimer(wxTimerEvent&)
{
    if (!ping_threads_->empty())
        for (int i = 0; i < domains_size_; i++)
        {
            Logger logger(ping_threads_->at(i)->get_domain(), ping_threads_->at(i)->get_ping_time(), i + 1);
            logger.WriteLog();
            logger.Check();
        }

    wxQueueEvent(parent_, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
    
    ping_threads_->clear();
    for (int i = 0; i < domains_size_; i++)
    {
        ping_threads_->push_back(new SinglePingThread(domains_->at(i), timeout_));
        ping_threads_->at(i)->Create();
        ping_threads_->at(i)->Run();
    }
}
