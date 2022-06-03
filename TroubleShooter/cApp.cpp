#include "cApp.h"

// Этот enum содержит ID событий контроллера.
// An enum to store controller event IDs.
enum ControllerEvents {
	TIMER = 60,
};
//todo: move all event enums to a separate .cpp file?

wxBEGIN_EVENT_TABLE(cApp, wxApp)
	EVT_TASKBAR_LEFT_UP(OnTaskBarIconLeftUp)
	EVT_LEFT_UP(OnPanelLeftUp)
	EVT_CLOSE(OnClosed)
	EVT_MENU(MENU_SHOW, OnTaskBarIconMenuShow)
	EVT_MENU(MENU_SETTINGS, OnTaskBarIconMenuSettings)
	EVT_MENU(MENU_EXIT, OnTaskBarIconMenuClose)
	EVT_THREAD(PING_THREAD_UPDATED, OnThreadUpdate)
	EVT_THREAD(PING_THREAD_COMPLETED, OnThreadUpdate)
	EVT_BUTTON(SETTINGS_APPLY, OnApplyButtonLeftUp)
wxEND_EVENT_TABLE()

// not used
cApp::cApp() = default;
cApp::~cApp() = default;


bool cApp::OnInit()
{
	appStatus = green;

	taskBarIcon = new cTaskBarIcon(this);
	chartController = ChartController();
	initializePingController();

	// those are required for image files to be loaded
	wxImage::AddHandler(new wxPNGHandler);
	wxImage::AddHandler(new wxICOHandler);
	UpdateIcon();


	return true;
}


void cApp::OnTaskBarIconLeftUp(wxTaskBarIconEvent & event) { createFrame(); }
void cApp::OnClosed(wxCloseEvent & event)
{
	// cFrame window closing is managed here in the controller, not in the class itself
	if (event.GetEventObject() == mainFrame)
		closeFrame();
	else if (event.GetEventObject() == settingsFrame)
		closeSettingsFrame();
}
void cApp::OnTaskBarIconMenuShow(wxCommandEvent & event) { createFrame(); }
void cApp::OnTaskBarIconMenuSettings(wxCommandEvent& event) { createSettingsFrame(); }
void cApp::OnTaskBarIconMenuClose(wxCommandEvent& event) 
{ 
	closeFrame(); 
	closeSettingsFrame();
	if (pingController) {
		pingController->Delete();
	}
}

void cApp::OnPanelLeftUp(wxMouseEvent& event)
{
	if (event.GetId() == FRAME_SETTINGS)
		createSettingsFrame();
	event.Skip();
}

void cApp::OnApplyButtonLeftUp(wxCommandEvent& event)
{
	ConfigController config = ConfigController(this);
	auto addresses = settingsFrame->GetNewAdresses();
	config.WriteAddresses(addresses);
	delete addresses;
}

void cApp::OnThreadUpdate(wxThreadEvent & event)
{
	std::vector<PingRes>* pings_results = pingController->GetPingResults();
	int a = 0;

	for (int i = 0; i < 4; ++i) {
		Logger logger(pings_results->at(i).address, pings_results->at(i).time, i);
		logger.WriteLog();
		logger.Check();
	}
}

void cApp::OnThreadCompleted(wxThreadEvent & event)
{

}

void cApp::createSettingsFrame()
{
	if (settingsFrame == nullptr) {
		auto config = ConfigController(this);
		auto addresses = config.GetAddressList();
		settingsFrame = new cSettingsFrame(this, addresses);
		settingsFrame->SetIcon(wxIcon(wxString("res/Icon_settings.ico"), wxBITMAP_TYPE_ICO));
		settingsFrame->Show();

		delete addresses;
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
		int timeout = 3000; //todo: save as a config
		ConfigController config = ConfigController(this);
		wxVector<wxString>* addressesSettings = settingsFrame->GetNewAdresses();

		pingController->SetAddresses(addressesSettings);

		addressesSettings->clear();
		settingsFrame->Destroy();
		settingsFrame = nullptr;
	}
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
		iconPath = wxString("res/Icon_black.ico");
		break;
	}

	auto icon = wxIcon(iconPath, wxBITMAP_TYPE_ICO);

	if (mainFrame != nullptr)
		mainFrame->SetIcon(icon);

	return taskBarIcon->UpdateIcon(icon, wxString("Pinger"));
}


void cApp::initializeChartSeries(/*log file*/)
{
	// Данные поступают из файла (считать их с помощью отдельного метода здесь?)
	// Data comes from a file (read it with a separate method here?)

	// Разница в значениях X должна совпадать с 3 секундным обновлением таймером, который пока оставим константой. Нужно учесть, что мы добавим опцию изменить это время.
	// The X value difference must match the 3s timer update that we will hardcode as a const. Keep in mind, that we will implement an option to change that time further on.

	wxString str;
	wxString filename = "ping_res.csv";
	wxTextFile filein;
	//wxFile fileout;
	filein.Open(filename);
	int num = filein.GetLineCount();
	//str = filein.GetLine(num-3);
	wxArrayString rows;
	int s = 0;
	int count;
	if (num >= 84) {
		count = 84;
	}
	else
		count = num;
	auto data = wxVector<wxRealPoint>();
	auto data1 = wxVector<wxRealPoint>();
	auto data2 = wxVector<wxRealPoint>();
	auto data3 = wxVector<wxRealPoint>();
	int type = 0;
	for (int i = num - count; i < num; i++) {
		str = filein.GetLine(i);
		rows = wxSplit(str, ';');
		if (atoi((rows[4].c_str())) == 0) {
			data.push_back(wxRealPoint(s, atoi((rows[1].c_str()))));
		}
		else if (atoi((rows[4].c_str())) == 1) {
			data1.push_back(wxRealPoint(s, atoi((rows[1].c_str()))));
		}
		else if (atoi((rows[4].c_str())) == 2) {
			data2.push_back(wxRealPoint(s, atoi((rows[1].c_str()))));
		}
		else if (atoi((rows[4].c_str())) == 3) {
			data3.push_back(wxRealPoint(s, atoi((rows[1].c_str()))));
		}

		type++;
		if (type == 4) {
			type = 0;
			s += 3;
		}
	}

	// serie goes to Frame
	chartController.CreateSeries(data, data1, data2, data3);
	//chartController.CreateSerie(ChartController::dsLAN, data);
	//chartController.CreateSerie(ChartController::dsISP, data1);
	//chartController.CreateSerie(ChartController::dsDNS, data2);
	//chartController.CreateSerie(ChartController::dsDomain, data3);
}

void cApp::initializePingController()
{
	int timeout = 3000; //todo: save as a config
	ConfigController config = ConfigController(this);
	wxVector<wxString>* addressesConfig = config.GetAddressList();
	auto addressVector = new std::vector<wxString>();

	addressVector->push_back(wxString(addressesConfig->at(0)));
	addressVector->push_back(wxString(addressesConfig->at(1)));
	addressVector->push_back(wxString(addressesConfig->at(2)));
	addressVector->push_back(wxString(addressesConfig->at(3)));
	pingController = new PingController(this, timeout, addressVector);
	pingController->Create();
	pingController->Run();

	addressesConfig->clear();
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