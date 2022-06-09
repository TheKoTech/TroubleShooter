#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/gbsizer.h>


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
