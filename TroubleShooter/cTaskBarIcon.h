#pragma once
#include "wx/wx.h"
#include <wx/taskbar.h>

enum MenuItems {
	MENU_SHOW = 6000,
	MENU_SETTINGS = 6001,
	MENU_EXIT = 6002
};

class cTaskBarIcon : wxTaskBarIcon
{
public:
	cTaskBarIcon(wxApp* parent);

	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnTaskBarIconRightUp(wxTaskBarIconEvent& event);
	void Show(wxCommandEvent& event);
	void ShowSettings(wxCommandEvent& event);
	void Close(wxCommandEvent& event);

	/**
	 * Change cTaskBarIcon's image.
	 * @param newIcon - new icon.
	 */
	bool UpdateIcon(wxIcon newIcon);

	/**
	 * Change cTaskBarIcon's image and tooltip.
	 * @param newIcon - new icon.
	 * @param newTooltip - new tooltip.
	 */
	bool UpdateIcon(wxIcon newIcon, wxString newTooltip);

	/**
	 * Change cTaskBarIcon's tooltip.
	 * @param newTooltip - new tooltip.
	 */
	void UpdateTooltip(wxString newTooltip);

private:
	wxApp* parent;
	wxDECLARE_EVENT_TABLE();
};
