#include "cFrame.h"
#include "wx/display.h"

wxBEGIN_EVENT_TABLE(cFrame, wxFrame)
EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

const int frameWidth = 500;
const int frameHeight = 300;

cFrame::cFrame(wxApp* parent) : wxFrame(
	nullptr, 
	wxID_ANY, 
	"App prototype", 
	wxPoint(
		wxDisplay().GetGeometry().GetWidth() - frameWidth - 5, 
		wxDisplay().GetGeometry().GetHeight() - frameHeight - 45),
	wxSize(frameWidth, frameHeight))
{
	this->parent = parent;
}

cFrame::~cFrame()
{
}

void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
}

void cFrame::LoadIcon(wxIcon* icon)
{
	SetIcon(*icon);
}
