#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/taskbar.h>
#include <wx/statline.h>
#include <wx/vector.h>
#include "wx/chart.h"
#include <wx/xy/xysimpledataset.h>
#include <wx/xy/xyplot.h>
#include <wx/xy/xylinerenderer.h>


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
	wxStaticText* titleStability = nullptr;
	
	wxButton* btn = nullptr;

	
	

	wxPanel* scanPanel = nullptr;

	void InitializeUI();
	void OnButtonClick(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

