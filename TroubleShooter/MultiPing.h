#pragma once

#include "MultiPingService.h"
#include <string>
#include <wx/thread.h>

using namespace std;

class PingThread : public wxThread
{
public:
    PingThread();
    void SetParams(PingRes* result, string host, int timeout);
private:
    void* Entry();
    PingRes* result; 
    string host; 
    int timeout;
};


void MultiPing256(PingRes* results, string base_ip, int timeout);

void MultiPing(PingRes* results, string* adresses, int num_adresses, int timeout);

void Ping(PingRes* result, string host, int timeout);