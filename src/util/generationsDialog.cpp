#include "generationsDialog.hpp"


wxBEGIN_EVENT_TABLE(generationsDialog, wxDialog)
    EVT_BUTTON(ID_okButton, generationsDialog::OnOkButton)
    EVT_BUTTON(ID_cancelButton, generationsDialog::OnCancelButton)
wxEND_EVENT_TABLE()


generationsDialog::generationsDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* topSizer;
	topSizer = new wxBoxSizer( wxVERTICAL );

	generationsSpinCtrl = new wxSpinCtrl( this, ID_generationsSpinCtrl, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 2147483647, 0 );
	topSizer->Add( generationsSpinCtrl, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT|wxTOP, 5 );

	modalPanel = new wxPanel( this, ID_modalPanel, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* modalPanelSizer;
	modalPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	okButton = new wxButton( modalPanel, ID_okButton, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	modalPanelSizer->Add( okButton, 0, wxALL, 5 );

	cancelButton = new wxButton( modalPanel, ID_cancelButton, wxT("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	modalPanelSizer->Add( cancelButton, 0, wxALL, 5 );


	modalPanel->SetSizer( modalPanelSizer );
	modalPanel->Layout();
	modalPanelSizer->Fit( modalPanel );
	topSizer->Add( modalPanel, 1, wxEXPAND|wxLEFT|wxRIGHT, 10 );


	this->SetSizer( topSizer );
	this->Layout();
	topSizer->Fit( this );

	this->Centre( wxBOTH );
}

generationsDialog::~generationsDialog()
{
}


void generationsDialog::OnOkButton(wxCommandEvent &evt) {
    EndModal(wxID_OK);
}

void generationsDialog::OnCancelButton(wxCommandEvent &evt) {
    EndModal(wxID_CANCEL);
}