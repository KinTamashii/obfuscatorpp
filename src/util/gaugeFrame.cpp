#include "gaugeFrame.hpp"


wxBEGIN_EVENT_TABLE(labeledGaugeFrame, wxFrame)
	EVT_BUTTON(ID_cancelButton, labeledGaugeFrame::OnCancelButton)
wxEND_EVENT_TABLE()


labeledGaugeFrame::labeledGaugeFrame( wxWindow* parent, wxWindowID id, int range, const wxString& title, const wxPoint& pos, long style, const wxValidator &validator, const wxString &name )
: wxFrame( parent, id, title, pos, wxDefaultSize, style, name ) {
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	this->SetMinSize(wxSize(200,-1));
	wxBoxSizer* topSizer;
	topSizer = new wxBoxSizer( wxVERTICAL );

	labelText = new wxStaticText(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize);

	topSizer->Add(labelText, 0, wxCENTER | wxLEFT | wxRIGHT, 5);
	
    gauge = new wxGauge(this, wxID_ANY, range, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, validator);
	

	topSizer->Add( gauge, 1, wxEXPAND, 0 );

	cancelButton = new wxButton(this, ID_cancelButton, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );

	topSizer->Add(cancelButton, 0, wxCENTER | wxDOWN, 5);

	this->SetSizer( topSizer );
	this->Fit();
	this->Layout();
	this->Centre( wxBOTH );
	
}


void labeledGaugeFrame::OnCancelButton(wxCommandEvent&evt) {
	_isWorking = false;
	cancelButton->Disable();
}