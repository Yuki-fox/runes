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

	// 找所有Process
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

				// 取得名稱
				GetModuleBaseNameW( hProcess ,hMod, szProcessName, sizeof(szProcessName));

				// 取得檔案位置
				GetModuleFileNameExW( hProcess, hMod, szProcessPath, sizeof(szProcessPath));

				// 比較路徑
				if( _wcsicmp( szProcessPath, pszProcessName )== 0 )
					bFind = true;
			}
			CloseHandle( hProcess );
		}

		// 如果有找到該路徑 就關閉Process
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
	// 取得目前目錄的Laucher 路徑
	wchar_t szNewLauncherPath[ 1024 ];
	wchar_t szOldLauncherPath[ 1024 ];
	wchar_t szDir[ 1024 ];
	GetModuleFileNameW( NULL, szDir, sizeof( szDir ) );

	wchar_t* str = wcsrchr( szDir, L'\\' );

	if( str ) *( str + 1 ) = L'\0';

	swprintf_s( szOldLauncherPath, 1024, L"%slauncher.exe", szDir );
	swprintf_s( szNewLauncherPath, 1024, L"%s_launcher.exe", szDir );

	//::MessageBoxW( NULL, szOldLauncherPath, szNewLauncherPath , NULL );

	// 等待Launcher 自己結束自己的時間
	Sleep( 500 );

	// 結束Launcher.exe Process
	KillProcByPath( szOldLauncherPath );

	// 結束_Launcher.exe Process
	KillProcByPath( szNewLauncherPath );

	// KillProc 等待時間
	Sleep( 1000 );

	wchar_t szLauncherFn[ 128 ]		= L"launcher.exe";
	wchar_t szNewLauncher[ 128 ]	= L"_launcher.exe";
	wchar_t szBakLauncher[ 128 ]	= L"launcher_old.exe";

	//::MessageBoxW( NULL, szLauncherFn, szNewLauncher , NULL );

	// 假如檔案都存在
	if( _waccess( szLauncherFn, 0 ) != -1  && _waccess( szNewLauncher, 0 ) != -1  )
	{
		//::MessageBoxW( NULL, szLauncherFn, szNewLauncher , NULL );
		// 刪除舊的Launcher_old.exe
		SetFileAttributesW(szBakLauncher, FILE_ATTRIBUTE_NORMAL );
		DeleteFileW( szBakLauncher );

		// 將Launcher 更新交換

		// launcher.exe 改名為 launcher_old.exe
		if( MoveFileW( szLauncherFn,  szBakLauncher ) == FALSE )
			return;

		// _launcher.exe 改名為 launcher.exe		
		if( MoveFileW( szNewLauncher,  szLauncherFn ) == FALSE )
		{
			// 如果失敗, 則將 launcher_old.exe 改名回 launcher.exe 
			MoveFileW( szBakLauncher,  szLauncherFn );
			return;
		}
		DeleteFileW( szBakLauncher );
	}
}

void LauncherOpera::StartLauncher()
{
	// 啟動Launcher
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
