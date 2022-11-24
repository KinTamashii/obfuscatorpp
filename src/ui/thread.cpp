#include "ui.hpp"


wxDEFINE_EVENT(wxEVT_COMMAND_MTHREAD_COMPLETED, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_VALUE, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_RANGE, wxThreadEvent);
wxDEFINE_EVENT(wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE, wxThreadEvent);


#include <chrono>
mThread::~mThread()
{
    wxCriticalSectionLocker enter(m_pHandler->m_pThreadCS);
    m_pHandler->m_pThread = nullptr; // Set the destroyed thread to null.
}


std::string mThread::DoTransform(const std::string &str) {
    return m_pHandler->Transform(m_pHandler, str, m_pHandler->numObfuscationsSpinBox->GetValue(), m_pHandler->languageChoice->GetSelection()+1);
}



wxThread::ExitCode ioBoxTransferThread::Entry() {
	m_pHandler->outputTextCtrl->SetValue(DoTransform((std::string)m_pHandler->inputTextCtrl->GetValue()));
	wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}


void fileThread::DoTransformFile(const std::string &path) {
    m_pHandler->SendEvent(wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE, path);
    file::write(
        DoTransform(file::read(path)),
        file::path::basename(path)+"_result.txt"
    );
}

wxThread::ExitCode fileTransformThread::Entry() {
	for (const auto &path : paths)
		if (file::path::extension((std::string)path) == ".txt") {
            DoTransformFile((std::string)path);
        }
			
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

wxThread::ExitCode folderTransformThread::Entry() {
	for (const auto &path : paths)
		for (const file::fs::directory_entry &entry : file::fs::recursive_directory_iterator((std::string)path))
			if (entry.path().extension() == ".txt") {
                DoTransformFile(entry.path());
            }
				
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}



template <typename ...T>
    requires (
        variadic::compare<T...>::template same<const std::string &, file::fs::path>() ||
        variadic::compare<T...>::template same<const std::string &>()
    )
    void generateThread::DoGenerate(T... args) {
        constexpr bool args2 = sizeof...(args) == 2;
        std::string cur;
        if constexpr (args2) {
            cur = variadic::get<0>(args...);
        } else {
            cur = file::read(variadic::get<0>(args...));
        }
        csv::generator gen(cur);

        for (int i = 0; i < numGenerations && m_pHandler->IsWorking(); i++) {
            // Update the progress gauge.
            if constexpr (args2)
                m_pHandler->SendEvent(
                    wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE,
                    fmt::format("{}/{}", i, numGenerations)
                );
            else
                m_pHandler->SendEvent(
                    wxEVT_COMMAND_MTHREAD_SET_GAUGE_MESSAGE,
                    fmt::format("{} – {}/{}", variadic::get<0>(args...), i, numGenerations)
                );
            gen.add((cur = DoTransform(cur)));
        }
        
        if constexpr (args2)
            file::write(gen.get(), (std::string)variadic::get<1>(args...));
        else
            file::write(gen.get(), file::path::basename(variadic::get<0>(args...))+"_result.csv");
    }

wxThread::ExitCode ioBoxGenerateThread::Entry() {
    DoGenerate((std::string)m_pHandler->inputTextCtrl->GetValue(), path);
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}





wxThread::ExitCode fileGenerateThread::Entry() {
	for (const auto &path : paths)
		if (file::path::extension((std::string)path) == ".txt")
            DoGenerate(
                (std::string)path
            );
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}

wxThread::ExitCode folderGenerateThread::Entry() {
	for (const auto &path : paths)
		for (const file::fs::directory_entry &entry : file::fs::recursive_directory_iterator((std::string)path))
			if (entry.path().extension() == ".txt") {
                DoGenerate(
                    (std::string)path
                );
            }
				
    wxQueueEvent(m_pHandler, new wxThreadEvent(wxEVT_COMMAND_MTHREAD_COMPLETED));
    return (wxThread::ExitCode)0;
}




void mFrame::OnThreadCompletion(wxCommandEvent& evt) {
    wxMessageOutputDebug().Printf("MFRAME: mThread exited!\n");
    progressGauge->Deactivate();
    Layout();
    EnableTransform();
}
void mFrame::OnThreadSetGaugeValue(wxCommandEvent& evt) {   
    progressGauge->SetValue(evt.GetInt());
    wxMessageOutputDebug().Printf("MFRAME: %d OnThreadSetGaugeValue..\n", progressGauge->GetValue());
    Unlock();
}


void mFrame::OnThreadSetGaugeRange(wxCommandEvent &evt) {
    progressGauge->SetRange(evt.GetInt());
    wxMessageOutputDebug().Printf("MFRAME: %d OnThreadSetGaugeRange...\n", progressGauge->GetRange());
    Unlock();
}

void mFrame::OnThreadSetGaugeMessage(wxCommandEvent &evt) {
    progressGauge->SetMessage((std::string)evt.GetString());
    wxMessageOutputDebug().Printf("MFRAME: OnThreadSetGaugeMessage...\n");
    Unlock();
}

void mFrame::DoPauseThread() {
    wxCriticalSectionLocker enter(m_pThreadCS);
    if (m_pThread) {
        if (m_pThread->Pause() != wxTHREAD_NO_ERROR )
            wxLogError("Can't pause the thread!");
    }
}