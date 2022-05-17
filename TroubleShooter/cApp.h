#pragma once
#include "wx/wx.h"
#include "cTaskBarIcon.h"
#include "cFrame.h"
#include "ChartController.h"
#include "MultiPing.h"
#include "Logger.h"
#include "cSettingsFrame.h"

enum AppStatus { green, yellow, red, black };

// cApp ��� ���������� ����������.
class cApp : public wxApp
{
public:
	cApp();
	~cApp();

	// ������ ���� �������� ����������� � ���������������� "TopLevelWindow" (taskBarIcon � ����� ������). �� ����, ���������� ������ ������������. 
	virtual bool OnInit();

private:
	// AppStatus �������� ������ ����������
	// The AppStatus represents the current connection status: OK (green), Unstable (yellow), Outage (Red) or that the App is disabled (black)
	AppStatus appStatus;
	cFrame* mainFrame = nullptr;
	cSettingsFrame* settingsFrame = nullptr;
	cTaskBarIcon* taskBarIcon = nullptr;
	ChartController chartController;

	// Todo: ������ ������

	// UI ������

	//����� �������� ��� ��������� AppStatus. ������ ������ ���������� �� ��������������� AppStatus.
	//Should be called on AppStatus change. Changes the app icon to match AppStatus.
	bool UpdateIcon();

	// ���� ����� ���������� ��� �������� cFrame. �� ��������� ������ �� ����� � ������, ��������� �� � wxVector � ����� ���� ����������� ��� �����������.
	// This method is called on cFrame creation. It receives log data from a file, forms it into a wxVector and passes it to chartController for display.
	void initializeChartSeries();
	void createFrame();
	void closeFrame();
	void createSettingsFrame();
	void closeSettingsFrame();

	// ������
	void OnTaskBarIconLeftUp(wxTaskBarIconEvent& event);
	void OnClosed(wxCloseEvent& event);
	void OnTaskBarIconMenuShow(wxCommandEvent& event);
	void OnTaskBarIconMenuSettings(wxCommandEvent& event);
	void OnTaskBarIconMenuClose(wxCommandEvent& event);
	void OnPanelLeftUp(wxMouseEvent& event);
	void OnApplySettingsButtonClick(wxCommandEvent& event);
	void OnDiscardSettingsButtonClick(wxCommandEvent& event);

	void OnTimer(wxTimerEvent& event);

	wxDECLARE_EVENT_TABLE();
};

