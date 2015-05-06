

bool ParseCommandLine(int argc, TCHAR* argv[]);
bool SetConsoleProperties(NT_CONSOLE_PROPS *props);
void ShowHelp();

// Defaults (i.e. window properties we'll set if no command line parameters are not used)
TCHAR commandLine[MAX_PATH];
TCHAR szCommandArguments[MAX_PATH];
TCHAR szShortcutPath[MAX_PATH];
SHORT windowSizeX = 132; // 132 columns wide
SHORT windowSizeY = 50; // 50 lines tall
SHORT screenBufferSizeX = 132; // 132 columns wide
SHORT screenBufferSizeY = 1000; // large scrollback
USHORT cursorSize = 25; // small cursor
BOOL quickEdit = TRUE; // turn QuickEdit on
BOOL autoPosition = TRUE; // turn Auto Position on
BOOL insertMode = TRUE;
USHORT historyBufferSize = 25;
USHORT numberOfHistoryBuffers = 4;

// simple wrapper around CoInitialize
class CCoInitialize {
public:
	CCoInitialize() : m_hr(CoInitialize(NULL)) { }
	~CCoInitialize() { if (SUCCEEDED(m_hr)) CoUninitialize(); }
	operator HRESULT() const { return m_hr; }
	HRESULT m_hr;
};
