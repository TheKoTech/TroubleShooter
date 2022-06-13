#include "cFrame.h"
#include "ThemeDefault.h"
#include "cHighlightPanel.h"


wxBEGIN_EVENT_TABLE(cFrame, wxFrame)
EVT_CLOSE(OnClosed)
EVT_BUTTON(10000, OnButtonClick)
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

	initializeUI();
	applyFrameSettings();
}

cFrame::~cFrame()
{

}

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

	wxPanel* contPanelLeft = new wxPanel(contentPanel);
	wxPanel* contPanelRight = new wxPanel(contentPanel);
	wxGridBagSizer* leftSizer = new wxGridBagSizer();
	wxGridBagSizer* rightSizer = new wxGridBagSizer();

	contPanelLeft->SetBackgroundColour(COL_BG);
	contPanelRight->SetBackgroundColour(COL_BG_AUX);

	contentSizer->Add(contPanelLeft, 1, wxALL | wxGROW, 0);
	contentSizer->Add(contPanelRight, 1, wxALL | wxGROW, 0);



	// initialize controls

	wxFont titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));
	wxFont subtitleFont = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	wxStaticText* connectionTitle = new wxStaticText(contPanelLeft, wxID_ANY, "Connection Stability");
	connectionTitle->SetForegroundColour(COL_TITLE);
	connectionTitle->SetFont(titleFont);
	
	chart->GetPlot()->SetBackground(new FillAreaDraw(COL_TITLE, COL_BG));
	chart->SetBackground(new FillAreaDraw(COL_BG_AUX, COL_BG));
	wxChartPanel* chartPanel = new wxChartPanel(contPanelLeft, wxID_ANY, chart, wxDefaultPosition, wxSize(400, 240));
	chartPanel->SetAntialias(true);



	wxStaticText* statusTitle = new wxStaticText(contPanelRight, wxID_ANY, "Status");
	statusTitle->SetForegroundColour(COL_TITLE);
	statusTitle->SetFont(titleFont);
	
	wxStaticText* warningTitle = new wxStaticText(contPanelRight, wxID_ANY, "Warnings");
	warningTitle->SetForegroundColour(COL_FG);
	warningTitle->SetFont(subtitleFont);

	//wxPanel* lanPanel = createStatusPanelElement();



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

	rightSizer->Add(statusTitle,
		wxGBPosition(0, 0),
		wxDefaultSpan,
		wxALL | wxALIGN_TOP,
		12);
	rightSizer->Add(warningTitle,
		wxGBPosition(0, 1),
		wxDefaultSpan,
		wxLEFT | wxTOP | wxALIGN_TOP,
		15);

	contPanelLeft->SetSizerAndFit(leftSizer);
	contPanelRight->SetSizerAndFit(rightSizer);

	// todo: Link events

}
void cFrame::createStatusBar()
{	
	auto font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);


	auto settingsPanel = new cHighlightPanel(statusbarPanel, COL_BG_STATUSBAR, COL_BLUE);
	auto settingsSizer = new wxBoxSizer(wxHORIZONTAL);
	auto settingsImage = wxImage(wxString("res/Icon_settings.png"), wxBITMAP_TYPE_PNG);
	settingsImage.Rescale(20, 20, wxIMAGE_QUALITY_HIGH);
	auto settingsBmp = new wxStaticBitmap(settingsPanel, wxID_ANY, wxBitmapBundle(settingsImage));

	auto settingsText = new wxStaticText(settingsPanel, wxID_ANY, "Settings");
	settingsText->SetForegroundColour(COL_FG_STATUSBAR);
	settingsText->SetFont(font);

	settingsSizer->Add(settingsBmp, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT | wxLEFT, 7);
	settingsSizer->Add(settingsText, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	settingsPanel->SetSizerAndFit(settingsSizer);

	statusBarSizer->Add(settingsPanel, 1, wxALIGN_RIGHT);
}

wxPanel* cFrame::createStatusPanelElement()
{
	
	return nullptr;
}

void cFrame::applyFrameSettings()
{
	this->SetBackgroundColour(*wxWHITE);
	this->SetClientSize(clientSize);
	this->SetMinClientSize(clientSize);
	wxSize windowSize = this->ClientToWindowSize(clientSize);
	this->SetPosition(wxPoint(
		wxDisplay().GetClientArea().GetWidth() - windowSize.GetWidth() - 10,
		wxDisplay().GetClientArea().GetHeight() - windowSize.GetHeight() - 10));
}

// Event handlers
void cFrame::OnButtonClick(wxCommandEvent& event)
{
	parent->ProcessEvent(event);
}
void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
	this->Destroy();
}