/*
    MakeConsoleLink 
        A small program to help creating a shortcut (*.lnk) file
        including console properties
*/

#include "stdafx.h"
#include "MakeConsoleLink.h"

int __cdecl _tmain(int argc, TCHAR **argv)
{
    if (!ParseCommandLine(argc, argv))
    {
        _tprintf(TEXT("\nInvalid command line!\n"));
    }
    CCoInitialize init;
    CComPtr<IShellLink> spsl;
    spsl.CoCreateInstance(CLSID_ShellLink);

    spsl->SetPath(commandLine);
    spsl->SetArguments(szCommandArguments); 

    NT_CONSOLE_PROPS props;
    ZeroMemory(&props, sizeof(NT_CONSOLE_PROPS));
    SetConsoleProperties(&props);

    CComQIPtr<IShellLinkDataList>(spsl)->AddDataBlock(&props);

    CComQIPtr<IPersistFile>(spsl)->Save(szShortcutPath, TRUE); 
    return 0;
}

bool SetConsoleProperties(NT_CONSOLE_PROPS *props)
{
    props->dbh.cbSize = sizeof(NT_CONSOLE_PROPS);
    props->dbh.dwSignature = NT_CONSOLE_PROPS_SIG;

    /*
    The *Fill* settings below creates a bright green font on a blue background.
    This is hard coded, as I don't yet have a great way to incoroporate this into
    the command line parameters.
    */
    props->wFillAttribute = (FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
    props->wPopupFillAttribute = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_RED;

    props->dwWindowSize.X = windowSizeX;
    props->dwWindowSize.Y = windowSizeY;
    props->dwScreenBufferSize.X = screenBufferSizeX;
    props->dwScreenBufferSize.Y = screenBufferSizeY;
    props->uCursorSize = cursorSize;
    props->bQuickEdit = quickEdit;
    props->bInsertMode = insertMode;
    props->bAutoPosition = autoPosition;
    props->uHistoryBufferSize = historyBufferSize;
    props->uNumberOfHistoryBuffers = numberOfHistoryBuffers;

    /*
        Straight copy of the default colors for a console window
        if we don't fill these out, black is the only color choice 
        if anyone wants to modify the fill colors :}
    */
    props->ColorTable[0] = RGB(0x00, 0x00, 0x00);
    props->ColorTable[1] = RGB(0x00, 0x00, 0x80);
    props->ColorTable[2] = RGB(0x00, 0x80, 0x00);
    props->ColorTable[3] = RGB(0x00, 0x80, 0x80);
    props->ColorTable[4] = RGB(0x80, 0x00, 0x00);
    props->ColorTable[5] = RGB(0x80, 0x00, 0x80);
    props->ColorTable[6] = RGB(0x80, 0x80, 0x00);
    props->ColorTable[7] = RGB(0xC0, 0xC0, 0xC0);
    props->ColorTable[8] = RGB(0x80, 0x80, 0x80);
    props->ColorTable[9] = RGB(0x00, 0x00, 0xFF);
    props->ColorTable[10] = RGB(0x00, 0xFF, 0x00);
    props->ColorTable[11] = RGB(0x00, 0xFF, 0xFF);
    props->ColorTable[12] = RGB(0xFF, 0x00, 0x00);
    props->ColorTable[13] = RGB(0xFF, 0x00, 0xFF);
    props->ColorTable[14] = RGB(0xFF, 0xFF, 0x00);
    props->ColorTable[15] = RGB(0xFF, 0xFF, 0xFF);

    return true;
}

bool ParseCommandLine(int argc, TCHAR* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (_wcsicmp(argv[i], L"-commandLine") == 0)
        {
            if (++i < argc)
            {
                StringCchCopy(commandLine, MAX_PATH, argv[i]);
                if (commandLine[0] == 0)
                {
                    _tprintf(TEXT("ERROR: invalid commandLine parameter: %s\n"), argv[i]);
                    return false;
                }
            }
        }
        else if (_wcsicmp(argv[i], L"-commandArgs") == 0)
        {
            if (++i < argc)
            {
                StringCchCopy(szCommandArguments, MAX_PATH, argv[i]);
                if (commandLine[0] == 0)
                {
                    _tprintf(TEXT("ERROR: invalid szCommandArguments parameter: %s\n"), argv[i]);
                    return false;
                }
            }
        }
        else if (_wcsicmp(argv[i], L"-shortcutPath") == 0)
        {
            if (++i < argc)
            {
                StringCchCopy(szShortcutPath, MAX_PATH, argv[i]);
                if (commandLine[0] == 0)
                {
                    _tprintf(TEXT("ERROR: invalid shortcutPath parameter: %s\n"), argv[i]);
                    return false;
                }
            }
        }
        else if (_wcsicmp(argv[i], L"-quickedit") == 0)
        {
            if (++i < argc)
            {
                quickEdit = (BOOL)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-insertMode") == 0)
        {
            if (++i < argc)
            {
                insertMode = (BOOL)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-windowSizeX") == 0)
        {
            if (++i < argc)
            {
                windowSizeX = (SHORT)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-windowSizeY") == 0)
        {
            if (++i < argc)
            {
                windowSizeY = (SHORT)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-windowBufferX") == 0)
        {
            if (++i < argc)
            {
                screenBufferSizeX = (SHORT)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-windowBufferY") == 0)
        {
            if (++i < argc)
            {
                screenBufferSizeY = (SHORT)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-bufferSize") == 0)
        {
            if (++i < argc)
            {
                historyBufferSize = (USHORT)_wtoi(argv[i]);
            }
        }
        else if (_wcsicmp(argv[i], L"-historyBufferSize") == 0)
        {
            if (++i < argc)
            {
                numberOfHistoryBuffers = (USHORT)_wtoi(argv[i]);
            }
        }
        else if ((_wcsicmp(argv[i], L"-help") == 0) || (_wcsicmp(argv[i], L"-?") == 0))
        {
            ShowHelp();
        }
        else
        {
            //show help anyway if we can't figure out the command line
            ShowHelp();

            //but let the caller know that we failed too
            return false;
        }
    }
    return true;
}

void ShowHelp()
{
    _tprintf(TEXT("MakeConsoleLink creates a shortcut with specified console properties\n\n"));
    _tprintf(TEXT("Command line options:\n"));
    _tprintf(TEXT("    -commandLine\tName of shortcut executable (required)\n"));
    _tprintf(TEXT("    -commandArgs\tArguments for command line (required)\n"));
    _tprintf(TEXT("    -shortcutPath\tPath to shortcut to create (required)\n"));
    _tprintf(TEXT("    -quickedit\t\tTurn on quickedit mode (0|1) - Default: 1 (on)\n"));
    _tprintf(TEXT("    -insertMode\t\tTurn on insert mode (0|1) - Default: 1 (on)\n"));
    _tprintf(TEXT("    -windowSizeX\tWindow Width - Default: 132\n"));
    _tprintf(TEXT("    -windowSizeY\tWindow Height - Default:50\n"));
    _tprintf(TEXT("    -windowBufferX\tSize of width buffer - Default:132\n"));
    _tprintf(TEXT("    -windowBufferY\tSize of height buffer - Default: 1000\n"));
    _tprintf(TEXT("    -bufferSize\t\tSize of command buffer - Default: 25\n"));
    _tprintf(TEXT("    -historyBufferSize\tNumber of history buffers - Default: 4\n"));
    _tprintf(TEXT("\n"));

}