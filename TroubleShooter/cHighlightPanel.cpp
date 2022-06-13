#include "cHighlightPanel.h"

cHighlightPanel::cHighlightPanel(
	wxWindow* parent,
	wxColour defaultColour,
	wxColour highlightColour,
	wxWindowID winid,
	wxPoint pos,
	const wxSize size
) : wxPanel(parent, winid, pos, size)
{
    colDefault = defaultColour;
    colHighlight = highlightColour;

	SetBackgroundColour(defaultColour);
    //this->Bind(wxEVT_ENTER_WINDOW, &cHighlightPanel::OnMouseEnter, this);
    BindRecursively(this, wxEVT_ENTER_WINDOW, &cHighlightPanel::OnMouseEnter, this);
    this->Bind(wxEVT_LEAVE_WINDOW, &cHighlightPanel::OnMouseLeave, this);
}

void cHighlightPanel::SetSizerAndFit(wxSizer* sizer, bool deleteOld)
{
	__super::SetSizerAndFit(sizer, deleteOld);
}

template <typename EventTag, typename Class, typename EventArg, typename EventHandler>
void cHighlightPanel::BindRecursively(
    wxWindow* component, 
    EventTag eventType, 
    void (Class::*method)(EventArg&),
    EventHandler* handler)
{
    if (component)
    {
        component->Bind(eventType, method, handler);

        wxWindowListNode* pclNode = component->GetChildren().GetFirst();
        while (pclNode)
        {
            wxWindow* pclChild = pclNode->GetData();
            this->BindRecursively(pclChild, eventType, method, handler);

            pclNode = pclNode->GetNext();
        }
    }
}

void cHighlightPanel::OnMouseEnter(wxMouseEvent& event)
{
    SetBackgroundColour(colHighlight);
    this->Refresh();
    event.Skip();
}

void cHighlightPanel::OnMouseLeave(wxMouseEvent& event)
{
    SetBackgroundColour(colDefault);
    this->Refresh();
    event.Skip();
}

