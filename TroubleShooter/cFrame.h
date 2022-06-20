#pragma once
#include "wx/wx.h"
#include <wx/display.h>
#include <wx/taskbar.h>
#include <wx/vector.h>
#include <wx/gbsizer.h>
#include "wx/chart.h"
#include <wx/axisplot.h>
#include "app_ids.h"
#include "cHighlightPanel.h"


enum FrameIds {
	FRAME_ADDRLIST = MAIN_FRAME + 1,
	FRAME_SETTINGS = MAIN_FRAME + 2,
	FRAME_TEMPBUTTON = MAIN_FRAME + 10,
};

enum Tab {
	TAB_NONE = -1,
	TAB_ADDRLIST = 0,
	TAB_SETTINGS = 1,
};


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

	wxPanel* mainPanel = nullptr;
	wxPanel* titleBarPanel = nullptr;
	wxPanel* contentPanel = nullptr;
	wxPanel* contPanelRight = nullptr;
	wxPanel* statusbarPanel = nullptr;

	wxBoxSizer* mainSizer = nullptr;
	wxBoxSizer* titleBarSizer = nullptr;
	wxBoxSizer* statusBarSizer = nullptr;
	wxBoxSizer* contentSizer = nullptr;

	Chart* chart = nullptr;

	cHighlightPanel* currentHPanel = nullptr;
	cHighlightPanel* addrlistHPanel = nullptr;
	cHighlightPanel* settingsHPanel = nullptr;

	Tab currentTab;

	// UI methods
	void initializeUI();
	void createTitleBar();
	void createContent();
	void createAddrlistPanel();
	void createSettingsPanel();
	void createStatusBar();

	void selectTab(Tab tab);

	// Event handlers
	void OnClosed(wxCloseEvent& event);
	void OnStatusBarHPanelUp(wxMouseEvent& event);

	void OnTempButtonUp(wxCommandEvent& event);

	wxDECLARE_EVENT_TABLE();
};

