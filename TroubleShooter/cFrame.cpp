#include "cFrame.h"
#include "Colors.cpp"

wxBEGIN_EVENT_TABLE(cFrame, wxFrame)
	EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

const int clientWidth = 700;
const int clientHeight = 320;
const wxSize clientSize = wxSize(clientWidth, clientHeight);

cFrame::cFrame(wxApp* parent, Chart* chart) : wxFrame(nullptr, wxID_ANY, "Pinger",
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
= default;

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
	auto const titleText = new wxStaticText(titleBarPanel, wxID_ANY, wxString("App prototype"));
	titleText->SetFont(wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	titleBarSizer->Add(titleText, 0, wxGROW | wxLEFT, 12);

	//todo: add buttons as images
}
void cFrame::createContent()
{
	// content is divided into 2 panels - chart and labels.

	auto const contPanelLeft = new wxPanel(contentPanel);
	auto const contPanelRight = new wxPanel(contentPanel);
	auto const leftSizer = new wxGridBagSizer();
	auto const rightSizer = new wxGridBagSizer();

	contPanelLeft->SetBackgroundColour(COL_BG);
	contPanelRight->SetBackgroundColour(COL_BG_AUX);

	contentSizer->Add(contPanelLeft, 1, wxALL | wxGROW, 0);
	contentSizer->Add(contPanelRight, 1, wxALL | wxGROW, 0);



	// initialize controls

	auto const titleFont = wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));
	auto const subtitleFont = wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));

	auto const connectionTitle = new wxStaticText(contPanelLeft, wxID_ANY, "Connection Stability");
	connectionTitle->SetForegroundColour(COL_TITLE);
	connectionTitle->SetFont(titleFont);
	
	chart->GetPlot()->SetBackground(new FillAreaDraw(COL_TITLE, COL_BG));
	chart->SetBackground(new FillAreaDraw(COL_BG_AUX, COL_BG));
	auto const chartPanel = new wxChartPanel(contPanelLeft, wxID_ANY, chart, wxDefaultPosition, wxSize(400, 240));
	chartPanel->SetAntialias(true);


	auto const statusTitle = new wxStaticText(contPanelRight, wxID_ANY, "Status");
	statusTitle->SetForegroundColour(COL_TITLE);
	statusTitle->SetFont(titleFont);

	auto const warningTitle = new wxStaticText(contPanelRight, wxID_ANY, "Warnings");
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
	auto const font = wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD);
	settingsPanel = new wxPanel(statusbarPanel, FRAME_SETTINGS);

	auto const settingsText = new wxStaticText(settingsPanel, FRAME_SETTINGS, "Settings");
	settingsText->SetForegroundColour(COL_FG_STATUSBAR);
	settingsText->SetFont(font);

	auto const settingsSizer = new wxBoxSizer(wxHORIZONTAL);
	auto const settingsImage = wxImage("res/Icon_settings.png", wxBITMAP_TYPE_PNG);
	auto const settingsBmp = new wxStaticBitmap(settingsPanel, FRAME_SETTINGS, wxBitmapBundle(settingsImage));

	settingsSizer->Add(settingsBmp, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 5);
	settingsSizer->Add(settingsText, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
	settingsPanel->SetSizerAndFit(settingsSizer);

	settingsPanel->Bind(wxEVT_ENTER_WINDOW, &cFrame::OnEnterSettingsPanel, this);
	settingsPanel->Bind(wxEVT_LEAVE_WINDOW, &cFrame::OnLeaveSettingsPanel, this);
	bindMouseEventRecursive(settingsPanel, &cFrame::OnSettingsLeftUp);

	statusBarSizer->Add(settingsPanel, 1, wxALIGN_RIGHT);
}

wxPanel* cFrame::createStatusPanelElement()
{
	return nullptr;
}

void cFrame::applyFrameSettings()
{
	this->SetClientSize(clientSize);
	this->SetMinClientSize(clientSize);
	wxSize windowSize = this->ClientToWindowSize(clientSize);
	this->SetPosition(wxPoint(
		wxDisplay().GetClientArea().GetWidth() - windowSize.GetWidth() - 10,
		wxDisplay().GetClientArea().GetHeight() - windowSize.GetHeight() - 10));
}

// Event handlers

void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
	this->Destroy();
}

void cFrame::bindMouseEventRecursive(wxWindow* window, void (cFrame::*method)(wxMouseEvent&))
{
	if (window)
	{
		window->Bind(wxEVT_LEFT_UP, method, this);

		auto node = window->GetChildren().GetFirst();
		while (node)
		{
			auto child = node->GetData();
			this->bindMouseEventRecursive(child, method);
			node = node->GetNext();
		}
	}
}

void cFrame::OnEnterSettingsPanel(wxMouseEvent& event)
{
	bool mouseInside = settingsPanel->GetClientRect().Contains(event.GetPosition());

	if (mouseInside) {
		settingsPanel->SetBackgroundColour(COL_BG);
		settingsPanel->Refresh();
	}
	else
	{
		event.Skip();
	}
}

void cFrame::OnLeaveSettingsPanel(wxMouseEvent& event)
{
	bool mouseInside = settingsPanel->GetClientRect().Contains(event.GetPosition());

	if (!mouseInside) {
		settingsPanel->SetBackgroundColour(COL_BG_STATUSBAR);
		settingsPanel->Refresh();
	}
	else
	{
		event.Skip();
	}
}

void cFrame::OnSettingsLeftUp(wxMouseEvent& event)
{
	// if (event.GetId() == FRAME_SETTINGS) {
	// 	handler->QueueEvent(event.Clone());
	// }
	event.Skip();

}
