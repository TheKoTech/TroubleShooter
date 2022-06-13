#include "Ping.h"

#include <WS2tcpip.h>
#include <iphlpapi.h>
#include <IcmpAPI.h>

#include <iostream>

#pragma comment(lib, "Iphlpapi.lib")

#pragma region Constructor

SinglePingThread::SinglePingThread(const wxString& domain, const int timeout) :
    wxThread(wxTHREAD_JOINABLE)
{
    domain_ = domain;
    timeout_ = timeout;
    ping_time_ = -1;
}

SinglePingThread::~SinglePingThread()
{
    Exit();
}

#pragma endregion Constructor

wxThread::ExitCode SinglePingThread::Entry()
{
    ping_time_ = ping();
    return nullptr;
}

int SinglePingThread::ping() const
{
    //Get host by domain
    addrinfo* domain_info = nullptr;
    const DWORD status = getaddrinfo(
        domain_,
        nullptr,
        nullptr,
        &domain_info);
    if (status != 0) { throw; }
    if (domain_info->ai_family != 2) { throw; }

    // Create the ICMP context.
    const void* icmp_handle = IcmpCreateFile();
    if (icmp_handle == INVALID_HANDLE_VALUE) { throw; }

    // Payload to send.
    constexpr WORD payload_size = 8;
    unsigned char payload[payload_size]{42};

    // Reply buffer for exactly 1 echo reply, payload data, and 8 bytes for ICMP error message.
    constexpr DWORD reply_buffer_size = sizeof ICMP_ECHO_REPLY + payload_size + 8;
    unsigned char reply_buffer[reply_buffer_size]{};

    // Make the echo request.
    const DWORD reply_count = IcmpSendEcho(
        const_cast<HANDLE>(icmp_handle),
        reinterpret_cast<sockaddr_in*>(domain_info->ai_addr)->sin_addr.S_un.S_addr,
        payload,
        payload_size,
        nullptr,
        reply_buffer,
        reply_buffer_size,
        timeout_);

    // Return value of 0 indicates failure, try to get error info.
    if (reply_count == 0)
    {
        const auto e = GetLastError();
        DWORD buffer_size = 1000;
        WCHAR buffer[1000];
        GetIpErrorString(e, buffer, &buffer_size);
        std::cout << "IcmpSendEcho returned error " << e << " (" << buffer << ")" << std::endl;
        return 0;
    }

    const auto echo_reply = reinterpret_cast<const ICMP_ECHO_REPLY*>(reply_buffer);
    /* IN_ADDR addr;
    addr.S_un.S_addr = echo_reply->Address;
    constexpr DWORD addr_buffer_size = 32;
    char addr_buffer[addr_buffer_size]{};
    const char* s_ip = inet_ntop(AF_INET, &addr, addr_buffer, addr_buffer_size);
    std::cout << "Reply from: " << s_ip << " time=" << echo_reply->RoundTripTime << "ms" << std::endl; */

    // Close ICMP context.
    IcmpCloseHandle(const_cast<HANDLE>(icmp_handle));
    return static_cast<int>(echo_reply->RoundTripTime);
}

int SinglePingThread::get_ping_time() const
{
    return ping_time_;
}

wxString SinglePingThread::get_domain() const
{
    return domain_;
}
