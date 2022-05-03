#include "cFrame.h"

wxBEGIN_EVENT_TABLE(cFrame, wxFrame)
	EVT_CLOSE(OnClosed)
	EVT_BUTTON(10000, OnButtonClick)
wxEND_EVENT_TABLE()

const int clientWidth = 700;
const int clientHeight = 400;
const wxSize clientSize = wxSize(clientWidth, clientHeight);

cFrame::cFrame(wxApp* parent) : wxFrame(nullptr, wxID_ANY, "App prototype")
{
	this->parent = parent;

	this->SetClientSize(clientSize);
	wxSize* windowSize = new wxSize(this->ClientToWindowSize(wxSize(clientWidth, clientHeight)));
	this->SetPosition(wxPoint(
		wxDisplay().GetClientArea().GetWidth() - windowSize->GetWidth() - 10,
		wxDisplay().GetClientArea().GetHeight() - windowSize->GetHeight() - 10));
	this->SetBackgroundColour(*wxWHITE);
	
	InitializeUI();
	this->SetClientSize(clientSize);
	this->SetMinClientSize(clientSize);
}

cFrame::~cFrame()
{

}

void cFrame::InitializeUI()
{
	frameSizer = new wxBoxSizer(wxHORIZONTAL);

	stabilityPanel = new wxPanel(this, wxID_ANY/*, wxPoint(0, 0), wxSize(clientWidth / 2, clientHeight)*/);
	stabilityPanel->SetBackgroundColour(*wxWHITE);
	scanPanel = new wxPanel(this, wxID_ANY/*, wxPoint(clientWidth / 2, 0), wxSize(clientWidth / 2, clientHeight)*/);
	scanPanel->SetBackgroundColour(*wxWHITE);

	stabilitySizer = new wxBoxSizer(wxVERTICAL);
	scanSizer = new wxBoxSizer(wxVERTICAL);


	wxFont* titleFont = new wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFontWeight(100));


	// Frame
	frameSizer->Add(stabilityPanel, 1, wxEXPAND);
	frameSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL),
		0,
		wxALL | wxEXPAND,
		5);
	frameSizer->Add(scanPanel, 1, wxEXPAND);
	this->SetSizerAndFit(frameSizer);


	// Panel 1 - Stability
	titleText1 = new wxStaticText(stabilityPanel, wxID_ANY, wxString("Connection Stability"));
	titleText1->SetFont(*titleFont);


	stabilitySizer->Add(titleText1, 0, wxLEFT | wxALIGN_LEFT, 40);
	stabilitySizer->Add(
		new wxStaticLine(stabilityPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL),
		0,
		wxGROW | wxALL,
		10);

	//wxChartsCategoricalData::ptr catData(new wxChartsCategoricalData());
	//wxLineChartCtrl* chart = new wxLineChartCtrl(stabilityPanel, wxID_ANY);



	stabilityPanel->SetSizerAndFit(stabilitySizer);



	// Panel 2 - Scanner
	wxStaticText* titleText2 = new wxStaticText(scanPanel, wxID_ANY, wxString("IP Scanner"));
	titleText2->SetFont(*titleFont);
	scanSizer->Add(titleText2, 0, wxLEFT | wxALIGN_LEFT, 40);
	scanSizer->Add(
		new wxStaticLine(scanPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL),
		0,
		wxGROW | wxALL,
		10);
	btn = new wxButton(scanPanel, 10000, "Test");
	scanSizer->Add(
		btn,
		0,
		wxSTRETCH_NOT | wxALL,
		10);
	sampleText1 = new wxStaticText(scanPanel, wxID_ANY, wxString("1 sample\n2 sample\n3 sample"));
	scanSizer->Add(
		sampleText1,
		1, 
		wxGROW | wxALL,
		10);


	scanPanel->SetSizerAndFit(scanSizer);




}

void cFrame::OnButtonClick(wxCommandEvent& event)
{
	parent->ProcessEvent(event);
}

void cFrame::OnMouseMove(wxMouseEvent& event)
{
	titleText1->SetLabel(wxString(std::to_string(event.GetX()) + "; " + std::to_string(event.GetY())));
}

void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
}

void cFrame::LoadIcon(wxIcon* icon)
{
	SetIcon(*icon);
}
