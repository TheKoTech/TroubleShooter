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

    std::string address;   // ip адрес либо доменное имя в тестовом виде
    int time = -1;   // число >=0, если пинг был успешен, -1, если не успешен
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