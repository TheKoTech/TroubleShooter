#include "cApp.h"

// cApp.cpp это имплементация контроллера.


wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
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
	chartController = ChartController();

	wxICOHandler* handler = new wxICOHandler;
	wxImage::AddHandler(handler);
	UpdateIcon();
	return true;
}


void cApp::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event)
{
	//todo: wrap it into an "OnWindowOpen()" event ?
	if (mainFrame == nullptr) {
		mainFrame = new cFrame(this, chartController.CreateChart());
		UpdateIcon(); 
		mainFrame->Show();
		mainFrame->Raise();
		initializeChartSeries();
	}
	else
	{
		mainFrame->Close();
	}
}

void cApp::OnClosed(wxCloseEvent& event)
{
	if (event.GetEventObject() == mainFrame)
	{
		chartController.ClearDataset();
		mainFrame->Destroy();
		mainFrame = nullptr;
	}
}

/* 
  Should be called on AppStatusChanged() or something.
*/
bool cApp::UpdateIcon()
{
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

/*
  This method is called on window start. It receives log data from a file, forms it into series and passes it to cFrame.
*/
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

/*
This has to be called each App update (2-3sec)
*/
void cApp::updateChartSerie()
{
	//dsController.UpdateSerie(id, newPoint);
}

wxIMPLEMENT_APP(cApp);