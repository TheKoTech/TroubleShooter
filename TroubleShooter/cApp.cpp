#include "cApp.h"

// cApp.cpp это имплементация контроллера.

#include <wx/msw/msvcrt.h>


wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_CLOSE(OnClosed)
	EVT_MENU(MENU_SHOW, OnTaskBarIconMenuShow)
	EVT_MENU(MENU_EXIT, OnTaskBarIconMenuClose)
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
	mainFrame = nullptr; // MEMORY LEAK
	chartController = ChartController(); // not leaking

	wxICOHandler* handler = new wxICOHandler(); // not leaking
	wxImage::AddHandler(handler); // not leaking
	UpdateIcon();
	return true;
}


void cApp::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event) { createFrame(); }
void cApp::OnClosed(wxCloseEvent& event)
{
	if (event.GetEventObject() == mainFrame)
	{
		closeFrame();
	}
}
void cApp::OnTaskBarIconMenuShow(wxCommandEvent& event) { createFrame(); }
void cApp::OnTaskBarIconMenuClose(wxCommandEvent& event) 
{ 
	closeFrame();
}

//Should be called on AppStatusChanged() or something.
bool cApp::UpdateIcon()
{
	wxString iconPath;
	switch (appStatus)
	{
	case green:
		iconPath = wxString("res/Icon_green.ico");
		break;
	case yellow:
		iconPath = wxString("res/Icon_yellow.ico");
		break;
	case red:
		iconPath = wxString("res/Icon_red.ico");
		break;
	case black:
		iconPath = wxString("res/Icon_black.ico");
		break;
	}

	wxIcon icon = wxIcon(iconPath, wxBITMAP_TYPE_ICO); // not leaking

	if (mainFrame != nullptr)
		mainFrame->SetIcon(icon); // nope

	// wxTaskBarIcon.SetIcon() это защищённый метод
	return taskBarIcon->UpdateIcon(icon, wxString("Pinger")); // not leaking
}


//This method is called on window start. It receives log data from a file, forms it into series and passes it to cFrame.
void cApp::initializeChartSeries()
{
	// data comes from a file
	wxVector<wxRealPoint> data = wxVector<wxRealPoint>();
	data.push_back(wxRealPoint(60, 127));
	data.push_back(wxRealPoint(55, 141));
	data.push_back(wxRealPoint(50, 137));
	data.push_back(wxRealPoint(45, 140));
	data.push_back(wxRealPoint(40, 2000));
	data.push_back(wxRealPoint(35, 2000));
	data.push_back(wxRealPoint(30, 1719));
	data.push_back(wxRealPoint(25, 145));
	data.push_back(wxRealPoint(20, 141));
	data.push_back(wxRealPoint(15, 135));
	data.push_back(wxRealPoint(10, 137));
	data.push_back(wxRealPoint(5, 129));
	data.push_back(wxRealPoint(0, 133));

	// serie goes to Frame
	chartController.CreateSerie(ChartController::dsLAN, data);
}

void cApp::updateChartSerie()
{
	//dsController.UpdateSerie(id, newPoint);
}

void cApp::createFrame()
{
	if (mainFrame == nullptr) {
		mainFrame = new cFrame(this, chartController.CreateChart());
		UpdateIcon();
		mainFrame->Show();
		mainFrame->Raise();
		initializeChartSeries();
	}
	else
	{
		mainFrame->Raise();
	}
}
void cApp::closeFrame()
{
	if (mainFrame != nullptr) {
		chartController.ClearDataset();
		mainFrame->Destroy();
		mainFrame = nullptr;
	}
}

wxIMPLEMENT_APP(cApp);