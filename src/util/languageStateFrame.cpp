///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "languageStateFrame.hpp"

wxDEFINE_EVENT(wxEVT_LANGUAGE_STATE_FRAME_CLOSE, wxCommandEvent);

wxBEGIN_EVENT_TABLE(languageStateFrame, wxFrame)
    EVT_BUTTON(ID_activeButton, languageStateFrame::OnActiveButton)
    EVT_BUTTON(ID_inactiveButton, languageStateFrame::OnInactiveButton)
    EVT_BUTTON(ID_toggleButton, languageStateFrame::OnToggleButton)
    EVT_CLOSE(languageStateFrame::OnClose)
wxEND_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////

languageStateFrame::languageStateFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
    wxBoxSizer* topSizer = new wxBoxSizer( wxVERTICAL );

    // topSizer->Add();

    wxPanel* upperPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* upperPanelSizer = new wxBoxSizer( wxHORIZONTAL );


    wxButton* activeButton = new wxButton(upperPanel, ID_activeButton, wxT("All"), wxDefaultPosition, wxDefaultSize, 0);
    upperPanelSizer->Add(activeButton, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

    wxButton* inactiveButton = new wxButton(upperPanel, ID_inactiveButton, wxT("None"), wxDefaultPosition, wxDefaultSize, 0);
    upperPanelSizer->Add(inactiveButton, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

    wxButton* toggleButton = new wxButton(upperPanel, ID_toggleButton, wxT("Toggle"), wxDefaultPosition, wxDefaultSize, 0);
    upperPanelSizer->Add(toggleButton, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

    upperPanel->SetSizer(upperPanelSizer);
    upperPanel->Layout();
    upperPanelSizer->Fit(upperPanel);

    topSizer->Add(upperPanel, 0, wxALIGN_CENTER | wxUP, 5);

    wxPanel* lowerPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    wxBoxSizer* lowerPanelSizer = new wxBoxSizer( wxHORIZONTAL );
    
    #define num_languages (int)((sizeof(google::language_names)/sizeof(google::language_names[0]))-1)
    

    for (int i = 0; i < num_languages; ) {
        
        
        wxPanel* langPanel = new wxPanel( lowerPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
        wxBoxSizer* langPanelSizer = new wxBoxSizer( wxVERTICAL );

        int j = std::min(i + 19, num_languages);
        for (;i < j;i++) {
            wxCheckBox* chkbox = new wxCheckBox(langPanel, ID_CheckBox+i, google::language_names[i+1], wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
            languageStates[i] = chkbox;
            chkbox->SetValue(pref.getActiveLanguage(i));
            Bind(wxEVT_CHECKBOX, &languageStateFrame::OnCheckBox, this, ID_CheckBox+i);
            langPanelSizer->Add(
                chkbox,
                1, wxEXPAND, 0
            );
        }

        

        langPanel->SetSizer( langPanelSizer );
        langPanel->Layout();
        langPanelSizer->Fit( langPanel );
        
        lowerPanelSizer->Add( langPanel, 1, wxALIGN_CENTER, 0 );
        
    }

    lowerPanel->SetSizer(lowerPanelSizer);
    lowerPanel->Layout();
    lowerPanelSizer->Fit(lowerPanel);


    topSizer->Add(lowerPanel, 0, wxEXPAND | wxALL, 5);


	this->SetSizer( topSizer );
	this->Layout();

	this->Centre( wxBOTH );
    Fit();
}

languageStateFrame::~languageStateFrame()
{
}

void languageStateFrame::OnActiveButton(wxCommandEvent &evt) {
    for (int i = 0; i < num_languages; i++) {
        languageStates[i]->SetValue(true);
        pref.getActiveLanguage(i) = true;
    }
}

void languageStateFrame::OnInactiveButton(wxCommandEvent &evt) {
    for (int i = 0; i < num_languages; i++) {
        languageStates[i]->SetValue(false);
        pref.getActiveLanguage(i) = false;
    }
}

void languageStateFrame::OnToggleButton(wxCommandEvent &evt) {
    for (int i = 0; i < num_languages; i++) {
        bool value = !languageStates[i]->GetValue();
        languageStates[i]->SetValue(value);
        pref.getActiveLanguage(i) = value;
    }
}

void languageStateFrame::OnCheckBox(wxCommandEvent &evt) {
    int index = evt.GetId()-ID_CheckBox;
    pref.getActiveLanguage(index) = languageStates[index]->GetValue();
}


void languageStateFrame::OnClose(wxCloseEvent& evt) {
    wxQueueEvent(GetParent(), new wxCommandEvent(wxEVT_LANGUAGE_STATE_FRAME_CLOSE));
    if ( evt.CanVeto() ) // Don't allow to be closed by itself
    {
        evt.Veto();
    }

}