#include "PingController.h"
#include "Logger.h"

#pragma region Constructor

PingController::PingController(wxApp* parent, std::vector<wxString>* domains, const int timeout) : wxThread(),
    pings_results_(nullptr)
{
    this->parent_ = parent;
    this->timeout_ = timeout;
    this->domains_ = domains;
}

PingController::~PingController()
{
    delete domains_;
    parent_ = nullptr;
}

#pragma endregion Constructor


wxThread::ExitCode PingController::Entry()
{
    while (!TestDestroy())
    {
        pings_results_ = new ping_res[domains_count()];
        auto pt = new SinglePingThread[domains_count()];
        for (int i = 0; i < domains_count(); ++i)
        {
            pings_results_[i].domain = domains_->at(i);
            pt[i].set_params(&pings_results_[i], domains_->at(i), timeout_);
            pt[i].Create();
            pt[i].Run();
        }

        wxThread::This()->Sleep(timeout_);

        for (int i = 0; i < domains_count(); ++i)
        {
            if (pt[i].IsAlive())
            {
                pt[i].Delete();
                pings_results_[i].ping_time = -1;
            }
        }

        for (int i = 0; i < 4; ++i)
        {
            Logger logger(pings_results_[i].domain, pings_results_[i].ping_time, i + 1);
            logger.WriteLog();
            logger.Check();
        }

        wxQueueEvent(parent_, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_UPDATED));
    }

    wxQueueEvent(parent_, new wxThreadEvent(wxEVT_THREAD, PING_THREAD_COMPLETED));
    return nullptr;
}

unsigned int PingController::domains_count() const
{
    return domains_->size();
}

ping_res* PingController::get_ping_results() const
{
    return pings_results_;
}
