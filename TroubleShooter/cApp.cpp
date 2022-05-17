#include "cApp.h"

// ���� enum �������� ID ������� �����������.
// An enum to store controller event IDs.
enum ControllerEvents {
	TIMER = 60
};
//todo: move all event enums to a separate .cpp file?

wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_LEFT_UP(OnPanelLeftUp)
	EVT_CLOSE(OnClosed)
	EVT_MENU(MENU_SHOW, OnTaskBarIconMenuShow)
	EVT_MENU(MENU_SETTINGS, OnTaskBarIconMenuSettings)
	EVT_MENU(MENU_EXIT, OnTaskBarIconMenuClose)
	EVT_TIMER(TIMER, OnTimer)
	EVT_BUTTON(SETTINGS_APPLY, OnApplySettingsButtonClick)
	EVT_BUTTON(SETTINGS_DISCARD, OnDiscardSettingsButtonClick)
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

	int timeout = 1000;

	////// �������� ����������������� ���� //////
	int num_adresses = 7;
	string* adresses = new string[]{
		"yandex.ru",
		"google.com",
		"ru.wikipedia.com",
		"192.168.0.6",
		"rutracker.org",
		"hhhh",
		"iafisher.com"
	};
	PingRes* res_ping = new PingRes[num_adresses];
	MultiPing(res_ping, adresses, num_adresses, timeout);

	for (int i = 0; i < num_adresses; ++i) {
		Logger logger(res_ping[i].adress, res_ping[i].time);
		logger.WriteLog();
		logger.Check();
	}
	////// ����� �������� ����������������� ���� //////

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
	if (event.GetId() == FRAME_SETTINGS)
		createSettingsFrame();
	event.Skip();
}

void cApp::OnApplySettingsButtonClick(wxCommandEvent& event)
{

}

void cApp::OnDiscardSettingsButtonClick(wxCommandEvent& event)
{

}

void cApp::createSettingsFrame()
{
	if (settingsFrame == nullptr) {
		settingsFrame = new cSettingsFrame(this);
		UpdateIcon();
		settingsFrame->Show();
	}
	else
	{
		settingsFrame->Raise();
		settingsFrame->Maximize(false);
	}
}

void cApp::closeSettingsFrame()
{
	if (settingsFrame != nullptr) {
		settingsFrame->Destroy();
		settingsFrame = nullptr;
	}
}

void cApp::OnTimer(wxTimerEvent& event)
{
	// ���������� ��� ���������� �������
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

	auto icon = wxIcon(iconPath, wxBITMAP_TYPE_ICO);

	if (mainFrame != nullptr)
		mainFrame->SetIcon(icon);

	if (settingsFrame != nullptr)
		settingsFrame->SetIcon(wxIcon(wxString("res/Icon_settings.png")));

	return taskBarIcon->UpdateIcon(icon, wxString("Pinger"));
}


void cApp::initializeChartSeries(/*log file*/)
{
	// ������ ��������� �� ����� (������� �� � ������� ���������� ������ �����?)
	// Data comes from a file (read it with a separate method here?)

	// ������� � ��������� X ������ ��������� � 3 ��������� ����������� ��������, ������� ���� ������� ����������. ����� ������, ��� �� ������� ����� �������� ��� �����.
	// The X value difference must match the 3s timer update that we will hardcode as a const. Keep in mind, that we will implement an option to change that time further on.

	auto data = wxVector<wxRealPoint>();
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