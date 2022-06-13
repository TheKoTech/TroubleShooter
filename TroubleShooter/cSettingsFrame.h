#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/gbsizer.h>
#include "app_ids.h"

enum {
	SETTINGS_APPLY =	SETTINGS_FRAME + 1,
	SETTINGS_DISCARD =	SETTINGS_FRAME + 2,
	SETTINGS_LAN =		SETTINGS_FRAME + 3,
	SETTINGS_ISP =		SETTINGS_FRAME + 4,
	SETTINGS_DNS =		SETTINGS_FRAME + 5,
	SETTINGS_HOST =		SETTINGS_FRAME + 6,
};

class cSettingsFrame : public wxFrame
{
public:
	cSettingsFrame(wxApp* parent, wxVector<wxString>* addresses);
	~cSettingsFrame();

	wxVector<wxString>* GetNewAdresses();
private:
	wxApp* parent = nullptr;

	bool settingsChanged;
	wxTextCtrl* lanInput = nullptr;
	wxTextCtrl* ispInput = nullptr;
	wxTextCtrl* dnsInput = nullptr;
	wxTextCtrl* hostInput = nullptr;

	void initializeUI(wxVector<wxString>* addresses);
	void applyFrameSettings();


	void OnApplyButtonClick(wxCommandEvent& event);
	void OnDiscardButtonClick(wxCommandEvent& event);
	void OnClosed(wxCloseEvent& event);

	wxDECLARE_EVENT_TABLE();
};
