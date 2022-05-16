#include "cTaskBarIcon.h"
#include "wx/bitmap.h"
#include "Colors.cpp"

wxBEGIN_EVENT_TABLE(cTaskBarIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_TASKBAR_RIGHT_UP(OnTaskBarIconRightUp)
	EVT_MENU(MENU_SHOW, Show)
	EVT_MENU(MENU_SETTINGS, ShowSettings)
	EVT_MENU(MENU_EXIT, Close)
wxEND_EVENT_TABLE()




cTaskBarIcon::cTaskBarIcon(wxApp* parent)
{
	this->parent = parent;
}

void cTaskBarIcon::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event) { parent->ProcessEvent(event); }
void cTaskBarIcon::OnTaskBarIconRightUp(wxTaskBarIconEvent& event) 
{ 
	wxMenu menu;

	wxMenuItem* itemShow = new wxMenuItem(&menu, MENU_SHOW, wxString("Show"), wxString("Show the app window"));
	wxMenuItem* itemSettings = new wxMenuItem(&menu, MENU_SETTINGS, wxString("Settings"), wxString("Show the settings window"));
	wxMenuItem* itemExit = new wxMenuItem(&menu, MENU_EXIT, wxString("Exit"), wxString("Exit the app"));

	menu.Append(itemShow);
	menu.Append(itemSettings);
	menu.AppendSeparator();
	menu.Append(itemExit);

	PopupMenu(&menu);
}

void cTaskBarIcon::Show(wxCommandEvent& event) { parent->ProcessEvent(event); }
void cTaskBarIcon::ShowSettings(wxCommandEvent& event)
{
	//todo: open settings in cApp
	parent->ProcessEvent(event);
}
void cTaskBarIcon::Close(wxCommandEvent& event) {
	parent->ProcessEvent(event);
	this->Destroy();
}


bool cTaskBarIcon::UpdateIcon(wxIcon newIcon) { return SetIcon(wxBitmapBundle(newIcon), this->m_strTooltip); }
bool cTaskBarIcon::UpdateIcon(wxIcon newIcon, wxString newTooltip) { return SetIcon(wxBitmapBundle(newIcon), newTooltip); }
void cTaskBarIcon::UpdateTooltip(wxString newTooltip) { this->m_strTooltip = newTooltip; }