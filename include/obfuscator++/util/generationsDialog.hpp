#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/spinctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/dialog.h>


/* Gets a number from the user; used for the spreadsheet comparison generator's number of repititions. */
class generationsDialog : public wxDialog
{
	private:
        wxDECLARE_EVENT_TABLE();
	protected:
		wxSpinCtrl* generationsSpinCtrl;
		wxPanel* modalPanel;
		wxButton* okButton;
		wxButton* cancelButton;

	public:
        enum {
            ID_generationsSpinCtrl = 1,
            ID_modalPanel,
            ID_okButton,
            ID_cancelButton
        };


		generationsDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("How many generations?"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
		~generationsDialog();

        void OnOkButton(wxCommandEvent &evt);
        void OnCancelButton(wxCommandEvent &evt);

        inline int GetNumGenerations() {
            return generationsSpinCtrl->GetValue();
        }

};
