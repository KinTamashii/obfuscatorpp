#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/textctrl.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/clipbrd.h>
#include <wx/progdlg.h>

#include <wx/spinctrl.h>


#include <util/file.hpp>

#include <util/gaugeFrame.hpp>
#include <preferences.hpp>

#include <thread.hpp>
#include <util/generationsDialog.hpp>
#include <util/languageStateFrame.hpp>


/* Main ui frame. */
class mFrame : public wxFrame {
    friend mThread;
    friend ioBoxTransferThread;
    friend fileTransformThread;
    friend folderTransformThread;
    friend ioBoxGenerateThread;
	private:
        wxDECLARE_EVENT_TABLE();
        std::string dir; /* Stores the last used directory for opening/saving a file. */

	protected:
        volatile bool locked = false; // Prevent compiler from optimizing this out.
		wxPanel* infoPanel;
		wxStaticText* originalLabel;
		wxPanel* actionPanel;
        wxPanel* actionUpperPanel;
        wxPanel* actionLowerPanel;
		wxChoice* languageChoice;
        wxSpinCtrl* numObfuscationsSpinBox;
        wxCheckBox* includeNonTextCheckBox;
        wxButton *languageStateButton;
        languageStateFrame *languageState;
        labeledGaugeFrame *progressGauge;
		wxButton* transformButton;
        wxButton* generateButton;
        wxButton* swapButton;
        wxButton* accumulateButton;
        wxButton* copyButton;
		wxStaticText* translationLabel;
		wxPanel* ioPanel;
		wxTextCtrl* inputTextCtrl;
		wxTextCtrl* outputTextCtrl;
		wxMenuBar* menuBar;
		wxMenu* menuFile;
		wxMenu* menuTools;
            wxMenu* menuTools_Transform;
                wxMenuItem* menuTools_Transform_File;
                wxMenuItem* menuTools_Transform_Folder;
            wxMenu* menuTools_Generate;
                wxMenuItem* menuTools_Generate_File;
                wxMenuItem* menuTools_Generate_Folder;
		wxStatusBar* statusBar;
        std::string (*Transform)(mFrame*, const std::string &, int, int) = nullptr; // Function to call for ui actions.
        bool gaugeIsWorking;

        
	public:
        mThread *m_pThread = nullptr;
        wxCriticalSection m_pThreadCS;    // protects the m_pThread pointer

        template <typename THREAD, typename ...Args>
        void DoStartThread(Args... args);
        void DoPauseThread();
        void DoResumeThread() {}
        void OnThreadSetGaugeValue(wxCommandEvent&);
        void OnThreadSetGaugeRange(wxCommandEvent &evt);
        void OnThreadSetGaugeMessage(wxCommandEvent &evt);
        void OnThreadCompletion(wxCommandEvent&);
        void OnClose(wxCloseEvent&);

        enum {
            ID_TransformButton = 1,
            ID_GenerateButton,
            ID_SwapButton,
            ID_AccumulateButton,
            ID_CopyButton,
            ID_progressGauge,
            ID_languageState,
            ID_languageStateButton,
            //menuFile
                ID_menuFile_Open,
                ID_menuFile_Save,
                ID_menuFile_SaveAs,
                ID_menuFile_SaveTranslation,
                ID_menuFile_SaveTranslationAs,
                ID_menuFile_Close,
            //menuTools
                ID_menuTools_Transform,
                    ID_menuTools_Transform_File,
                    ID_menuTools_Transform_Folder,
                ID_menuTools_Generate,
                    ID_menuTools_Generate_File,
                    ID_menuTools_Generate_Folder,
        };

        

		mFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
        ~mFrame();

        /* Calls the Transform function with the text in the input box as input,
            puts the result in the output box. */
        void OnTransformButton(wxCommandEvent &evt);

        /* Calls the Transform function multiple times with the text in the input box as input,
            saving a spreadsheet in the directory specified by the user. */
        void OnGenerateButton(wxCommandEvent &evt);

        /* Preforms the actions of OnTransformButton, but puts whatever text
        is currently in the output box into the input box beforehand. */
        void OnSwapButton(wxCommandEvent &evt);

        /* Preforms the actions of OnTransformButton, but appends whatever text
        is currently in the output box to the end of the input box beforehand. */
        void OnAccumulateButton(wxCommandEvent &evt);

        /* Copys the text in the output box. */
        void OnCopyButton(wxCommandEvent &evt);

        /* Opens a frame allowing the user to select which languages to use. */
        void OnLanguageStateButton(wxCommandEvent &evt);

        void OnLanguageStateFrameClose(wxCommandEvent &evt);

        //menuFile
            /* Opens a text file and puts the text into the input box. */
            void OnMenuFile_Open(wxCommandEvent &evt);
            /* Saves the text in the output box. */
            void OnMenuFile_Save(wxCommandEvent &evt);
            /* Saves the text in the output box in the specifed location. */
            void OnMenuFile_SaveAs(wxCommandEvent &evt);
            /* Saves the text in the input and output box. */
            void OnMenuFile_SaveTranslation(wxCommandEvent &evt);
            /* Saves the text in the input and output box in the specifed location. */
            void OnMenuFile_SaveTranslationAs(wxCommandEvent &evt);
            /* Closes the program, or the hides the languageStateFrame if shown. */
            void OnMenuFile_Close(wxCommandEvent &evt);
        //menuTools
            //Transform
                /* Calls the transform function for each selected file and saves the results. */
                void OnMenuTools_TransformFile(wxCommandEvent &evt);
                /* Calls the transform function for every file in each selected folder and saves the results. */
                void OnMenuTools_TransformFolder(wxCommandEvent &evt);
            //Generate
                /* Calls the transform function multiple times for each
                selected file and saves a comparison spreadsheet for each. */
                void OnMenuTools_GenerateFile(wxCommandEvent &evt);
                /* Calls the transform function multiple times for every file in
                each selected folder and saves a comparison spreadsheet for each. */
                void OnMenuTools_GenerateFolder(wxCommandEvent &evt);

        inline bool GetPath(
            const wxString &message=wxFileSelectorPromptStr,
            const wxString &wildcard=wxFileSelectorDefaultWildcardStr
        ) {
             wxFileDialog 
                dlg(this, message, "", "",
                            wildcard, wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
            if (dlg.ShowModal() == wxID_CANCEL)
                return true;
            dir = dlg.GetPath();

            return false;
            
        }

        static inline std::string GetFileHelp(std::string label) {
            label[0] = std::toupper(label[0]);
            return label+std::string("s the selected text file(s).");
        }
        static inline std::string GetFolderHelp(std::string label) {
            label[0] = std::tolower(label[0]);
            return std::string("Recursively ")+label+std::string("s all text files in the selected directory/directories.");
        }

        static inline std::string GetGenerateFileHelp(std::string label) {
            label[0] = std::toupper(label[0]);
            return label+std::string("s the selected text file(s) multiple times creating a comparison spreadsheet between the original and each resulting text.");
        }
        static inline std::string GetGenerateFolderHelp(std::string label) {
            label[0] = std::tolower(label[0]);
            return std::string("Recursively ")+label+std::string("s all text files in the selected directory/directories multiple times creating a comparison spreadsheet between the original and each resulting text.");
        }
        /* Prevent the user from preforming any of these actions. */
        inline void DisableTransform() {
            transformButton->Disable();
            generateButton->Disable();
            swapButton->Disable();
            accumulateButton->Disable();
            menuTools_Transform_File->Enable(false);
            menuTools_Transform_Folder->Enable(false);
        }
        /* Allow the user to preform these actions. */
        inline void EnableTransform() {
            transformButton->Enable();
            generateButton->Enable();
            swapButton->Enable();
            accumulateButton->Enable();
            menuTools_Transform_File->Enable();
            menuTools_Transform_Folder->Enable();
        }
        template <typename T>
        inline void SendEvent(const wxEventTypeTag<wxThreadEvent> &tag, T item) {
            wxThreadEvent *evt = new wxThreadEvent(tag);
            if constexpr (std::is_arithmetic_v<T>) {
                evt->SetInt(item);
            } else if constexpr (std::is_same<T, const std::string&>::value) {
                evt->SetString(item);
            }
            wxQueueEvent(this, evt);
        }
        inline void SendEvent(const wxEventTypeTag<wxThreadEvent> &tag, const std::string &item) {
            SendEvent<const std::string &>(tag, item);
        }

        template <typename T>
        inline void SendBlockingEvent(const wxEventTypeTag<wxThreadEvent> &tag, T item) {
            SendEvent<T>(tag, item);
            locked = true;
            
            while (locked) {}
        }

        inline void SendBlockingEvent(const wxEventTypeTag<wxThreadEvent> &tag, const std::string &item) {
            SendBlockingEvent<const std::string &>(tag, item);
        }

        inline void Unlock() {
            locked = false;
        }


        
        
        inline void SetTransform(std::string (*Transform)(mFrame*, const std::string &, int, int), const std::string &label) {
            this->Transform = Transform;
            this->transformButton->SetLabel(label);
            menuTools->SetLabel(ID_menuTools_Transform, label);

            
            menuTools_Transform_File->SetHelp(GetFileHelp(label));
            menuTools_Transform_Folder->SetHelp(GetFolderHelp(label));
            menuTools_Generate_File->SetHelp(GetGenerateFileHelp(label));
            menuTools_Generate_Folder->SetHelp(GetGenerateFolderHelp(label));
        };

        inline int GetProgressValue() {
            return progressGauge->GetValue();
        }

        inline int GetProgressRange() {
            return progressGauge->GetRange();
        }


        inline bool IncludeNonText() {
            return includeNonTextCheckBox->GetValue();
        }

        inline bool IsWorking() {
            return progressGauge->IsWorking();
        }
        inline bool IsAsleep() {
            return progressGauge->IsAsleep();
        }

};

