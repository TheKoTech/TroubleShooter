#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/gbsizer.h>
#include "Colors.cpp"

class cSettingsFrame : public wxFrame
{
public:
	cSettingsFrame(wxApp* parent);
	~cSettingsFrame();

private:
	wxApp* parent = nullptr;

	wxPanel* mPanel = nullptr;
	wxTextCtrl* lanInput = nullptr;
	wxTextCtrl* ispInput = nullptr;
	wxTextCtrl* dnsInput = nullptr;
	wxTextCtrl* hostInput = nullptr;

	void initializeUI();
	void applyFrameSettings();

	void OnClosed(wxCloseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

