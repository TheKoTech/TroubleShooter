#include "cFrame.h"
#include "ThemeDefault.h"


wxBEGIN_EVENT_TABLE(cFrame, wxFrame)
EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

const int clientWidth = 700;
const int clientHeight = 320;
const wxSize clientSize = wxSize(clientWidth, clientHeight);

cFrame::cFrame(wxApp* parent, Chart* chart) : wxFrame(nullptr, wxID_ANY, "App prototype",
	wxDefaultPosition,
	wxDefaultSize,
	wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{
	this->parent = parent;
	this->chart = chart;

	currentTab = TAB_NONE;
	
	initializeUI();
	
	this->SetClientSize(clientSize);
	this->SetMinClientSize(clientSize);
	wxSize windowSize = this->ClientToWindowSize(clientSize);
	this->SetPosition(wxPoint(
		wxDisplay().GetClientArea().GetWidth() - windowSize.GetWidth() - 10,
		wxDisplay().GetClientArea().GetHeight() - windowSize.GetHeight() - 10));
}

cFrame::~cFrame() = default;

void cFrame::initializeUI() 
{
	mainPanel = new wxPanel(this);
	contentPanel = new wxPanel(mainPanel);
	statusbarPanel = new wxPanel(mainPanel);

	mainSizer = new wxBoxSizer(wxVERTICAL);
	//titleBarSizer = new wxBoxSizer(wxHORIZONTAL);
	statusBarSizer = new wxBoxSizer(wxVERTICAL);
	contentSizer = new wxBoxSizer(wxHORIZONTAL);

	// Create the actual Controls
	createContent();
	createStatusBar();



	mainSizer->Add(contentPanel, 1, wxGROW);
	mainSizer->Add(statusbarPanel, 1, wxGROW);
	contentPanel->SetSizerAndFit(contentSizer);
	statusbarPanel->SetSizerAndFit(statusBarSizer);
	mainPanel->SetSizerAndFit(mainSizer);

	contentPanel->SetBackgroundColour(COL_BG);
	statusbarPanel->SetBackgroundColour(COL_BG_STATUSBAR);
}

void cFrame::createTitleBar()
{
	wxStaticText* titleText = new wxStaticText(titleBarPanel, wxID_ANY, wxString("App prototype"));
	titleText->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleBarSizer->Add(titleText, 0, wxGROW | wxLEFT, 12);

	//todo: add buttons as images
}

void cFrame::createContent()
{
	// content is divided into 2 panels - chart and labels.

	auto contPanelChart = new wxPanel(contentPanel);
	auto leftSizer = new wxGridBagSizer();

	contPanelRight = new wxPanel(contentPanel);
	selectTab(TAB_ADDRLIST);
	
	
	contPanelChart->SetBackgroundColour(COL_BG);


	// initialize controls
	auto titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));
	auto subtitleFont = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	auto connectionTitle = new wxStaticText(contPanelChart, wxID_ANY, "Connection Stability");
	connectionTitle->SetForegroundColour(COL_TITLE);
	connectionTitle->SetFont(titleFont);
	
	chart->GetPlot()->SetBackground(new FillAreaDraw(COL_TITLE, COL_BG));
	chart->SetBackground(new FillAreaDraw(COL_BG_AUX, COL_BG));
	auto chartPanel = new wxChartPanel(contPanelChart, wxID_ANY, chart, wxDefaultPosition, wxSize(400, 240));
	chartPanel->SetAntialias(true);


	// Add controls to sizers
	leftSizer->Add(connectionTitle,
		wxGBPosition(0, 0),
		wxDefaultSpan,
		wxALL | wxALIGN_TOP,
		12);
	leftSizer->Add(chartPanel,
		wxGBPosition(1, 0),
		wxDefaultSpan,
		wxLeft | wxRight | wxGROW,
		12);

	contentSizer->Add(contPanelChart, 8, wxALL | wxGROW, 0);
	contentSizer->Add(contPanelRight, 5, wxALL | wxGROW, 0);

	contPanelChart->SetSizerAndFit(leftSizer);
}

void cFrame::createAddrlistPanel()
{
	auto sizer = contPanelRight->GetSizer();
	if (!sizer) sizer = new wxBoxSizer(wxVERTICAL);

	contPanelRight->SetBackgroundColour(COL_BG_AUX);

	auto titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	auto title = new wxStaticText(contPanelRight, wxID_ANY, "Address list");
	title->SetForegroundColour(COL_TITLE);
	title->SetFont(titleFont);

	sizer->Add(title,
		0,
		wxALL,
		12);

	contPanelRight->SetSizerAndFit(sizer);
}

void cFrame::createSettingsPanel()
{
	auto sizer = new wxBoxSizer(wxVERTICAL);

	contPanelRight->SetBackgroundColour(COL_BG_AUX);

	auto titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	auto title = new wxStaticText(contPanelRight, wxID_ANY, "Settings");
	title->SetForegroundColour(COL_TITLE);
	title->SetFont(titleFont);

	auto tempButton = new wxButton(contPanelRight, FRAME_TEMPBUTTON, "Push and shift");
	tempButton->Bind(wxEVT_BUTTON, &cFrame::OnTempButtonUp, this);

	auto tempTextinput = new wxTextCtrl(contPanelRight, wxID_ANY, "0");



	sizer->Add(title,		
		0, wxALL, 12);

	sizer->Add(tempButton,
		0, wxLEFT, 12);

	contPanelRight->SetSizerAndFit(sizer);
}

void cFrame::createStatusBar()
{
	auto buttonPanel = new wxPanel(statusbarPanel);
	auto hozrizontalSizer = new wxBoxSizer(wxHORIZONTAL);
	auto font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);

	addrlistHPanel = new cHighlightPanel(buttonPanel, COL_BG_STATUSBAR, COL_HL_STATUSBAR, COL_BG_AUX, FRAME_ADDRLIST);
	auto addrSizer = new wxBoxSizer(wxHORIZONTAL);
	auto addrImage = wxImage(wxString("res/Icon_addr_list.png"), wxBITMAP_TYPE_PNG);
	auto addrBmp = new wxStaticBitmap(addrlistHPanel, FRAME_ADDRLIST, wxBitmapBundle(addrImage));
	currentHPanel = addrlistHPanel;
	currentHPanel->Activate();

	auto addrText = new wxStaticText(addrlistHPanel, FRAME_ADDRLIST, "Address list");
	addrText->SetForegroundColour(COL_FG_STATUSBAR);
	addrText->SetFont(font);

	addrSizer->Add(addrBmp, 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	addrSizer->Add(addrText, 1, wxALIGN_CENTER | wxRIGHT, 10);
	addrlistHPanel->SetSizerAndFit(addrSizer);

	settingsHPanel = new cHighlightPanel(buttonPanel, 
		COL_BG_STATUSBAR, COL_HL_STATUSBAR, COL_BG_AUX, FRAME_SETTINGS);
	auto settingsSizer = new wxBoxSizer(wxHORIZONTAL);
	auto settingsImage = wxImage(wxString("res/Icon_settings.png"), wxBITMAP_TYPE_PNG);
	auto settingsBmp = new wxStaticBitmap(settingsHPanel, FRAME_SETTINGS, wxBitmapBundle(settingsImage));

	auto settingsText = new wxStaticText(settingsHPanel, FRAME_SETTINGS, "Settings");
	settingsText->SetForegroundColour(COL_FG_STATUSBAR);
	settingsText->SetFont(font);

	settingsSizer->Add(settingsBmp, 0, wxALIGN_CENTER | wxRIGHT | wxLEFT, 5);
	settingsSizer->Add(settingsText, 1, wxALIGN_CENTER | wxRIGHT, 10);
	settingsHPanel->SetSizerAndFit(settingsSizer);


	hozrizontalSizer->Add(addrlistHPanel, 0, wxEXPAND);
	hozrizontalSizer->Add(settingsHPanel, 0, wxEXPAND);
	buttonPanel->SetSizerAndFit(hozrizontalSizer);

	statusBarSizer->Add(buttonPanel, 1, wxALIGN_RIGHT);

	addrlistHPanel->BindRecursively(wxEVT_LEFT_UP, &cFrame::OnStatusBarHPanelUp, this);
	settingsHPanel->BindRecursively(wxEVT_LEFT_UP, &cFrame::OnStatusBarHPanelUp, this);
}

void cFrame::selectTab(Tab tab)
{
	//if (currentTab == tab)
	//	return;

	currentTab = tab;
	auto size = contPanelRight->GetSize();

	if (contPanelRight->GetSizer())
		contPanelRight->GetSizer()->Clear(true);

	switch (tab) {
	case TAB_ADDRLIST:
		createAddrlistPanel();
		break;
	case TAB_SETTINGS:
		createSettingsPanel();
		break;
	}

	contPanelRight->SetSize(size);
}

// Event handlers
void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->QueueEvent(event.Clone());
}

void cFrame::OnStatusBarHPanelUp(wxMouseEvent& event)
{
	if (currentHPanel->GetId() == event.GetId())
		return;

	currentHPanel->Deactivate();
	switch (event.GetId()) {

	case FRAME_ADDRLIST:
		currentHPanel = addrlistHPanel;
		currentHPanel->Activate();
		selectTab(TAB_ADDRLIST);
		break;

	case FRAME_SETTINGS:
		currentHPanel = settingsHPanel;
		currentHPanel->Activate();
		selectTab(TAB_SETTINGS);
		break;
	}
}

void cFrame::OnTempButtonUp(wxCommandEvent& event) 
{
	parent->QueueEvent(event.Clone());
}
