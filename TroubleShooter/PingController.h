#pragma once
#include "Ping.h"

#include "wx/thread.h"
#include "wx/app.h"

#define PING_THREAD_UPDATED 16000
#define PING_THREAD_COMPLETED 16001

/// The main thread to run all pinging classes and functions 
class PingController final : public wxThread
{
public:
	/// The main thread to run all pinging classes and functions 
	/// @param parent - controller (cApp)
	/// @param domains - list of domains that will be pinged
	/// @param timeout - the thread will update every Timeout milliseconds
	PingController(wxApp* parent, std::vector<wxString>* domains, int timeout);
	~PingController() override;
	
private:
	wxApp* parent_;
	int timeout_;
	std::vector<wxString>* domains_;
	ping_res* pings_results_;

protected:
	/// Acts like the "main" function. 
	ExitCode Entry() override;

public:
	unsigned int domains_count() const;
	ping_res* get_ping_results() const;
};

