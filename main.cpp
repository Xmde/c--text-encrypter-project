//code by django

#include "wx/wxprec.h"


#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wx/dataobj.h"
#include "wx/clipbrd.h"


#include "wx/file.h"

#if !wxUSE_DRAG_AND_DROP
#undef wxUSE_CLIPBOARD
#define wxUSE_CLIPBOARD 0
#endif

#include "wx/colordlg.h"
#include "wx/fontdlg.h"
#include "wx/numdlg.h"
#include "wx/tokenzr.h"

#include "encrypt.h"

class MyApp : public wxApp
{
public:
	bool OnInit() wxOVERRIDE;
};

// makes actions for ritch text box
class MyTextCtrl : public wxTextCtrl
{
public:
	MyTextCtrl(wxWindow* parent, wxWindowID id, const wxString& value,
		const wxPoint& pos, const wxSize& size, int style = 0)
		: wxTextCtrl(parent, id, value, pos, size, style)
	{
		m_hasCapture = false;
	}

	void OnTextCut(wxClipboardTextEvent& event);
	void OnTextCopy(wxClipboardTextEvent& event);
	void OnTextPaste(wxClipboardTextEvent& event);

private:
	static inline wxChar GetChar(bool on, wxChar c) { return on ? c : '-'; }

	void LogClipEvent(const wxString& what, wxClipboardTextEvent& event);

	bool m_hasCapture;

	wxDECLARE_EVENT_TABLE();
};

//text input container
class MyPanel : public wxPanel{
	public:
		MyPanel(wxFrame* frame, int x, int y, int w, int h);

		// return true if currently text control has any selection
		bool HasSelection() const
		{
			long from, to;
			GetFocusedText()->GetSelection(&from, &to);
			return from != to;
		}

		MyTextCtrl* m_textrich;

		wxTextCtrl* m_log;

		wxButton* Encrypt;

		wxButton* Decrypt;

	private:
		wxTextCtrl* GetFocusedText() const;
};

//window
class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, int x, int y);

	void OnQuit(wxCommandEvent& event);
	void OnFileSave(wxCommandEvent& event);
	void OnFileLoad(wxCommandEvent& event);

	void DoEncrypt(wxCommandEvent& event);
	void DoDecrypt(wxCommandEvent& event);

	void OnIdle(wxIdleEvent& event);

private:
	MyPanel* m_panel;

	wxDECLARE_EVENT_TABLE();
};

wxIMPLEMENT_APP(MyApp);

enum
{
	TEXT_QUIT = wxID_EXIT,
	TEXT_LOAD = 101,
	TEXT_SAVE,
	BUTTON_Encrypt,
	BUTTON_Decrypt,

};

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	// Create the main frame window
	MyFrame* frame = new MyFrame("encrypter project", 50, 50);

	wxMenu* file_menu = new wxMenu;
	file_menu->Append(TEXT_SAVE, "&Save file\tCtrl-S",
		"Save the contense to a file");
	file_menu->Append(TEXT_LOAD, "&Load file\tCtrl-O",
		"Load a file to prosses");
	file_menu->Append(TEXT_QUIT, "E&xit\tAlt-X", "Quit");

	wxMenuBar* menu_bar = new wxMenuBar(wxMB_DOCKABLE);
	menu_bar->Append(file_menu, "&File");

	frame->SetMenuBar(menu_bar);

	frame->Show(true);

	// report success
	return true;
}

//actions for text box
wxBEGIN_EVENT_TABLE(MyTextCtrl, wxTextCtrl)

EVT_TEXT_CUT(wxID_ANY, MyTextCtrl::OnTextCut)
EVT_TEXT_COPY(wxID_ANY, MyTextCtrl::OnTextCopy)
EVT_TEXT_PASTE(wxID_ANY, MyTextCtrl::OnTextPaste)

wxEND_EVENT_TABLE()

void MyTextCtrl::OnTextCut(wxClipboardTextEvent& event)
{
	LogClipEvent("cut to", event);
}

void MyTextCtrl::OnTextCopy(wxClipboardTextEvent& event)
{
	LogClipEvent("copied to", event);
}

void MyTextCtrl::OnTextPaste(wxClipboardTextEvent& event)
{
	LogClipEvent("pasted from", event);
}

void MyTextCtrl::LogClipEvent(const wxString& what, wxClipboardTextEvent& event)
{
	wxFrame* frame = wxDynamicCast(wxGetTopLevelParent(this), wxFrame);
	wxCHECK_RET(frame, "no parent frame?");

	event.Skip();

	wxLogMessage("Text %s%s the clipboard.","", what);
}


//window where the things are
MyPanel::MyPanel(wxFrame* frame, int x, int y, int w, int h)
	: wxPanel(frame, wxID_ANY, wxPoint(x, y), wxSize(w, h))
{
	m_log = new wxTextCtrl(this, wxID_ANY, "This is the log window.\n",
		wxPoint(5, 260), wxSize(630, 100),
		wxTE_MULTILINE | wxTE_READONLY);

	wxLog::SetActiveTarget(new wxLogTextCtrl(m_log));

	m_textrich = new MyTextCtrl(this, wxID_ANY, "", wxPoint(450, 10), wxSize(200, 230), wxTE_RICH | wxTE_MULTILINE );
	m_textrich->SetHint("add text to prosses");

	Encrypt = new wxButton(this, BUTTON_Encrypt, _T("Encrypt text"), wxDefaultPosition, wxSize(100, 50), 0);

	Decrypt = new wxButton(this, BUTTON_Decrypt, _T("Decrypt text"), wxDefaultPosition, wxSize(100, 50), 0);

	wxBoxSizer* row1 = new wxBoxSizer(wxHORIZONTAL);
	row1->Add(m_textrich, 1, wxALL | wxEXPAND, 10);

	wxBoxSizer* row2 = new wxBoxSizer(wxHORIZONTAL);
	row2->Add(Encrypt, 1, wxALL | wxEXPAND, 10);
	row2->Add(Decrypt, 1, wxALL | wxEXPAND, 10);

	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	topSizer->Add(row1, 2, wxALL | wxEXPAND, 10);
	topSizer->Add(row2, 0, wxALL | wxEXPAND, 10);

	topSizer->Add(m_log, 1, wxALL | wxEXPAND, 10);


	SetSizer(topSizer);
}

//setting actions to do for buttons
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(TEXT_QUIT, MyFrame::OnQuit)
EVT_MENU(TEXT_SAVE, MyFrame::OnFileSave)
EVT_MENU(TEXT_LOAD, MyFrame::OnFileLoad)

//button actions
EVT_BUTTON(BUTTON_Encrypt, DoEncrypt)
EVT_BUTTON(BUTTON_Decrypt, DoDecrypt)

EVT_IDLE(MyFrame::OnIdle)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, int x, int y)
	: wxFrame(NULL, wxID_ANY, title, wxPoint(x, y))
{
	SetIcon(wxICON(sample));

#if wxUSE_STATUSBAR
	CreateStatusBar(2);
#endif // wxUSE_STATUSBAR

	m_panel = new MyPanel(this, 10, 10, 300, 100);
	m_panel->GetSizer()->Fit(this);
}

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event)){
	Close(true);
}


void MyFrame::OnFileSave(wxCommandEvent& WXUNUSED(event)){
	if (m_panel->m_textrich->SaveFile("output.txt"))
	{
		// verify that the file length is correct
		wxFile file("dummy.txt");
		wxLogStatus(this,
			"Successfully saved file (text len = %lu, file size = %ld)",
			(unsigned long)m_panel->m_textrich->GetValue().length(),
			(long)file.Length());
	}
	else
		wxLogStatus(this, "Couldn't save the file");
}

void MyFrame::OnFileLoad(wxCommandEvent& WXUNUSED(event)){
	if (m_panel->m_textrich->LoadFile("input.txt"))
	{
		wxLogStatus(this, "Successfully loaded file");
	}
	else
	{
		wxLogStatus(this, "Couldn't load the file");
	}
}


void MyFrame::OnIdle(wxIdleEvent& event){
		// track the window which has the focus in the status bar
		static wxWindow* s_windowFocus = (wxWindow*)NULL;
		wxWindow* focus = wxWindow::FindFocus();
		if (focus && (focus != s_windowFocus))
		{
			s_windowFocus = focus;

			wxString msg;
			msg.Printf(
	#ifdef __WXMSW__
				"Focus: wxWindow = %p, HWND = %p",
	#else
				"Focus: wxWindow = %p",
	#endif
				s_windowFocus
	#ifdef __WXMSW__
				, s_windowFocus->GetHWND()
	#endif
			);

	#if wxUSE_STATUSBAR
			SetStatusText(msg);
	#endif // wxUSE_STATUSBAR
		}
		event.Skip();
}

void MyFrame::DoEncrypt(wxCommandEvent& event) {
	wxLogMessage("Begning Encryption");
	std::string inputText = m_panel->m_textrich->GetValue().ToStdString();
	m_panel->m_textrich->Clear();
	m_panel->m_textrich->AppendText(enc::encrypt(inputText));
	wxLogMessage("Finished Encryption");

}

void MyFrame::DoDecrypt(wxCommandEvent& event) {
	wxLogMessage("Begning Decryption");
	std::string inputText = m_panel->m_textrich->GetValue().ToStdString();
	m_panel->m_textrich->Clear();
	m_panel->m_textrich->AppendText(enc::decrypt(inputText));
	wxLogMessage("Finished Decryption");
}

