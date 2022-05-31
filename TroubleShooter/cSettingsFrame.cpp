#include "cSettingsFrame.h"
#include "Colors.cpp"


wxBEGIN_EVENT_TABLE(cSettingsFrame, wxFrame)
EVT_CLOSE(OnClosed)
EVT_BUTTON(SETTINGS_APPLY, OnApplyButtonClick)
EVT_BUTTON(SETTINGS_DISCARD, OnDiscardButtonClick)
wxEND_EVENT_TABLE()

const wxSize minClientSize = wxSize(215, 290);

cSettingsFrame::cSettingsFrame(wxApp* parent, wxVector<wxString>* addresses) : wxFrame(nullptr, wxID_ANY, "Settings")
{
	this->parent = parent;
	settingsChanged = false; // if text is changed, deny to close the window



	initializeUI(addresses);
	applyFrameSettings();
}

cSettingsFrame::~cSettingsFrame() = default;

wxVector<wxString>* cSettingsFrame::GetNewAdresses()
{
	auto adresses = new wxVector<wxString>();
	adresses->push_back(lanInput->GetLineText(0));
	adresses->push_back(ispInput->GetLineText(0));
	adresses->push_back(dnsInput->GetLineText(0));
	adresses->push_back(hostInput->GetLineText(0));
	return adresses;
}

void cSettingsFrame::initializeUI(wxVector<wxString>*addresses)
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

	lanInput = new wxTextCtrl(mainPanel, SETTINGS_LAN, addresses->at(0));
	ispInput = new wxTextCtrl(mainPanel, SETTINGS_ISP, addresses->at(1));
	dnsInput = new wxTextCtrl(mainPanel, SETTINGS_DNS, addresses->at(2));
	hostInput = new wxTextCtrl(mainPanel, SETTINGS_HOST, addresses->at(3));

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

void cSettingsFrame::OnApplyButtonClick(wxCommandEvent & event)
{
	event.Skip();
}


void cSettingsFrame::OnDiscardButtonClick(wxCommandEvent & event)
{
	this->Close();
}

void cSettingsFrame::OnClosed(wxCloseEvent & event)
{
	parent->ProcessEvent(event);
}