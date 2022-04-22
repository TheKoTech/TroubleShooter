#include "cApp.h"

wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftClick)
	EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

enum AppStatus { green, yellow, red, black };

cApp::cApp()
{
}
cApp::~cApp()
{
}

bool cApp::OnInit() 
{
	appStatus = green;

	taskBarIcon = new cTaskBarIcon(this);
	mainFrame = nullptr;

	UpdateIcon();
	return true;
}


void cApp::OnTaskBarIconLeftClick(wxTaskBarIconEvent& event)
{
	if (mainFrame == nullptr) {
		mainFrame = new cFrame(this);
	}
	UpdateIcon();
	mainFrame->Show();
}

void cApp::OnClosed(wxCloseEvent& event)
{
	if (event.GetEventObject() == mainFrame)
	{
		mainFrame->Destroy();
		mainFrame = nullptr;
	}
}

bool cApp::UpdateIcon()
{
	wxICOHandler* handler = new wxICOHandler;
	wxImage::AddHandler(handler);

	std::string iconPath = "res/Icon_";
	switch (appStatus)
	{
	case green:
		iconPath += "green.ico";
		break;
	case yellow:
		iconPath += "yellow.ico";
		break;
	case red:
		iconPath += "red.ico";
		break;
	case black:
		iconPath += "black.ico";
		break;
	}

	wxIcon* icon = new wxIcon(iconPath, wxBITMAP_TYPE_ICO);

	if (mainFrame != nullptr)
		mainFrame->SetIcon(*icon);

	// wxTaskBarIcon.SetIcon() это защищённый метод
	return taskBarIcon->UpdateIcon(icon, new wxString(std::string("Pinger")));
}

wxIMPLEMENT_APP(cApp);