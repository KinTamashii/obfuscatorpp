#pragma once
#include <wx/frame.h>
#include <wx/gauge.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/stattext.h>
#define FMT_HEADER_ONLY
#include <fmt/format.h>

/* Used to keep track of the progress of a task. */
class labeledGaugeFrame : public wxFrame {
    private:
        wxStaticText *labelText;
        wxGauge *gauge;
        wxButton *cancelButton;
        bool _isWorking;
        std::chrono::high_resolution_clock::time_point start;
    public:
        enum {
            ID_cancelButton = 10001
        };
        labeledGaugeFrame(wxWindow* parent, wxWindowID id = wxID_ANY, int range = 100, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, long style = wxTAB_TRAVERSAL|wxFRAME_FLOAT_ON_PARENT|wxCAPTION, const wxValidator &validator=wxDefaultValidator, const wxString &name=wxGaugeNameStr);

        inline void SetValue(int pos) {
            gauge->SetValue(pos);
            UpdateDisplay();
        }

        inline void SetRange(int pos) {
            gauge->SetRange(pos);
            UpdateDisplay();
        }

        inline int GetValue() {
            return gauge->GetValue();
        }
        inline int GetRange() {
            return gauge->GetRange();
        }

        /* Additional information about the task. */
        inline void SetMessage(const std::string &message) {
            labelText->SetLabelText(message);
            Fit();
            Layout();
        }

        inline void ClearMessage() {
            labelText->SetLabelText("");
            Fit();
            Layout();
        }

        inline long Elapsed() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count();
        }

        inline void UpdateDisplay() {
            long elapsed = Elapsed();
            
            SetTitle(
                fmt::format(
                    "{:0.3f} sec    {:0.2f}%",
                    (((GetRange()/(double)GetValue())*(double)elapsed)-elapsed)/1000,
                    GetValue()/(float)GetRange()*100
                )
            );
            
        }

        inline void Center() {
            wxPoint pt = GetParent()->GetPosition();
            wxSize sz = GetParent()->GetSize();
            wxSize size = GetSize();
            SetPosition(wxPoint(pt.x+((sz.x-size.x)/2), pt.y+((sz.y-size.y)/2)));
        }

        inline void Activate() {
            _isWorking = true;
            cancelButton->Enable();
            Center();
            gauge->SetValue(0);
            start = std::chrono::high_resolution_clock::now();
            this->Show();
            
            
            UpdateDisplay();
        }
        inline void Deactivate() {
            _isWorking = false;
            gauge->SetValue(0);
            ClearMessage();
            this->Hide();
        }

        inline bool IsWorking() {
            return _isWorking;
        }
        inline bool IsAsleep() {
            return !_isWorking;
        }

        void OnCancelButton(wxCommandEvent &evt);

        wxDECLARE_EVENT_TABLE();

};