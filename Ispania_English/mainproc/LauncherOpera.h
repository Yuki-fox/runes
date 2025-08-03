

class LauncherOpera
{

public:

	LauncherOpera();
	virtual ~LauncherOpera();

	void KillProcByPath( const wchar_t* pszProcessName );
	void UpdateLauncher();
	void StartLauncher();

};