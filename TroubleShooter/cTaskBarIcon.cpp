#include "cTaskBarIcon.h"
#include "wx/bitmap.h"

wxBEGIN_EVENT_TABLE(cTaskBarIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_TASKBAR_RIGHT_UP(OnTaskBarIconRightUp)
	EVT_MENU(MENU_SHOW, Show)
	EVT_MENU(MENU_EXIT, Close)
wxEND_EVENT_TABLE()




cTaskBarIcon::cTaskBarIcon(wxApp* parent)
{
	this->parent = parent;

}

cTaskBarIcon::~cTaskBarIcon()
{
}

void cTaskBarIcon::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event) { parent->ProcessEvent(event); }
void cTaskBarIcon::OnTaskBarIconRightUp(wxTaskBarIconEvent& event) 
{ 
	wxMenu menu;

	wxMenuItem* itemShow = new wxMenuItem(&menu, MENU_SHOW, wxString("Show"), wxString("Show the app window"));
	wxMenuItem* itemClose = new wxMenuItem(&menu, MENU_EXIT, wxString("Exit"), wxString("Exit the app"));

	menu.Append(itemShow);
	menu.Append(itemClose);

	PopupMenu(&menu);
}

void cTaskBarIcon::Show(wxCommandEvent& event) { parent->ProcessEvent(event); }
void cTaskBarIcon::Close(wxCommandEvent& event) {
	parent->ProcessEvent(event);
	this->Destroy();
}

/**
 * Change cTaskBarIcon's image.
 * @param newIcon - new icon.
 */
bool cTaskBarIcon::UpdateIcon(wxIcon newIcon) { return SetIcon(wxBitmapBundle(newIcon), this->m_strTooltip); }

/**
 * Change cTaskBarIcon's image and tooltip.
 * @param newIcon - new icon.
 * @param newTooltip - new tooltip.
 */
bool cTaskBarIcon::UpdateIcon(wxIcon newIcon, wxString newTooltip) { return SetIcon(wxBitmapBundle(newIcon), newTooltip); }

/**
 * Change cTaskBarIcon's tooltip.
 * @param newTooltip - new tooltip.
 */
void cTaskBarIcon::UpdateTooltip(wxString newTooltip) { this->m_strTooltip = newTooltip; }


