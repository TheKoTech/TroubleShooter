#pragma once
#include <wx/thread.h>
#include <wx/app.h>
#include <wx/timer.h>
#include "Ping.h"
#include "Logger.h"
#include "app_ids.h"

enum {
	PING_THREAD_UPDATED = PING_THREAD + 1,
	PING_THREAD_COMPLETED = PING_THREAD + 2,
};

/// The main thread to run all pinging classes and functions 
class PingController : public wxThread
{
public:
	/// The main thread to run all pinging classes and functions 
	/// @param parent - controller (cApp)
	/// @param timeout - the thread will update every Timeout milliseconds
	/// @param addressList - list of addressList that will be pinged
	PingController(wxApp* handler, int timeout, std::vector<wxString>* addressList);
	~PingController();
private:



	wxApp* handler;
	int timeout; // Thread is updated every Timeout milliseconds
	std::vector<wxString>* addressVector; // TODO: turn into a struct with AddressType (LAN, DNS...)
	PingRes* pings_results;

protected:

	/// Acts like the "main" function. 
	virtual ExitCode Entry();

public:
	int CountAddresses();
	PingRes* GetPingResults();

	// Unneeded?
	//private:
	//	wxDECLARE_EVENT_TABLE();
};

