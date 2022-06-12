#include "Ping.h"
#include "PingService.h"

#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <IcmpAPI.h>

#include <iostream>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")


#define DEFAULT_TTL 30
#define MAX_PING_DATA_SIZE 1024

SinglePingThread::SinglePingThread() : wxThread(wxTHREAD_JOINABLE), result_(nullptr), timeout_(0)
{
}

void SinglePingThread::set_params(ping_res* result, wxString host, const int timeout)
{
    this->result_ = result;
    this->host_ = host;
    this->timeout_ = timeout;
}

void* SinglePingThread::Entry()
{
    ping(host_.ToUTF8().data());
    return nullptr;
}

ping_res* multi_ping(std::vector<wxString> addresses, int num_addresses, int timeout)
{
    auto results = new ping_res[num_addresses];
    auto pt = new SinglePingThread[num_addresses];
    for (int i = 0; i < num_addresses; ++i)
    {
        results[i].domain = addresses[i];
        pt[i].set_params(&results[i], addresses[i], timeout);
        pt[i].Create();
        pt[i].Run();
    }

    time_t start = clock();
    bool exist_alive = true;
    while (exist_alive)
    {
        wxThread::This()->Sleep(timeout / 10);
        exist_alive = false;
        for (int i = 0; i < num_addresses; ++i)
            if (pt[i].IsAlive())
            {
                if (clock() - start < timeout)
                    exist_alive = true;
                else
                {
                    pt[i].Delete();
                    results[i].ping_time = -1;
                }
            }
    }

    return results;
}

int ping(const char* host)
{
    // Create the ICMP context.
    HANDLE icmp_handle = IcmpCreateFile();
    if (icmp_handle == INVALID_HANDLE_VALUE) { throw; }

    // Parse the destination IP address.
    IN_ADDR dest_ip{};
    if (InetPtonA(AF_INET, host, &dest_ip) != 1) { throw; }

    // Payload to send.
    constexpr WORD payload_size = 1;
    unsigned char payload[payload_size]{42};

    // Reply buffer for exactly 1 echo reply, payload data, and 8 bytes for ICMP error message.
    constexpr DWORD reply_buffer_size = sizeof ICMP_ECHO_REPLY + payload_size + 8;
    unsigned char reply_buffer[reply_buffer_size]{};

    // Make the echo request.
    const DWORD reply_count = IcmpSendEcho(
        icmp_handle,
        dest_ip.S_un.S_addr,
        payload,
        payload_size,
        nullptr,
        reply_buffer,
        reply_buffer_size,
        10000);

    // Return value of 0 indicates failure, try to get error info.
    if (reply_count == 0)
    {
        const auto e = GetLastError();
        DWORD buffer_size = 1000;
        WCHAR buf[1000];
        GetIpErrorString(e, buf, &buffer_size);
        std::cout << "IcmpSendEcho returned error " << e << " (" << buf << ")" << std::endl;
        return 255;
    }

    const auto echo_reply = reinterpret_cast<const ICMP_ECHO_REPLY*>(reply_buffer);
    IN_ADDR addr{};
    addr.S_un.S_addr = echo_reply->Address;
    constexpr DWORD addr_buffer_size = 32;
    char addr_buffer[addr_buffer_size]{};
    const char* s_ip = InetNtopA(AF_INET, &addr, addr_buffer, addr_buffer_size);
    std::cout << "Reply from: " << s_ip << ": bytes=" << echo_reply->DataSize
        << " time=" << echo_reply->RoundTripTime << "ms TTL=" << static_cast<int>(echo_reply->Options.Ttl) << std::endl;

    // Close ICMP context.
    IcmpCloseHandle(icmp_handle);
    return 0;
}

void ping(ping_res* result, wxString host)
{
    ICMPHeader* send_buf = nullptr;
    IPHeader* recv_buf = nullptr;

    constexpr int packet_size = MAX_PING_DATA_SIZE;
    WSAData wsa_data{};
    WSAStartup(MAKEWORD(2, 1), &wsa_data);

    bool ping_finished = false;
    SOCKET sd;
    sockaddr_in dest, source;
    if (setup_for_ping(host.ToUTF8().data(), DEFAULT_TTL, sd, dest) < 0)
    {
        result->ping_time = -2;
        ping_finished = true;
    }
    if (!ping_finished && allocate_buffers(send_buf, recv_buf, packet_size) < 0)
    {
        result->ping_time = -2;
        ping_finished = true;
    }
    if (!ping_finished)
    {
        int seq_no = 0;
        init_ping_packet(send_buf, packet_size, seq_no);

        time_t start = clock();
        if (send_ping(sd, dest, send_buf, packet_size) >= 0)
        {
            while (!ping_finished)
            {
                bool ShouldDecodeRes = true;
                if (recv_ping(sd, source, recv_buf, MAX_PING_PACKET_SIZE) < 0)
                {
                    result->ping_time = -2;

                    unsigned short header_len = recv_buf->h_len * 4;
                    auto icmphdr = (ICMPHeader*)((char*)recv_buf + header_len);
                    if (icmphdr->seq != seq_no)
                    {
                        ShouldDecodeRes = false;
                    }
                    else
                    {
                        result->ping_time = -2;
                        ping_finished = true;
                    }
                }
                if (ShouldDecodeRes)
                {
                    int decode_res = decode_reply(recv_buf, packet_size, &source);
                    if (decode_res != -2)
                    {
                        if (decode_res == -1)
                        {
                            result->ping_time = -2;
                        }
                        ping_finished = true;
                    }
                }
            }
        }
        else
        {
            result->ping_time = -2;
        }
        if (result->ping_time > -2)
        {
            result->ping_time = clock() - start;
        }
    }

    delete[]send_buf;
    delete[]recv_buf;
    WSACleanup();
}
