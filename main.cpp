#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <shellapi.h>
#include <tlhelp32.h>

using namespace std;

LPCTSTR gEX;
LPCTSTR oID;
HANDLE GetHndl(LPCTSTR pName);
int Start(LPCTSTR pName, LPCTSTR pArgs);
bool IsRunning(LPCTSTR pName);
void Kill(LPCTSTR pName);

int main(int argc, char **argv)
{
    FreeConsole();
    //Return error if no parameter is specified
    if (argv[1]==NULL){
        MessageBox(0, "Specify which game to launch: \n\nBattlefield 1: Ultimate = BF1-u \nBattlefield 1: Deluxe = BF1-d \nBattlefield 1: Standard = BF1-s \n\nBattlefield 4: Premium = BF4-p \nBattlefield 4: Deluxe = BF4-d \nBattlefield 4: Standard = BF4-s", "Error", MB_OK);
        return 1;}
    //determine which battlefield to start
	if (_stricmp(argv[1], "BF1-U") == 0) {
        gEX = "bf1.exe";
        oID = "origin://launchgame/Origin.OFR.50.0001662";
        }
	else if (_stricmp(argv[1], "BF1-D") == 0) {
		gEX = "bf1.exe";
		oID = "origin://launchgame/Origin.OFR.50.0001385";
	}
	else if (_stricmp(argv[1], "BF1-S") == 0) {
		gEX = "bf1.exe";
		oID = "origin://launchgame/Origin.OFR.50.0000557";
	}
	else if (_stricmp(argv[1], "BF4-P") == 0) {
		gEX = "bf4.exe";
		oID = "origin://launchgame/OFB-EAST:109552316";
	}
	else if (_stricmp(argv[1], "BF4-D") == 0) {
		gEX = "bf4.exe";
		oID = "origin://launchgame/OFB-EAST:109549060";
	}
	else if (_stricmp(argv[1], "BF4-S") == 0) {
		gEX = "bf4.exe";
		oID = "origin://launchgame/OFB-EAST:109546867";
	}
    else {
        MessageBox(0, "Please specify a valid parameter: \n\nBattlefield 1: Ultimate = BF1-u \nBattlefield 1: Deluxe = BF1-d \nBattlefield 1: Standard = BF1-s \n\nBattlefield 4: Premium = BF4-p \nBattlefield 4: Deluxe = BF4-d \nBattlefield 4: Standard = BF4-s", "Error", MB_OK);
        return 1;
        }
    //determine origin directory
    LPCTSTR oDir;
    if (argv[2]!=NULL){
        oDir = argv[2];}
    else oDir = "C:/PROGRA~2/Origin/Origin.exe";
    //check if origin is running
    if (IsRunning("Origin.exe")){
        Kill("Origin.exe");}
    //start origin as child program
    if (Start(oDir,"-StartClientMinimized") <= 32){
        MessageBox(0, "Failed to launch Origin. \nMake sure it's installed in its default location.", "Error", MB_OK);
        return 1;}
    while (!IsRunning("Origin.exe")){
        Sleep(1000);}
    //wait for origin to go idle
    Sleep(8000);
    //launch game
    Start(oID, NULL);
    for (int i = 1; i<41; i++){
        Sleep (1000);}
    //exit if game does not start after 40s
    if (!IsRunning(gEX)){
        MessageBox(0, "Game doesn't appear to be running. Closing launcher.", "Error", MB_OK);
        Kill("Origin.exe");
        return 1;}
    //wait for game to close
    while (IsRunning(gEX)){
        Sleep(1000);}
    //give some time for cloud sync
    Sleep(8000);
    //Close Origin
    Kill("Origin.exe");
    return 0;
}

//--------------- F U N C T I O N S ---------------//

HANDLE GetHndl(LPCTSTR pName){
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry) == TRUE){
        while (Process32Next(snapshot, &entry) == TRUE){
            if (_stricmp(entry.szExeFile, pName ) == 0){
            HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
            CloseHandle(snapshot);
            return hProcess;}}
    }return 0;
}

int Start(LPCTSTR pName, LPCTSTR pArgs){
    HINSTANCE h = ShellExecute(NULL,"open",pName,pArgs,NULL,SW_SHOW);
    int r = (int) h;
    return r;
}

bool IsRunning(LPCTSTR pName){
    if (GetHndl(pName)==0)
        return 0;
    return 1;
}

void Kill(LPCTSTR pName){
    HANDLE hProcess = GetHndl(pName);
    TerminateProcess(hProcess, 0 );
}

