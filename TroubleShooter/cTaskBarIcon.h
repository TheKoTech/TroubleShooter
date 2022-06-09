#pragma once
#include "wx/wx.h"
#include <wx/taskbar.h>


class cTaskBarIcon : wxTaskBarIcon
{
public:
	cTaskBarIcon(wxApp* handler);

	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnTaskBarIconRightUp(wxTaskBarIconEvent& event);
	void Show(wxCommandEvent& event);
	void ShowSettings(wxCommandEvent& event);
	void Close(wxCommandEvent& event);

	// Change cTaskBarIcon's image.
	bool UpdateIcon(wxIcon newIcon);

	// Change cTaskBarIcon's image and tooltip.
	bool UpdateIcon(wxIcon newIcon, wxString newTooltip);

	void UpdateTooltip(wxString newTooltip);

private:
	wxApp* parent;
	wxDECLARE_EVENT_TABLE();
};
