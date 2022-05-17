#pragma once

#include "MultiPingService.h"
#include <omp.h>
#include <string>

using namespace std;

void MultiPing256(PingRes* results, string base_ip, int timeout);

void MultiPing(PingRes* results, string* adresses, int num_adresses, int timeout);

void Ping(PingRes* result, string host, int timeout);