#pragma once
#include <wx/event.h>
#include <wx/thread.h>


class SinglePingThread : public wxThread
{
public:
    SinglePingThread(const wxString& domain, int timeout);
    ~SinglePingThread() override;

private:
    wxString domain_;
    int timeout_;
    int ping_time_;

    void* Entry() override;
    int ping() const;

public:
    int get_ping_time() const;
    wxString get_domain() const;
};
