#pragma once
#include <wx/event.h>
#include <wx/thread.h>
#include <vector>

enum AddressType
{
    LAN,
    ISP,
    DNS,
    HOST
};

struct ping_res
{
    wxString domain;
    int ping_time = -1; // Ping time in millisecond or -1 if request timed out
};

class SinglePingThread : public wxThread
{
public:
    SinglePingThread();
    void set_params(ping_res* result, wxString host, int timeout);

private:
    void* Entry() override;
    ping_res* result_;
    wxString host_;
    int timeout_;
};

ping_res* multi_ping(std::vector<wxString> addresses, int num_addresses, int timeout);
int ping(const char* host);
void ping(ping_res* result, wxString host);
