#pragma once
#include "wx/thread.h"
#include "wx/app.h"
#include "wx/timer.h"
#include "MultiPing.h"
#include "Logger.h"
#include <list>
#include <string>

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
	PingController(wxApp* handler, int timeout, std::list<wxString>* addressList);
	~PingController();
private:



	wxApp* handler;
	int timeout; // Thread is updated every Timeout milliseconds
	std::list<wxString>* addressList; // todo: turn into a struct with AddressType (LAN, DNS...)

protected:
	
	/// Acts like the "main" function. 
	virtual ExitCode Entry();

public:
	int CountAddresses();

	// Unneeded?
	//private:
	//	wxDECLARE_EVENT_TABLE();
};

