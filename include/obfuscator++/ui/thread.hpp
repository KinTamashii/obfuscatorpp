#pragma once
#include <wx/wx.h>
#include <wx/thread.h>
#include <ui/ui.hpp>
#include <string>
#include <util/variadic.hpp>
#include <util/template.hpp>
#include <util/csv.hpp>

wxDECLARE_EVENT(wxEVT_COMMAND_MTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_RANGE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE, wxThreadEvent);
class mFrame;


/* Secondary thread used to handle heavy functions behind the ui. */
struct mThread : public wxThread {
    mThread(mFrame *handler) : wxThread(wxTHREAD_DETACHED) {
        m_pHandler = handler;
    }
    std::string DoTransform(const std::string &str); // Call the function using the state of the ui.
    ~mThread();
    protected:
        mFrame *m_pHandler;
        
};


/* Calls the function with the text in the input box, and puts the result in the output box. */
class ioBoxTransferThread : public mThread {
    public:
        ioBoxTransferThread(mFrame *handler)
            : mThread(handler) {}
    protected:
        virtual ExitCode Entry();
        std::string str;
};


/* Calls the function on selected files in a dialog. */
class fileThread : virtual public mThread {
    public:
        fileThread(mFrame *handler, auto *dlg) :
            mThread(handler) {
                dlg->GetPaths(paths);
            }
        void DoTransformFile(const std::string &path);
    protected:
        wxArrayString paths;
};

/* Calls the function on selected files in a dialog. */
class fileTransformThread : public fileThread {
    public:
        fileTransformThread(mFrame *handler, wxFileDialog *dlg)
            : fileThread(handler, dlg), mThread(handler) {}
    protected:
        virtual ExitCode Entry();
};

/* Calls the function on all files in selected folders in a dialog. */
class folderTransformThread : public fileThread {
    public:
        folderTransformThread(mFrame *handler, wxDirDialog *dlg)
            : fileThread(handler, dlg), mThread(handler) {}
    protected:
        virtual ExitCode Entry();
};

/* Calls the function multiple times, creating a comparison spreadsheet. */
class generateThread : virtual public mThread {
    public:
    generateThread(mFrame *handler, int numGenerations) : mThread(handler) {
        this->numGenerations = numGenerations;
    }
    
    
    template <typename ...T>
    requires (
        variadic::compare<T...>::template same<const std::string &, file::fs::path>() ||
        variadic::compare<T...>::template same<const std::string &>()
    )
    void DoGenerate(T... args);
    
    /* Generate a comparison spreadsheet from the text in the file in the provided path. */
    inline void DoGenerate(const std::string &str) {
        DoGenerate<const std::string&>(str);
    }

    /* Generate a comparison spreadsheet from an input string, saving it in the provided path. */
    inline void DoGenerate(const std::string &str, file::fs::path path) {
        DoGenerate<const std::string&, file::fs::path>(str, path);
    }



    void DoGenerateFile(const std::string &path);
    int numGenerations;
};

/* Calls the function multiple times using the text from the input box, creating a comparison spreadsheet.*/
class ioBoxGenerateThread : public generateThread {
    public:
        ioBoxGenerateThread(mFrame *handler, wxFileDialog *dlg, int numGenerations)
        : generateThread(handler, numGenerations), mThread(handler) {
            this->path = dlg->GetPath();
        }
    protected:
        virtual ExitCode Entry();
        std::string path;
};

/* Calls the function multiple times for selected files in a dialog, creating a comparison spreadsheet for each.*/
class fileGenerateThread : public fileThread, public generateThread {
    public:
        fileGenerateThread(mFrame *handler, wxFileDialog *dlg, int numGenerations)
            : fileThread(handler, dlg), generateThread(handler, numGenerations), mThread(handler) {
                
            }
    protected:
        virtual ExitCode Entry();
};


/* Calls the function multiple times for all files each selected folder in a dialog, creating a comparison spreadsheet for each.*/
class folderGenerateThread : public fileThread, public generateThread {
    public:
        folderGenerateThread(mFrame *handler, wxDirDialog *dlg, int numGenerations)
            : fileThread(handler, dlg), generateThread(handler, numGenerations), mThread(handler) {
            }
    protected:
        virtual ExitCode Entry();
};