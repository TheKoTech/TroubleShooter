#include "cSettingsFrame.h"
#include "Colors.cpp"


wxBEGIN_EVENT_TABLE(cSettingsFrame, wxFrame)
	EVT_CLOSE(OnClosed)
	EVT_BUTTON(SETTINGS_APPLY, OnApplyButtonClick)
	EVT_BUTTON(SETTINGS_DISCARD, OnDiscardButtonClick)
wxEND_EVENT_TABLE()

const wxSize minClientSize = wxSize(215, 290);

cSettingsFrame::cSettingsFrame(wxApp* parent) : wxFrame(nullptr, wxID_ANY, "Settings")
{
	this->parent = parent;
	settingsChanged = false; // if text is changed, deny to close the window

	initializeUI();
	applyFrameSettings();
}

cSettingsFrame::~cSettingsFrame() = default;

void cSettingsFrame::initializeUI()
{
	auto const windowSizer = new wxBoxSizer(wxVERTICAL);
	auto const mainPanel = new wxPanel(this);
	auto const buttonPanel = new wxPanel(this);
	auto const mSizer = new wxGridBagSizer();
	auto const buttonSizer = new wxBoxSizer(wxHORIZONTAL);


	auto const titleFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	auto const normalFont = wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	auto const titleText = new wxStaticText(mainPanel, wxID_ANY, "Settings");
	titleText->SetForegroundColour(COL_TITLE);
	titleText->SetFont(titleFont);

	auto const lanText = new wxStaticText(mainPanel, wxID_ANY, "LAN");
	auto const ispText = new wxStaticText(mainPanel, wxID_ANY, "Internet Provider");
	auto const dnsText = new wxStaticText(mainPanel, wxID_ANY, "DNS");
	auto const hostText = new wxStaticText(mainPanel, wxID_ANY, "Host");
	lanText->SetFont(normalFont);
	ispText->SetFont(normalFont);
	dnsText->SetFont(normalFont);
	hostText->SetFont(normalFont);
	lanText->SetForegroundColour(COL_TITLE);
	ispText->SetForegroundColour(COL_TITLE);
	dnsText->SetForegroundColour(COL_TITLE);
	hostText->SetForegroundColour(COL_TITLE);

	lanInput = new wxTextCtrl(mainPanel, SETTINGS_LAN, wxString("192.168.1.1"));
	ispInput = new wxTextCtrl(mainPanel, SETTINGS_ISP, wxString(""));
	dnsInput = new wxTextCtrl(mainPanel, SETTINGS_DNS, wxString("8.8.8.8"));
	hostInput = new wxTextCtrl(mainPanel, SETTINGS_HOST, wxString("yandex.ru"));

	lanInput->SetBackgroundColour(COL_BG_AUX);
	ispInput->SetBackgroundColour(COL_BG_AUX);
	dnsInput->SetBackgroundColour(COL_BG_AUX);
	hostInput->SetBackgroundColour(COL_BG_AUX);

	lanInput->SetForegroundColour(COL_FG);
	ispInput->SetForegroundColour(COL_FG);
	dnsInput->SetForegroundColour(COL_FG);
	hostInput->SetForegroundColour(COL_FG);

	const int buttonBorder = 10;
	const int textBorder = 2;

	mSizer->Add(titleText, wxGBPosition(0, 0), wxDefaultSpan, wxLEFT | wxBOTTOM, buttonBorder);
	mSizer->Add(lanText, wxGBPosition(1, 0), wxDefaultSpan, wxBOTTOM, textBorder);
	mSizer->Add(lanInput, wxGBPosition(2, 0), wxDefaultSpan, wxBOTTOM, buttonBorder);
	mSizer->Add(ispText, wxGBPosition(3, 0), wxDefaultSpan, wxBOTTOM, textBorder);
	mSizer->Add(ispInput, wxGBPosition(4, 0), wxDefaultSpan, wxBOTTOM, buttonBorder);
	mSizer->Add(dnsText, wxGBPosition(5, 0), wxDefaultSpan, wxBOTTOM, textBorder);
	mSizer->Add(dnsInput, wxGBPosition(6, 0), wxDefaultSpan, wxBOTTOM, buttonBorder);
	mSizer->Add(hostText, wxGBPosition(7, 0), wxDefaultSpan, wxBOTTOM, textBorder);
	mSizer->Add(hostInput, wxGBPosition(8, 0), wxDefaultSpan, wxBOTTOM, buttonBorder);
	mainPanel->SetSizerAndFit(mSizer);



	auto const applyButton = new wxButton(buttonPanel, SETTINGS_APPLY, wxString("Apply"));
	auto const discardButton = new wxButton(buttonPanel, SETTINGS_DISCARD, wxString("Discard"));
	applyButton->SetForegroundColour(COL_GREEN);
	discardButton->SetForegroundColour(COL_RED);

	buttonSizer->Add(applyButton, 0, wxRIGHT | wxBOTTOM | wxALIGN_CENTER, 5);
	buttonSizer->Add(discardButton, 0, wxRIGHT | wxBOTTOM | wxALIGN_CENTER, 5);
	buttonPanel->SetSizerAndFit(buttonSizer);

	windowSizer->Add(mainPanel, 1, wxGROW | wxALL, 5);
	windowSizer->Add(buttonPanel, 0, wxALL | wxALIGN_RIGHT, 5);
	this->SetSizerAndFit(windowSizer);
	this->SetFocus();
}

void cSettingsFrame::applyFrameSettings()
{
	this->SetBackgroundColour(COL_BG);
	this->SetClientSize(minClientSize);
	this->SetMinClientSize(minClientSize);
	wxSize windowSize = this->ClientToWindowSize(minClientSize);
	this->SetPosition(wxPoint(
		wxDisplay().GetClientArea().GetWidth() - windowSize.GetWidth() - 50,
		wxDisplay().GetClientArea().GetHeight() - windowSize.GetHeight() - 25));
}

void cSettingsFrame::OnApplyButtonClick(wxCommandEvent& event)
{

}


void cSettingsFrame::OnDiscardButtonClick(wxCommandEvent& event)
{

}

void cSettingsFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
}