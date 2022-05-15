#include "MultiPing.h"

using namespace std;

PingRes::PingRes()
{
}

void MultiPing256(PingRes* results, string base_ip, int timeout) {
    int num_adresses = 256;

    string* adresses = new string[num_adresses];
    for (int i = 0; i < num_adresses; ++i) {
        adresses[i] = base_ip + to_string(i);
    }

    #pragma omp parallel for num_threads(num_adresses / 4)
    for (int i = 0; i < num_adresses; ++i) {
        //cout << adresses[i] << endl;
        results[i].adress = adresses[i];
        Ping(&results[i], adresses[i], timeout); 
    }
}

void MultiPing(PingRes* results, string* adresses, int num_adresses, int timeout) {
    #pragma omp parallel for num_threads(num_adresses)
    for (int i = 0; i < num_adresses; ++i) {
        //cout << adresses[i] << endl;
        results[i].adress = adresses[i];
        Ping(&results[i], adresses[i], timeout);
    }
}

void Ping(PingRes* result, string host, int timeout) 
{
    int seq_no = 0;
    ICMPHeader* send_buf = 0;
    IPHeader* recv_buf = 0;

    int packet_size = DEFAULT_PACKET_SIZE;
    int ttl = DEFAULT_TTL;

    packet_size = max(sizeof(ICMPHeader),
        min(MAX_PING_DATA_SIZE, (unsigned int)packet_size));
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 1), &wsaData);

    SOCKET sd;
    sockaddr_in dest, source;
    if (setup_for_ping((char*)host.c_str(), ttl, sd, dest) < 0) {
        result->time = -2;
        goto cleanup;
    }
    if (allocate_buffers(send_buf, recv_buf, packet_size) < 0) {
        result->time = -2;
        goto cleanup;
    }

    init_ping_packet(send_buf, packet_size, seq_no);

    time_t start, end;
    start = clock();
    if (send_ping(sd, dest, send_buf, packet_size) >= 0) {
        while (true) {
            if (recv_ping(sd, source, recv_buf, MAX_PING_PACKET_SIZE) < 0) {
                result->time = -2;

                unsigned short header_len = recv_buf->h_len * 4;
                ICMPHeader* icmphdr = (ICMPHeader*)((char*)recv_buf + header_len);
                if (icmphdr->seq != seq_no) {
                    //cerr << " bad sequence number!" << endl;
                    continue;
                }
                else {
                    result->time = -2;
                    goto cleanup;
                }
            }
            int decode_res = decode_reply(recv_buf, packet_size, &source);
            if (decode_res != -2) {
                //cerr << host + "   " + to_string(decode_res) + " vfb!\n";
                if (decode_res == -1) {
                    result->time = -2;
                }
                goto cleanup;
            }
        }
    }
    else {
        result->time = -2;
    }

cleanup:
    if (result->time != -2) {
        end = clock();
        result->time = end - start;
    }
    delete[]send_buf;
    delete[]recv_buf;
    WSACleanup();
    return;
}