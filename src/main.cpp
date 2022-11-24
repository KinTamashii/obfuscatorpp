#include <iostream>
#include <array>
#include <vector>


#include <google.hpp>
#include <fstream>
#define FMT_HEADER_ONLY
#include <fmt/format.h>





#include <util/arrayToInt.hpp>
#include <batch/batch.hpp>
#include <batch/split.hpp>
#include <batch/parse.hpp>
#include <batch/encode.hpp>
#include <listReader.hpp>
#include <util/timer.hpp>

#include <batch/base122.hpp>
#include <util/arrayToInt.hpp>
#include <util/parse.hpp>

#include <batch/execute.hpp>

#include <ui/ui.hpp>
#include <wx/stdpaths.h>
#include <preferences.hpp>

class mApp : public wxApp {
    public:
        virtual bool OnInit();
};

bool mApp::OnInit() {
    pref.read((std::string)wxStandardPaths::Get().GetUserConfigDir());
    mFrame *frame = new mFrame((wxWindow*)this, wxID_ANY, "Text Obfuscator++ v.0.1");
    frame->SetTransform(&batch::obfuscate, "Obfuscate");
    frame->Show(true);
    return true;
};


wxIMPLEMENT_APP(mApp);

