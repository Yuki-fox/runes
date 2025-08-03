#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <io.h>
#include "psapi.h"
#include "LauncherOpera.h"

LauncherOpera::LauncherOpera()
{

}

LauncherOpera::~LauncherOpera()
{

}

void LauncherOpera::KillProcByPath( const wchar_t* pszProcessName )
{
	DWORD dwProcs[ 2048 ];
	DWORD dwNeeded;

	HMODULE hMod;
	DWORD cbNeeded;

	// ��Ҧ�Process
	EnumProcesses( dwProcs,  sizeof( dwProcs ), &dwNeeded );

	DWORD dwProcCount = dwNeeded / sizeof(DWORD);

	int idx = 0;
	for( DWORD i = 0; i < dwProcCount; i++ )
	{
		bool bFind = false;

		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcs[ i ] );

		if( hProcess )
		{
			if( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded ) )
			{
				wchar_t szProcessName[ MAX_PATH ] = L"unknow";
				wchar_t szProcessPath[ MAX_PATH ] = L"unknow";

				// ���o�W��
				GetModuleBaseNameW( hProcess ,hMod, szProcessName, sizeof(szProcessName));

				// ���o�ɮצ�m
				GetModuleFileNameExW( hProcess, hMod, szProcessPath, sizeof(szProcessPath));

				// ������|
				if( _wcsicmp( szProcessPath, pszProcessName )== 0 )
					bFind = true;
			}
			CloseHandle( hProcess );
		}

		// �p�G�����Ӹ��| �N����Process
		if( bFind )
		{
			HANDLE hProcess = OpenProcess( PROCESS_TERMINATE, FALSE, dwProcs[ i ] );

			if( hProcess )
			{
				TerminateProcess( hProcess, 0 );
				CloseHandle( hProcess );
			}
		}
	}
}

void LauncherOpera::UpdateLauncher()
{
	// ���o�ثe�ؿ���Laucher ���|
	wchar_t szNewLauncherPath[ 1024 ];
	wchar_t szOldLauncherPath[ 1024 ];
	wchar_t szDir[ 1024 ];
	GetModuleFileNameW( NULL, szDir, sizeof( szDir ) );

	wchar_t* str = wcsrchr( szDir, L'\\' );

	if( str ) *( str + 1 ) = L'\0';

	swprintf_s( szOldLauncherPath, 1024, L"%slauncher.exe", szDir );
	swprintf_s( szNewLauncherPath, 1024, L"%s_launcher.exe", szDir );

	//::MessageBoxW( NULL, szOldLauncherPath, szNewLauncherPath , NULL );

	// ����Launcher �ۤv�����ۤv���ɶ�
	Sleep( 500 );

	// ����Launcher.exe Process
	KillProcByPath( szOldLauncherPath );

	// ����_Launcher.exe Process
	KillProcByPath( szNewLauncherPath );

	// KillProc ���ݮɶ�
	Sleep( 1000 );

	wchar_t szLauncherFn[ 128 ]		= L"launcher.exe";
	wchar_t szNewLauncher[ 128 ]	= L"_launcher.exe";
	wchar_t szBakLauncher[ 128 ]	= L"launcher_old.exe";

	//::MessageBoxW( NULL, szLauncherFn, szNewLauncher , NULL );

	// ���p�ɮ׳��s�b
	if( _waccess( szLauncherFn, 0 ) != -1  && _waccess( szNewLauncher, 0 ) != -1  )
	{
		//::MessageBoxW( NULL, szLauncherFn, szNewLauncher , NULL );
		// �R���ª�Launcher_old.exe
		SetFileAttributesW(szBakLauncher, FILE_ATTRIBUTE_NORMAL );
		DeleteFileW( szBakLauncher );

		// �NLauncher ��s�洫

		// launcher.exe ��W�� launcher_old.exe
		if( MoveFileW( szLauncherFn,  szBakLauncher ) == FALSE )
			return;

		// _launcher.exe ��W�� launcher.exe		
		if( MoveFileW( szNewLauncher,  szLauncherFn ) == FALSE )
		{
			// �p�G����, �h�N launcher_old.exe ��W�^ launcher.exe 
			MoveFileW( szBakLauncher,  szLauncherFn );
			return;
		}
		DeleteFileW( szBakLauncher );
	}
}

void LauncherOpera::StartLauncher()
{
	// �Ұ�Launcher
	SHELLEXECUTEINFOA ShExecInfo = {0};
	ShExecInfo.cbSize		= sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask		= SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd			= NULL;
	ShExecInfo.lpVerb		= "open";
	ShExecInfo.lpFile		= "launcher.exe";
	ShExecInfo.lpParameters = 0;	
	ShExecInfo.lpDirectory	= 0;
	ShExecInfo.nShow		= SW_SHOWNORMAL;
	ShExecInfo.hInstApp		= NULL;	
	ShellExecuteExA(&ShExecInfo);
}
