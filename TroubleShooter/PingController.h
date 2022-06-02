#pragma once
#include "wx/thread.h"
#include "wx/app.h"
#include "wx/timer.h"
#include "Ping.h"
#include "Logger.h"

enum MainPingThreadEvents {
	PING_THREAD_UPDATED = 16000,
	PING_THREAD_COMPLETED = 16001,
};

/// The main thread to run all pinging classes and functions 
class PingController : public wxThread
{
public:
	/// The main thread to run all pinging classes and functions 
	/// @param handler - controller (cApp)
	/// @param timeout - the thread will update every Timeout milliseconds
	/// @param addressList - list of addressList that will be pinged
	PingController(wxApp* handler, int timeout, std::vector<wxString>* addressList);
	~PingController();
private:



	wxApp* handler;
	int timeout; // Thread is updated every Timeout milliseconds
	std::vector<wxString>* addressVector; // TODO: turn into a struct with AddressType (LAN, DNS...)
	std::vector<PingRes> pings_results;
	std::vector<PingRes> pings_results_prev;
public:

protected:

	/// Acts like the "main" function. 
	virtual ExitCode Entry();

public:
	int CountAddresses();
	std::vector<PingRes>* GetPingResults();
	void SetAddresses(wxVector<wxString>*);

	// Unneeded?
	//private:
	//	wxDECLARE_EVENT_TABLE();
};

