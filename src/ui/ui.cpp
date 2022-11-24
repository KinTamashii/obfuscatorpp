///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "ui.hpp"



wxBEGIN_EVENT_TABLE(mFrame, wxFrame)
    EVT_MENU(ID_menuFile_Open, mFrame::OnMenuFile_Open)
    EVT_MENU(ID_menuFile_Save, mFrame::OnMenuFile_Save)
    EVT_MENU(ID_menuFile_SaveAs, mFrame::OnMenuFile_SaveAs)
    EVT_MENU(ID_menuFile_SaveTranslation, mFrame::OnMenuFile_SaveTranslation)
    EVT_MENU(ID_menuFile_SaveTranslationAs, mFrame::OnMenuFile_SaveTranslationAs)
	EVT_MENU(ID_menuFile_Close, mFrame::OnMenuFile_Close)
	EVT_MENU(ID_menuTools_Transform_File, mFrame::OnMenuTools_TransformFile)
	EVT_MENU(ID_menuTools_Transform_Folder, mFrame::OnMenuTools_TransformFolder)
    EVT_MENU(ID_menuTools_Generate_File, mFrame::OnMenuTools_GenerateFile)
	EVT_MENU(ID_menuTools_Generate_Folder, mFrame::OnMenuTools_GenerateFolder)
    EVT_BUTTON(ID_TransformButton, mFrame::OnTransformButton)
    EVT_BUTTON(ID_GenerateButton, mFrame::OnGenerateButton)
	EVT_BUTTON(ID_SwapButton, mFrame::OnSwapButton)
    EVT_BUTTON(ID_AccumulateButton, mFrame::OnAccumulateButton)
	EVT_BUTTON(ID_CopyButton, mFrame::OnCopyButton)
    EVT_BUTTON(ID_languageStateButton, mFrame::OnLanguageStateButton)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, mFrame::OnThreadSetGaugeValue)
    EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MTHREAD_COMPLETED, mFrame::OnThreadCompletion)
	EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MTHREAD_SET_GAUGE_RANGE, mFrame::OnThreadSetGaugeRange)
    EVT_COMMAND(wxID_ANY, wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE, mFrame::OnThreadSetGaugeMessage)
    EVT_CLOSE(mFrame::OnClose)
    EVT_COMMAND(wxID_ANY, wxEVT_LANGUAGE_STATE_FRAME_CLOSE, mFrame::OnLanguageStateFrameClose)
wxEND_EVENT_TABLE()



///////////////////////////////////////////////////////////////////////////

mFrame::mFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* topSizer;
	topSizer = new wxBoxSizer( wxVERTICAL );

	infoPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxTAB_TRAVERSAL );
	wxBoxSizer* infoPanelSizer;
	infoPanelSizer = new wxBoxSizer( wxHORIZONTAL );
    
	originalLabel = new wxStaticText( infoPanel, wxID_ANY, wxT("Original"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	originalLabel->Wrap( -1 );
	infoPanelSizer->Add( originalLabel, 1, wxALIGN_BOTTOM, 5 );

	actionPanel = new wxPanel( infoPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* actionPanelSizer;
	actionPanelSizer = new wxBoxSizer( wxVERTICAL );

	actionUpperPanel = new wxPanel(actionPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* actionUpperPanelSizer;
	actionUpperPanelSizer = new wxBoxSizer( wxHORIZONTAL );
    
	wxString languageChoiceChoices[(sizeof(google::language_names)/sizeof(google::language_names[0])) - 1] {};
    auto it = &google::language_names[1];
    for (auto &item : languageChoiceChoices)
        item = *it++;

	int languageChoiceNChoices = sizeof( languageChoiceChoices ) / sizeof( wxString );
	languageChoice = new wxChoice( actionUpperPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, languageChoiceNChoices, languageChoiceChoices, 0 );
	languageChoice->SetSelection( pref.GetDefaultLanguage()-1 );
	actionUpperPanelSizer->Add( languageChoice, 0, wxALIGN_CENTER|wxRIGHT, 5 );

	numObfuscationsSpinBox = new wxSpinCtrl(actionUpperPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, 0, INT_MAX, pref.GetDefaultNumIterations());
	actionUpperPanelSizer->Add(numObfuscationsSpinBox, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

    includeNonTextCheckBox = new wxCheckBox(actionUpperPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    includeNonTextCheckBox->SetLabelText("Include Non-Text");
    includeNonTextCheckBox->SetValue(pref.GetIncludeNonText());
    actionUpperPanelSizer->Add(includeNonTextCheckBox, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

    languageStateButton = new wxButton(actionUpperPanel, ID_languageStateButton, wxT("Languages"), wxDefaultPosition, wxDefaultSize, 0);
    actionUpperPanelSizer->Add(languageStateButton, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5);

	progressGauge = new labeledGaugeFrame(this, ID_progressGauge, 100);
    languageState = new languageStateFrame(this, ID_languageState);
	
	actionUpperPanel->SetSizer(actionUpperPanelSizer);
	actionUpperPanel->Layout();
	actionUpperPanelSizer->Fit(actionUpperPanel);

	actionPanelSizer->Add(actionUpperPanel, 0, wxALIGN_CENTER, 0);

	actionLowerPanel = new wxPanel(actionPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
	wxBoxSizer* actionLowerPanelSizer;
	actionLowerPanelSizer = new wxBoxSizer(wxHORIZONTAL);

	transformButton = new wxButton( actionLowerPanel, ID_TransformButton, wxT(""), wxDefaultPosition, wxDefaultSize, 0 );
	actionLowerPanelSizer->Add( transformButton, 1, wxALIGN_CENTER|wxALL, 5 );

    generateButton = new wxButton(actionLowerPanel, ID_GenerateButton, wxT("Generate"), wxDefaultPosition, wxDefaultSize, 0);
    actionLowerPanelSizer->Add(generateButton, 1, wxALIGN_CENTER|wxALL, 5);

	swapButton = new wxButton( actionLowerPanel, ID_SwapButton, wxT("Swap"), wxDefaultPosition, wxDefaultSize, 0 );
	actionLowerPanelSizer->Add( swapButton, 1, wxALIGN_CENTER|wxALL, 5 );

    accumulateButton = new wxButton( actionLowerPanel, ID_AccumulateButton, wxT("Accumulate"), wxDefaultPosition, wxDefaultSize, 0 );
	actionLowerPanelSizer->Add( accumulateButton, 1, wxALIGN_CENTER|wxALL, 5 );

	copyButton = new wxButton( actionLowerPanel, ID_CopyButton, wxT("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
	actionLowerPanelSizer->Add( copyButton, 1, wxALIGN_CENTER|wxALL, 5 );

	actionLowerPanel->SetSizer(actionLowerPanelSizer);
	actionLowerPanel->Layout();
	actionLowerPanelSizer->Fit(actionLowerPanel);

	actionPanelSizer->Add(actionLowerPanel, 1, wxALIGN_CENTER|wxALL, 0);
	actionPanel->SetSizer( actionPanelSizer );
	actionPanel->Layout();
	actionPanelSizer->Fit( actionPanel );
	infoPanelSizer->Add( actionPanel, 0, wxEXPAND | wxTOP, 5 );

	translationLabel = new wxStaticText( infoPanel, wxID_ANY, wxT("Result"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	translationLabel->Wrap( -1 );
	infoPanelSizer->Add( translationLabel, 1, wxALIGN_BOTTOM, 5 );


	infoPanel->SetSizer( infoPanelSizer );
	infoPanel->Layout();
	infoPanelSizer->Fit( infoPanel );
	topSizer->Add( infoPanel, 0, wxEXPAND | wxALL, 0 );

	ioPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN|wxTAB_TRAVERSAL );
	wxBoxSizer* ioPanelSizer;
	ioPanelSizer = new wxBoxSizer( wxHORIZONTAL );

	inputTextCtrl = new wxTextCtrl( ioPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxBORDER_RAISED|wxTE_PROCESS_TAB );
    inputTextCtrl->OSXDisableAllSmartSubstitutions();
	ioPanelSizer->Add( inputTextCtrl, 1, wxEXPAND, 5 );

	outputTextCtrl = new wxTextCtrl( ioPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxBORDER_RAISED );
    outputTextCtrl->OSXDisableAllSmartSubstitutions();
	ioPanelSizer->Add( outputTextCtrl, 1, wxEXPAND, 5 );


	ioPanel->SetSizer( ioPanelSizer );
	ioPanel->Layout();
	ioPanelSizer->Fit( ioPanel );
	topSizer->Add( ioPanel, 1, wxEXPAND | wxALL, 5 );


	this->SetSizer( topSizer );
	this->Layout();
	menuBar = new wxMenuBar( 0 );
	menuFile = new wxMenu();
	wxMenuItem* menuFile_Open;
	menuFile_Open = new wxMenuItem( menuFile, ID_menuFile_Open, wxString( wxT("Open\tCtrl+O") ) , wxT("Opens a text file and load the text into the input box."), wxITEM_NORMAL );
	menuFile->Append( menuFile_Open );

	menuFile->AppendSeparator();

	wxMenuItem* menuFile_Save;
	menuFile_Save = new wxMenuItem( menuFile, ID_menuFile_Save, wxString( wxT("Save\tCtrl+S") ) , wxT("Saves the text in the output box to a text file."), wxITEM_NORMAL );
	menuFile->Append( menuFile_Save );

	wxMenuItem* menuFile_SaveAs;
	menuFile_SaveAs = new wxMenuItem( menuFile, ID_menuFile_SaveAs, wxString( wxT("Save As\tCtrl+Shift+S") ) , wxT("Saves the text in the output box to a text file in the specified location."), wxITEM_NORMAL );
	menuFile->Append( menuFile_SaveAs );

	menuFile->AppendSeparator();

	wxMenuItem* menuFile_SaveTranslation;
	menuFile_SaveTranslation = new wxMenuItem( menuFile, ID_menuFile_SaveTranslation, wxString( wxT("Save Translation\tCtrl+T") ) , wxT("Saves the text in the input box and the text in the output box both in seperate text files for comparison."), wxITEM_NORMAL );
	menuFile->Append( menuFile_SaveTranslation );

	wxMenuItem* menuFile_SaveTranslationAs;
	menuFile_SaveTranslationAs = new wxMenuItem( menuFile, ID_menuFile_SaveTranslationAs, wxString( wxT("Save Translation As\tCtrl+Shift+T") ) , wxT("Saves the text in the input box and the text in the output box both in seperate text files in a specified location for comparison."), wxITEM_NORMAL );
	menuFile->Append( menuFile_SaveTranslationAs );

	menuFile->AppendSeparator();

	wxMenuItem* menuFile_Close;
	menuFile_Close = new wxMenuItem(menuFile, ID_menuFile_Close, wxString(wxT("Close\tCtrl+W")), wxT("Closes the window."), wxITEM_NORMAL);
	menuFile->Append(menuFile_Close);

	menuBar->Append( menuFile, wxT("File") );

	menuTools = new wxMenu();

	menuTools_Transform = new wxMenu();

	
	menuTools_Transform_File = new wxMenuItem( menuTools_Transform, ID_menuTools_Transform_File, wxString( wxT("File") ) , "", wxITEM_NORMAL );
	menuTools_Transform->Append( menuTools_Transform_File );

	
	menuTools_Transform_Folder = new wxMenuItem( menuTools_Transform, ID_menuTools_Transform_Folder, wxString( wxT("Folder") ) , "", wxITEM_NORMAL );
	menuTools_Transform->Append( menuTools_Transform_Folder );

	menuTools->Append(ID_menuTools_Transform, wxT("Transform"), menuTools_Transform);

    menuTools_Generate = new wxMenu();

    menuTools_Generate_File = new wxMenuItem( menuTools_Generate, ID_menuTools_Generate_File, wxString( wxT("File") ) , "", wxITEM_NORMAL );
	menuTools_Generate->Append( menuTools_Generate_File );

	
	menuTools_Generate_Folder = new wxMenuItem( menuTools_Generate, ID_menuTools_Generate_Folder, wxString( wxT("Folder") ) , "", wxITEM_NORMAL );
	menuTools_Generate->Append( menuTools_Generate_Folder );


    menuTools->Append(ID_menuTools_Generate, wxT("Generate"), menuTools_Generate);

    

	menuBar->Append( menuTools, wxT("Tools") );

    

	this->SetMenuBar( menuBar );

	statusBar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	Fit();

	SetMinSize(GetSize());

	SetSize(wxSize(GetSize().GetX(),GetSize().GetY()*2));
	
    
}


mFrame::~mFrame()
{
}




void mFrame::OnTransformButton(wxCommandEvent &evt) {
	DoStartThread<ioBoxTransferThread>();
}

void mFrame::OnGenerateButton(wxCommandEvent &evt) {
    wxFileDialog dlg(this, _("Save as a CSV file"), "", "",
                "CSV files (*.csv)|*.csv", wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() == wxID_CANCEL)
        return;

    generationsDialog gen_dlg(this);
    if (gen_dlg.ShowModal() == wxID_CANCEL)
        return;

    DoStartThread<ioBoxGenerateThread>(&dlg, gen_dlg.GetNumGenerations());
}

void mFrame::OnSwapButton(wxCommandEvent &evt) {
	if (outputTextCtrl->IsEmpty()) return;
	inputTextCtrl->SetValue(outputTextCtrl->GetValue());
	DoStartThread<ioBoxTransferThread>();
}

void mFrame::OnAccumulateButton(wxCommandEvent &evt) {
    if (outputTextCtrl->IsEmpty()) return;
    inputTextCtrl->SetValue(inputTextCtrl->GetValue()+'\n'+outputTextCtrl->GetValue());
	DoStartThread<ioBoxTransferThread>();
}

void mFrame::OnCopyButton(wxCommandEvent &evt) {
	if (wxTheClipboard->Open()) {
		wxTheClipboard->SetData( new wxTextDataObject(outputTextCtrl->GetValue()) );
		wxTheClipboard->Close();
	}
}

void mFrame::OnLanguageStateButton(wxCommandEvent &evt) {
    languageState->Show();
    languageStateButton->Disable();
}

void mFrame::OnMenuFile_Open(wxCommandEvent &evt) {
    wxFileDialog dlg(
        this, _("Open a text file"), "", "",
            "TXT files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST
    );
    if (dlg.ShowModal() == wxID_CANCEL)
        return;
    
    inputTextCtrl->SetValue(file::read((std::string)dlg.GetPath()));
}


void mFrame::OnMenuFile_Save(wxCommandEvent &evt) {
    if (dir.empty())
        if (GetPath(_("Save a text file"), "TXT files (*.txt)|*.txt")) return;
    file::write((std::string)outputTextCtrl->GetValue(), dir);
}
void mFrame::OnMenuFile_SaveAs(wxCommandEvent &evt) {
    if (GetPath(_("Save a text file"), "TXT files (*.txt)|*.txt")) return;
    file::write((std::string)outputTextCtrl->GetValue(), dir);
}
void mFrame::OnMenuFile_SaveTranslation(wxCommandEvent &evt) {
	if (dir.empty())
        if (GetPath(_("Save text files"), "TXT files (*.txt)|*.txt")) return;
	const std::string &cur_dir = file::path::basename(dir);
	file::write((std::string)inputTextCtrl->GetValue(), cur_dir, "original.txt");
	file::write((std::string)outputTextCtrl->GetValue(), cur_dir, "result.txt");
}
void mFrame::OnMenuFile_SaveTranslationAs(wxCommandEvent &evt) {
	if (GetPath(_("Save text files"), "TXT files (*.txt)|*.txt")) return;
	const std::string &cur_dir = file::path::basename(dir);
	file::write((std::string)inputTextCtrl->GetValue(), cur_dir, "original.txt");
	file::write((std::string)outputTextCtrl->GetValue(), cur_dir, "result.txt");

}


void mFrame::OnMenuFile_Close(wxCommandEvent &evt) {
	if (languageState->IsShown()) {
		languageState->Hide();
		languageStateButton->Enable();
	} else {
		Close();
	}
}

void mFrame::OnMenuTools_TransformFile(wxCommandEvent &evt) {
	wxFileDialog dlg(
        this, _("Open a text file"), "", "",
            "TXT files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE
    );
    if (dlg.ShowModal() == wxID_CANCEL)
        return;
	DoStartThread<fileTransformThread>(&dlg);
		
	
}

void mFrame::OnMenuTools_TransformFolder(wxCommandEvent &evt) {
	wxDirDialog dlg(
        this, _("Open a folder"), "",
            wxDD_DIR_MUST_EXIST|wxDD_MULTIPLE
    );
    if (dlg.ShowModal() == wxID_CANCEL)
        return;
	DoStartThread<folderTransformThread>(&dlg);
}

void mFrame::OnMenuTools_GenerateFile(wxCommandEvent &evt) {
    wxFileDialog dlg(
        this, _("Open a text file"), "", "",
            "TXT files (*.txt)|*.txt", wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE
    );
    if (dlg.ShowModal() == wxID_CANCEL)
        return;

    generationsDialog gen_dlg(this);
    if (gen_dlg.ShowModal() == wxID_CANCEL)
        return;
        
    DoStartThread<fileGenerateThread>(&dlg, gen_dlg.GetNumGenerations());
}
void mFrame::OnMenuTools_GenerateFolder(wxCommandEvent &evt) {
    wxDirDialog dlg(
        this, _("Open a folder"), "",
            wxDD_DIR_MUST_EXIST|wxDD_MULTIPLE
    );
    if (dlg.ShowModal() == wxID_CANCEL)
        return;

    generationsDialog gen_dlg(this);
    if (gen_dlg.ShowModal() == wxID_CANCEL)
        return;
        
    DoStartThread<folderGenerateThread>(&dlg, gen_dlg.GetNumGenerations());
}



template <typename THREAD, typename ...Args>
void mFrame::DoStartThread(Args... args) {
	if (pref.noActiveLanguages()) return;
	progressGauge->Activate();
	Layout();
	DisableTransform(); // Prevent any new action while the current one is running.
    m_pThread = new THREAD(this, args...);
    if ( m_pThread->Run() != wxTHREAD_NO_ERROR ) {
        wxLogError("Can't create the thread!");
        delete m_pThread;
        m_pThread = nullptr;
    }
}


void mFrame::OnLanguageStateFrameClose(wxCommandEvent &evt) {
    languageState->Hide();
    languageStateButton->Enable();
}


void mFrame::OnClose(wxCloseEvent& evt) {
    
    /*scope*/ {
        wxCriticalSectionLocker enter(m_pThreadCS);
        if (m_pThread) {
            wxMessageOutputDebug().Printf("MFRAME: deleting thread");
            if (m_pThread->Delete() != wxTHREAD_NO_ERROR )
                wxLogError("Can't delete the thread!");
        }
    }       
    while (1) {
		/*scope*/ {
            wxCriticalSectionLocker enter(m_pThreadCS);
            if (!m_pThread) break;
        }
        wxThread::This()->Sleep(1); // wait for thread completion
    }
    pref.SetData(
        includeNonTextCheckBox->GetValue(),
        languageChoice->GetSelection()+1,
        numObfuscationsSpinBox->GetValue()
    ); // Save the ui state.
    
    
    Destroy();
}
