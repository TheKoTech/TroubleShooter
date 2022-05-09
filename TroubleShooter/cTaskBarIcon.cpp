#include "cTaskBarIcon.h"
#include "cFrame.h"
#include "wx/bitmap.h"
#include <string.h>

wxBEGIN_EVENT_TABLE(cTaskBarIcon, wxTaskBarIcon)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	//EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

cTaskBarIcon::cTaskBarIcon(wxApp* parent)
{
	this->parent = parent;
}

cTaskBarIcon::~cTaskBarIcon()
{
}

void cTaskBarIcon::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event)
{
	parent->ProcessEvent(event);
}

bool cTaskBarIcon::UpdateIcon(wxIcon* newIcon) { return SetIcon(wxBitmapBundle(*newIcon), this->m_strTooltip); }
bool cTaskBarIcon::UpdateIcon(wxIcon* newIcon, wxString* newTooltip) { return SetIcon(wxBitmapBundle(*newIcon), *newTooltip); }

void cTaskBarIcon::UpdateTooltip(wxString* newTooltip) { this->m_strTooltip = *newTooltip; }

