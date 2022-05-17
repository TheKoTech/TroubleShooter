#include "cSettingsFrame.h"
#include "Colors.cpp"


wxBEGIN_EVENT_TABLE(cSettingsFrame, wxFrame)
	EVT_CLOSE(OnClosed)
wxEND_EVENT_TABLE()

const wxSize minClientSize = wxSize(300, 380);

cSettingsFrame::cSettingsFrame(wxApp* parent) : wxFrame(nullptr, wxID_ANY, "Settings")
{
	this->parent = parent;

	initializeUI();
	applyFrameSettings();
}

void cSettingsFrame::initializeUI()
{
	mPanel = new wxPanel(this);
	auto const buttonPanel = new wxPanel(this);
	auto const mSizer = new wxGridBagSizer();
	auto const buttonSizer = new wxBoxSizer(wxVERTICAL);
	mPanel->SetBackgroundColour(COL_BG_AUX);


	auto const titleFont = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
	auto const normalFont = wxFont(9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

	auto const titleText = new wxStaticText(mPanel, wxID_ANY, "Settings");
	titleText->SetForegroundColour(COL_TITLE);
	titleText->SetFont(titleFont);

	lanInput = new wxTextCtrl(mPanel, 11000, wxString("192.168.1.1"));
	ispInput = new wxTextCtrl(mPanel, 11001, wxString("10.0.0.1"));
	dnsInput = new wxTextCtrl(mPanel, 11002, wxString("8.8.8.8"));
	hostInput = new wxTextCtrl(mPanel, 11003, wxString("yandex.ru"));

	lanInput->SetBackgroundColour(COL_BG_AUX);
	ispInput->SetBackgroundColour(COL_BG_AUX);
	dnsInput->SetBackgroundColour(COL_BG_AUX);
	hostInput->SetBackgroundColour(COL_BG_AUX);

	lanInput->SetForegroundColour(COL_FG);
	ispInput->SetForegroundColour(COL_FG);
	dnsInput->SetForegroundColour(COL_FG);
	hostInput->SetForegroundColour(COL_FG);

	auto const applyButton = new wxButton(buttonPanel, SETTINGS_APPLY, wxString("Apply"));
	auto const discardButton = new wxButton(buttonPanel, SETTINGS_DISCARD, wxString("Discard"));

	//auto vButtonSizer = new wxBoxSizer(wxVERTICAL);
	auto hButtonSizer = new wxBoxSizer(wxHORIZONTAL);

	hButtonSizer->AddStretchSpacer(5);
	hButtonSizer->Add(applyButton, 1, wxALL | wxALIGN_CENTER, 5);
	hButtonSizer->Add(discardButton, 1, wxALL | wxALIGN_CENTER, 5);


	mSizer->Add(titleText, wxGBPosition(0, 0), wxDefaultSpan, wxALL, 5);
	mSizer->Add(lanInput, wxGBPosition(1, 0), wxDefaultSpan, wxALL, 5);
	mSizer->Add(ispInput, wxGBPosition(2, 0), wxDefaultSpan, wxALL, 5);
	mSizer->Add(dnsInput, wxGBPosition(3, 0), wxDefaultSpan, wxALL, 5);
	mSizer->Add(hostInput, wxGBPosition(4, 0), wxDefaultSpan, wxALL, 5);
	//mSizer->AddStretchSpacer(1);
	mSizer->Add(hButtonSizer, wxGBPosition(5,0), wxGBSpan(1, 2), wxALL | wxALIGN_RIGHT, 5);

	

	mPanel->SetSizerAndFit(mSizer);
	auto windowSizer = new wxBoxSizer(wxVERTICAL);
	windowSizer->Add(mPanel, 0, wxGROW | wxALL, 10);
	this->SetSizerAndFit(windowSizer);
}

cSettingsFrame::~cSettingsFrame() = default;

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

void cSettingsFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
}
