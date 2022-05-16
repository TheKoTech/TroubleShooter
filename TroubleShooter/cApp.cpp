#include "cApp.h"

// Этот enum содержит ID событий контроллера.
// An enum to store controller event IDs.
enum ControllerEvents {
	TIMER = 60
};
//todo: move all event enums to a separate .cpp file

wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_LEFT_UP(OnPanelLeftUp)
	EVT_CLOSE(OnClosed)
	EVT_MENU(MENU_SHOW, OnTaskBarIconMenuShow)
	EVT_MENU(MENU_SETTINGS, OnTaskBarIconMenuSettings)
	EVT_MENU(MENU_EXIT, OnTaskBarIconMenuClose)
	EVT_TIMER(TIMER, OnTimer)
wxEND_EVENT_TABLE()

// not used
cApp::cApp() = default;
cApp::~cApp() = default;


bool cApp::OnInit() 
{
	appStatus = green;

	taskBarIcon = new cTaskBarIcon(this); 
	mainFrame = nullptr; // hidden on start
	chartController = ChartController();
	settingsFrame = nullptr;

	// those are required for image files to be loaded
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxICOHandler); 
	UpdateIcon();
	return true;
}


void cApp::OnTaskBarIconLeftUp(wxTaskBarIconEvent& event) { createFrame(); }
void cApp::OnClosed(wxCloseEvent& event)
{
	// cFrame window closing is managed here in the controller, not in the class itself
	if (event.GetEventObject() == mainFrame)
		closeFrame();
	if (event.GetEventObject() == settingsFrame)
		closeSettingsFrame();
}
void cApp::OnTaskBarIconMenuShow(wxCommandEvent& event) { createFrame(); }
void cApp::OnTaskBarIconMenuSettings(wxCommandEvent& event) { createSettingsFrame(); }
void cApp::OnTaskBarIconMenuClose(wxCommandEvent& event) 
{ 
	closeFrame(); 
	closeSettingsFrame();
}

void cApp::OnPanelLeftUp(wxMouseEvent& event)
{
	//if (event.GetId() == 100) {
	//	// check if mouse is over the settings Panel-"Button"
	//	bool settings = mainFrame->settingsPanel->GetClientRect().Contains(event.GetPosition());

	//	if (settings) {
	//		createSettingsFrame();
	//	}
	//}
}

void cApp::createSettingsFrame()
{
	if (settingsFrame == nullptr) {
		settingsFrame = new cSettingsFrame(this);
		UpdateIcon();
		settingsFrame->Show();
		//settingsFrame->Raise();
	}
	else
	{
		if (settingsFrame->IsIconized())
		{
			settingsFrame->Maximize(false);
			settingsFrame->Raise();
		}
		else
			settingsFrame->Iconize();
	}
}

void cApp::closeSettingsFrame()
{
	settingsFrame->Destroy();
	settingsFrame = nullptr;
}

void cApp::OnTimer(wxTimerEvent& event)
{
	// Происходит при обновлении таймера
}

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
	default: 
		iconPath = wxString("res/Icon_black.ico");
		break;
	}

	wxIcon icon = wxIcon(iconPath, wxBITMAP_TYPE_ICO);

	if (mainFrame != nullptr)
		mainFrame->SetIcon(icon);

	if (settingsFrame != nullptr)
		settingsFrame->SetIcon(wxIcon(wxString("res/Icon_settings.png")));

	return taskBarIcon->UpdateIcon(icon, wxString("Pinger"));
}


void cApp::initializeChartSeries(/*log file*/)
{
	// Данные поступают из файла (считать их с помощью отдельного метода здесь?)
	// Data comes from a file (read it with a separate method here?)

	// Разница в значениях X должна совпадать с 3 секундным обновлением таймером, который пока оставим константой. Нужно учесть, что мы добавим опцию изменить это время.
	// The X value difference must match the 3s timer update that we will hardcode as a const. Keep in mind, that we will implement an option to change that time further on.

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
		if (mainFrame->IsIconized())
		{
			mainFrame->Maximize(false);
			mainFrame->Raise();
		}
		else
			mainFrame->Iconize();
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