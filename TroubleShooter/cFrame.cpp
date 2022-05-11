#include "cFrame.h"
#include "Colors.cpp"

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
	// for icons
	wxPNGHandler* handler = new wxPNGHandler;
	wxImage::AddHandler(handler);

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

	contentPanel->SetBackgroundColour(BG_COLOUR);
	statusbarPanel->SetBackgroundColour(BG_COLOUR_STATUSBAR);
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

	contPanelLeft->SetBackgroundColour(BG_COLOUR);
	contPanelRight->SetBackgroundColour(BG_COLOUR_AUX);

	contentSizer->Add(contPanelLeft, 1, wxALL | wxGROW, 0);
	contentSizer->Add(contPanelRight, 1, wxALL | wxGROW, 0);



	// initialize controls

	wxFont titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));
	wxFont subtitleFont = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	wxStaticText* connectionTitle = new wxStaticText(contPanelLeft, wxID_ANY, "Connection Stability");
	connectionTitle->SetForegroundColour(TITLE_COLOUR);
	connectionTitle->SetFont(titleFont);
	
	chart->GetPlot()->SetBackground(new FillAreaDraw(TITLE_COLOUR, BG_COLOUR));
	chart->SetBackground(new FillAreaDraw(BG_COLOUR_AUX, BG_COLOUR));
	wxChartPanel* chartPanel = new wxChartPanel(contPanelLeft, wxID_ANY, chart, wxDefaultPosition, wxSize(400, 240));
	chartPanel->SetAntialias(true);



	wxStaticText* statusTitle = new wxStaticText(contPanelRight, wxID_ANY, "Status");
	statusTitle->SetForegroundColour(TITLE_COLOUR);
	statusTitle->SetFont(titleFont);
	
	wxStaticText* warningTitle = new wxStaticText(contPanelRight, wxID_ANY, "Warnings");
	warningTitle->SetForegroundColour(FG_COLOUR);
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
	wxFont font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);


	wxPanel* settingsPanel = new wxPanel(statusbarPanel);
	wxBoxSizer* settingsSizer = new wxBoxSizer(wxHORIZONTAL);
	wxImage settingsImage = wxImage("res/Icon_settings.png", wxBITMAP_TYPE_PNG);
	wxStaticBitmap * settingsBmp = new wxStaticBitmap(settingsPanel, wxID_ANY, wxBitmapBundle(settingsImage));

	wxStaticText* settingsText = new wxStaticText(settingsPanel, wxID_ANY, "Settings");
	settingsText->SetForegroundColour(FG_STATUSBAR_COLOUR);
	settingsText->SetFont(font);

	settingsSizer->Add(settingsBmp, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);
	settingsSizer->Add(settingsText, 1, wxALIGN_CENTER_VERTICAL);
	settingsPanel->SetSizerAndFit(settingsSizer);

	statusBarSizer->Add(settingsPanel, 1, wxALIGN_RIGHT | wxRIGHT, 10);
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
void cFrame::LoadIcon(wxIcon* icon)
{
	SetIcon(*icon);
}
