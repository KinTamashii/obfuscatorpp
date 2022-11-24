#pragma once



#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/sizer.h>
#include <wx/gdicmn.h>
#include <wx/panel.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/frame.h>
#include <wx/checkbox.h>

#include <google.hpp>
#include <preferences.hpp>


wxDECLARE_EVENT(wxEVT_LANGUAGE_STATE_FRAME_CLOSE, wxCommandEvent);

/* Select which languages to allow in an obfuscation. */
class languageStateFrame : public wxFrame {
	private:
		wxDECLARE_EVENT_TABLE();
	protected:
		wxCheckBox *languageStates[(sizeof(google::language_names)/sizeof(google::language_names[0]))-1];

	public:
		enum {
			ID_activeButton = 1,
			ID_inactiveButton,
			ID_toggleButton,
			ID_CheckBox,
		};

		languageStateFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX) );

		~languageStateFrame();

		/* Set all languages to true. */
		void OnActiveButton(wxCommandEvent &evt);

		/* Set all languages to false. */
		void OnInactiveButton(wxCommandEvent &evt);

		/* Toggle the state of every language. */
		void OnToggleButton(wxCommandEvent &evt);


		void OnClose(wxCloseEvent& evt);

		/* Update the data containing the state of the languages. */
		void OnCheckBox(wxCommandEvent &evt);

};

