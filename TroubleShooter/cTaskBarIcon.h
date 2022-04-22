#pragma once
#include "wx/wx.h"
#include <wx/taskbar.h>
#include "cFrame.h"

class cTaskBarIcon : wxTaskBarIcon
{
public:
	cTaskBarIcon(wxApp* parent);
	~cTaskBarIcon();

	void OnTaskBarIconLeftClick(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);

	bool UpdateIcon(wxIcon* newIcon);
	bool UpdateIcon(wxIcon* newIcon, wxString* newTooltip);
	void UpdateTooltip(wxString* newTooltip);

private:
	wxApp* parent;
	wxDECLARE_EVENT_TABLE();
};

