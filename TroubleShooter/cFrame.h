#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/taskbar.h>
#include <wx/statline.h>
#include <wx/vector.h>
#include <wx/gbsizer.h>
#include "wx/chart.h"
#include <wx/xy/xyplot.h>
#include <wx/chartpanel.h>
#include <wx/xy/xysimpledataset.h>
#include <wx/xy/xylinerenderer.h>
#include <wx/axisplot.h>

//todo: optimize imports


/*
* The main window of the app or the TopLevelWindow
*/
class cFrame : public wxFrame
{
public:
	cFrame(wxApp* parent, Chart* chart); 
	~cFrame();
	
private:
	wxApp* parent;

	// The window is divided into 3 main panels:
	wxPanel* mainPanel;
	wxPanel* titleBarPanel;
	wxPanel* contentPanel;
	wxPanel* statusbarPanel;

	// Each requires a separate Sizer to put items in order. Additionally, all panels are put into a sizer.
	wxBoxSizer* mainSizer;
	wxBoxSizer* titleBarSizer;
	wxBoxSizer* statusBarSizer;
	wxBoxSizer* contentSizer;

	Chart* chart;

	// UI methods
	void initializeUI();
	void createTitleBar();
	void createContent();
	void createStatusBar();
	wxPanel* createStatusPanelElement();
	void applyFrameSettings();

	// Event handlers
	
	// Not used
	void OnButtonClick(wxCommandEvent& event);
	void OnClosed(wxCloseEvent& event);

	wxDECLARE_EVENT_TABLE();
};

