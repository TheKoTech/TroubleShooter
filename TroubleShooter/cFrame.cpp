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

	// Frame Options
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


	// Panel 1 - Network Stability
	titleStability = new wxStaticText(stabilityPanel, wxID_ANY, wxString("Connection Stability"));
	titleStability->SetFont(*titleFont);
	stabilitySizer->Add(titleStability, 0, wxLEFT | wxALIGN_LEFT, 40);
	stabilitySizer->Add(
		new wxStaticLine(stabilityPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL),
		0, 
		wxGROW | wxALL, 
		10);

	wxVector<wxRealPoint> data;
	data.push_back(wxRealPoint(10, 20));
	data.push_back(wxRealPoint(13, 16));
	data.push_back(wxRealPoint(7, 30));
	data.push_back(wxRealPoint(15, 34));
	data.push_back(wxRealPoint(25, 4));

	// Create the dataset with an initial single series.
	XYSimpleDataset* dataset = new XYSimpleDataset();

	// Further series can be added here, for example ...
	XYSerie* serie = new XYSerie(data);
	dataset->AddSerie(serie);

	// Create a renderer
	XYLineRenderer* renderer = new XYLineRenderer();
	renderer->SetSeriePen(size_t(2), new wxPen(*wxBLUE, wxPENSTYLE_SOLID));
	dataset->SetRenderer(renderer);

	// Create plot
	XYPlot* plot = new XYPlot();
	plot->AddDataset(dataset);

	// Define Axes and add to the plot
	NumberAxis* leftAxis = new NumberAxis(AXIS_LEFT);
	leftAxis->SetFixedBounds(0, 40);
	NumberAxis* bottomAxis = new NumberAxis(AXIS_BOTTOM);
	bottomAxis->SetFixedBounds(0, 200);

	
	plot->AddAxis(leftAxis);
	plot->AddAxis(bottomAxis);
	
	// Create chart
	Chart* chart = new Chart(plot, "This is a chart");

	// A separate panel must be created for the plot
	wxChartPanel* chartPanel = new wxChartPanel(stabilityPanel, wxID_ANY, chart);

	stabilitySizer->Add(chartPanel,
		1,
		wxALL | wxEXPAND,
		10);
	


	stabilityPanel->SetSizerAndFit(stabilitySizer);


	// Panel 2 - IP Scanner
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

void cFrame::OnClosed(wxCloseEvent& event)
{
	parent->ProcessEvent(event);
}

void cFrame::LoadIcon(wxIcon* icon)
{
	SetIcon(*icon);
}
