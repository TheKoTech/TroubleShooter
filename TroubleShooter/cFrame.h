#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/taskbar.h>
#include <wx/statline.h>
#include <wx/vector.h>
#include "wx/charts/wxchart.h"
#include "wx/charts/wxlinechartctrl.h"


enum IconId;

class cFrame : public wxFrame
{
public:
	cFrame(wxApp* parent); 
	~cFrame();

	void LoadIcon(wxIcon* icon);
	void OnClosed(wxCloseEvent& event);

	wxStaticText* sampleText1 = nullptr;

private:
	wxApp* parent = nullptr;
	wxFrame* frame = nullptr;
	wxBoxSizer* frameSizer = nullptr;
	wxBoxSizer* stabilitySizer = nullptr;
	wxBoxSizer* scanSizer = nullptr;

	wxPanel* stabilityPanel = nullptr;
	wxStaticText* titleText1 = nullptr;
	
	wxButton* btn = nullptr;
	

	wxPanel* scanPanel = nullptr;

	void InitializeUI();
	void OnButtonClick(wxCommandEvent& event);

	void OnMouseMove(wxMouseEvent& event);
	wxDECLARE_EVENT_TABLE();
};

