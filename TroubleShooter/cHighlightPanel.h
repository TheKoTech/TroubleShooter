#pragma once
#include <wx/panel.h>

enum PanelState {
	HPANEL_DEFAULT = 0,
	HPANEL_HIGHLIGHTED = 1,
	HPANEL_ACTIVATED = 2
};

class cHighlightPanel : public wxPanel
{
private:
	PanelState panel_state;

	wxColour col_default;
	wxColour col_highlight;
	wxColour col_activated;

	bool activated;

public:
	cHighlightPanel(
		wxWindow* parent,
		wxColour defaultColour,
		wxColour highlightColour,
		wxColour activatedColour,
		wxWindowID winid = wxID_ANY,
		wxPoint pos = wxDefaultPosition,
		const wxSize size = wxDefaultSize
	) : wxPanel(parent, winid, pos, size)
	{
		activated = false;
		panel_state = HPANEL_DEFAULT;
		col_default = defaultColour;
		col_highlight = highlightColour;
		col_activated = activatedColour;
		SetBackgroundColour(defaultColour);
	}
	

	void SetSizerAndFit(wxSizer* sizer, bool deleteOld = true)
	{
		__super::SetSizerAndFit(sizer, deleteOld);
		BindRecursively(wxEVT_ENTER_WINDOW, &cHighlightPanel::OnMouseEnter, this);
		BindRecursively(wxEVT_LEAVE_WINDOW, &cHighlightPanel::OnMouseLeave, this);
	}

	//void SetColours(wxColour defaultColour, wxColour highlightColour)
	//{
	//	col_default = defaultColour;
	//	col_highlight = highlightColour;
	//	this->Refresh();
	//}

	// Binds the specified event to all component's children
	template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
	void BindRecursively(
		const EventTag eventType,
		void (Class::* method)(EventArg&),
		EventHandler* handler)
	{
		bindRecursively(this, eventType, method, handler);
	}

	void Activate() {
		if (!activated) {
			activated = true;
			this->SetBackgroundColour(col_activated);
			this->Refresh();
		}
	}

	void Deactivate() {
		if (activated) {
			activated = false;
			if (!this->IsMouseInWindow()) {
				SetBackgroundColour(col_default);
			}
			else {
				SetBackgroundColour(col_highlight);
			}
			this->Refresh();
		}
	}

private:
	template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
	void bindRecursively(
		wxWindow* component,
		const EventTag eventType,
		void (Class::* method)(EventArg&),
		EventHandler* handler)
	{
		if (component)
		{
			component->Bind(eventType, method, handler);

			wxWindowListNode* pclNode = component->GetChildren().GetFirst();
			while (pclNode)
			{
				wxWindow* pclChild = pclNode->GetData();
				this->bindRecursively(pclChild, eventType, method, handler);

				pclNode = pclNode->GetNext();
			}
		}
	}

	
	void OnMouseEnter(wxMouseEvent& event) {
		if (panel_state == HPANEL_DEFAULT && !activated) {
			SetBackgroundColour(col_highlight);
			this->Refresh();
		}
		event.Skip();
	}

	void OnMouseLeave(wxMouseEvent& event) {
		if (!this->IsMouseInWindow() && !activated) {
			SetBackgroundColour(col_default);
			this->Refresh();
		}
		event.Skip();
	}
};

