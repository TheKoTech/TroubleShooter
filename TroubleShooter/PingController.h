#pragma once

#include "Ping.h"
#include "Logger.h"

#include <wx/app.h>
#include <wx/thread.h>
#include <wx/timer.h>

#define PING_THREAD_UPDATED 16000
#define PING_THREAD_COMPLETED 16001

/// The main thread to run all pinging classes and functions 
class PingController final : public wxThread, public wxEvtHandler
{
public:
    /// The main thread to run all pinging classes and functions 
    /// @param parent - controller (cApp)
    /// @param domains - list of domains that will be pinged
    /// @param timeout - the thread will update every Timeout milliseconds
    PingController(wxApp* parent, wxVector<wxString>* domains, int timeout);
    ~PingController() override;

private:
    wxApp* parent_;
    wxVector<SinglePingThread*>* ping_threads_;
    wxVector<wxString>* domains_;
    int domains_size_;
    int timeout_;

    wxTimer timer_;
    // ReSharper disable once CppInconsistentNaming
    void OnTimer(wxTimerEvent &WXUNUSED(ev));

protected:
    /// Acts like the "main" function. 
    ExitCode Entry() override;
};
