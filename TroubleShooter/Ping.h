#pragma once

#include "PingService.h"
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

enum PING_END
{
    GOOD,
    FAIL
};

struct PingRes
{
    PingRes();

    wxString address;   // ip ����� ���� �������� ��� � �������� ����
    int time = -1;   // ����� >=0, ���� ���� ��� �������, -1, ���� �� �������
};

class SinglePingThread : public wxThread
{
public:
    SinglePingThread();
    SinglePingThread(const SinglePingThread&);
    SinglePingThread(PingRes* result, wxString host, int timeout);
    void SetParams(PingRes* result, wxString host, int timeout);
private:
    void* Entry() override;
    PingRes* result;
    wxString host;
    int timeout;
};


PingRes* MultiPing(std::vector<wxString> addresses, int num_addresses, int timeout);

void Ping(PingRes* result, wxString host);