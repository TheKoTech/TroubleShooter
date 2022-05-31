#include "Ping.h"


using namespace std;


PingRes::PingRes()
{
}

SinglePingThread::SinglePingThread() : wxThread(wxTHREAD_JOINABLE), result(nullptr), timeout(0)
{
}

SinglePingThread::SinglePingThread(const SinglePingThread&) : wxThread(wxTHREAD_JOINABLE), result(nullptr), timeout(0)
{
}

SinglePingThread::SinglePingThread(PingRes* result, wxString host, int timeout)
{
    this->result = result;
    this->host = host;
    this->timeout = timeout;
}

void SinglePingThread::SetParams(PingRes* result, wxString host, int timeout)
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

PingRes* MultiPing(vector<wxString> addresses, int num_addresses, int timeout) {
    auto results = new PingRes[num_addresses];
    auto pt = new SinglePingThread[num_addresses];
    for (int i = 0; i < num_addresses; ++i) {
        results[i].address = addresses[i];
        pt[i].SetParams(&results[i], addresses[i], timeout);
        pt[i].Create();
        pt[i].Run();
    }

    // TODO Вместо бесконечной проверки, использовать событие wxEVT_THREAD с уникальным ID. См. cMainPingThread.cpp
    time_t start = clock();
    bool exist_alive = true;
    while (exist_alive) {
        wxThread::This()->Sleep(timeout / 10);
        exist_alive = false;
        for (int i = 0; i < num_addresses; ++i)
            if (pt[i].IsAlive()) {
                if (clock() - start < timeout)
                    exist_alive = true;
                else {
                    pt[i].Delete();
                    results[i].time = -1;
                }
            }
    }

    return results;
}

// TODO Задокументировать код
void Ping(PingRes* result, wxString host)
{
    ICMPHeader* send_buf = nullptr;
    IPHeader* recv_buf = nullptr;

    int packet_size = max(sizeof(ICMPHeader), min(MAX_PING_DATA_SIZE, (unsigned int)DEFAULT_PACKET_SIZE));
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 1), &wsaData);

    bool finishPing = false;
    SOCKET sd;
    sockaddr_in dest, source;
    if (setup_for_ping(host.ToUTF8().data(), DEFAULT_TTL, sd, dest) < 0) {
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