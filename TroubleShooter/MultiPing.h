#pragma once

#include "MultiPingService.h"
#include <string>
#include <wx/event.h>
#include <wx/thread.h>

using namespace std;


enum AddressType
{
    LAN,
    ISP,
    DNS,
    HOST
};

struct PingRes
{
    PingRes();

    std::string address;   // ip ����� ���� �������� ��� � �������� ����
    int time = -1;   // ����� >=0, ���� ���� ��� �������, -1, ���� �� �������
};

class SinglePingThread : public wxThread
{
public:
    SinglePingThread();
    SinglePingThread(PingRes* result, string host, int timeout);
    void SetParams(PingRes* result, string host, int timeout);
private:
    void* Entry();
    PingRes* result;
    string host;
    int timeout;
};


void MultiPing(PingRes* results, string* addresses, int num_addresses, int timeout);

void Ping(PingRes* result, string host);