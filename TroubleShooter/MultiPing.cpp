#include "MultiPing.h"

#include <vector>

using namespace std;


PingRes::PingRes()
{
}

SinglePingThread::SinglePingThread() : wxThread(wxTHREAD_JOINABLE)
{
}

SinglePingThread::SinglePingThread(PingRes* result, string host, int timeout)
{
    this->result = result;
    this->host = host;
    this->timeout = timeout;
}

void SinglePingThread::SetParams(PingRes* result, string host, int timeout)
{
    this->result = result;
    this->host = host;
    this->timeout = timeout;
}

// TODO Нет обработчиков событий. См. cMainPingThread.cpp. ThreadUpdated необязателен.
void* SinglePingThread::Entry()
{
    Ping(result, host);
    return nullptr;
}

PingRes* MultiPing(string* addresses, int num_addresses, int timeout) {
    auto results = new PingRes[num_addresses];
    //auto pt = new SinglePingThread[num_addresses];
    vector<SinglePingThread> pt;
    for (int i = 0; i < num_addresses; ++i) {
        results[i].address = addresses[i];
        pt.emplace_back(&results[i], addresses[i], timeout);
        pt[i].Create();
        pt[i].Run();
    }

    // TODO Вместо бесконечной проверки, использовать событие wxEVT_THREAD с уникальным ID. См. cMainPingThread.cpp
    time_t start = clock();
    bool exist_alive = true;
    while (exist_alive) {
        exist_alive = false;
        for (int i = 0; i < num_addresses; ++i)
            if (pt[i].IsAlive()) {
                if (clock() - start < timeout)
                    exist_alive = true;
                else {
                    pt[i].Delete();
                    delete& pt[i];
                    results[i].time = -1;
                }
            }
    }
    pt.clear();

    return results;
}

// TODO Задокументировать код
void Ping(PingRes* result, string host)
{
    ICMPHeader* send_buf = nullptr;
    IPHeader* recv_buf = nullptr;

    int packet_size = max(sizeof(ICMPHeader), min(MAX_PING_DATA_SIZE, (unsigned int)DEFAULT_PACKET_SIZE));
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 1), &wsaData);

    bool finishPing = false;
    SOCKET sd;
    sockaddr_in dest, source;
    if (setup_for_ping((char*)host.c_str(), DEFAULT_TTL, sd, dest) < 0) {
        result->time = -2;
        finishPing = true;
    }
    if (!finishPing && allocate_buffers(send_buf, recv_buf, packet_size) < 0) {
        result->time = -2;
        finishPing = true;
    }
    if (!finishPing) {
        int seq_no = 0;
        init_ping_packet(send_buf, packet_size, seq_no);

        time_t start = clock();
        if (send_ping(sd, dest, send_buf, packet_size) >= 0) {
            while (!finishPing) {
                bool ShouldDecodeRes = true;
                if (recv_ping(sd, source, recv_buf, MAX_PING_PACKET_SIZE) < 0) {
                    result->time = -2;

                    unsigned short header_len = recv_buf->h_len * 4;
                    auto icmphdr = (ICMPHeader*)((char*)recv_buf + header_len);
                    if (icmphdr->seq != seq_no) {
                        ShouldDecodeRes = false;
                    }
                    else {
                        result->time = -2;
                        finishPing = true;
                    }
                }
                if (ShouldDecodeRes)
                {
                    int decode_res = decode_reply(recv_buf, packet_size, &source);
                    if (decode_res != -2) {
                        if (decode_res == -1) {
                            result->time = -2;
                        }
                        finishPing = true;
                    }
                }
            }
        }
        else {
            result->time = -2;
        }
        if (result->time > -2) {
            result->time = clock() - start;
        }
    }

    delete[]send_buf;
    delete[]recv_buf;
    WSACleanup();
}