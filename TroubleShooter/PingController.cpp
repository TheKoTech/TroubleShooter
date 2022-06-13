#include "PingController.h"

#include <iostream>

#pragma region Constructor

PingController::PingController(wxApp* parent, wxVector<wxString>* domains, const int timeout) : wxThread()
{
    parent_ = parent;
    domains_ = domains;
    domains_size_ = static_cast<int>(domains_->size());
    ping_threads_ = new wxVector<SinglePingThread*>();
    timeout_ = timeout;

    // Initialize Winsock
    auto wsa_data = WSAData();
    constexpr WORD dll_version = MAKEWORD(2, 2);
    if (WSAStartup(dll_version, &wsa_data) != 0)
        std::cout << "WSAData didn't start" << std::endl;

    // Initialize wxTimer
    timer_.SetOwner(this);
    Bind(wxEVT_TIMER, &PingController::OnTimer, this);
    timer_.Start(timeout_);
}

PingController::~PingController()
{
    if (parent_ != nullptr)
    {
        parent_ = nullptr;
        Exit();
    }
}

#pragma endregion Constructor

wxThread::ExitCode PingController::Entry()
{
    while (!TestDestroy())
    {
        Sleep(100);
    }

    Unbind(wxEVT_TIMER, &PingController::OnTimer, this);
    timer_.Stop();
    WSACleanup();
    ping_threads_->clear();
    wxQueueEvent(parent_, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));

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
