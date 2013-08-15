#include "stdafx.h"
const TCHAR* ChangeCurDir2ModulePath(HINSTANCE hInstance)
{
	static BOOL modulePathGetted=FALSE;
	static TCHAR moduleFileName[MAX_PATH]={0};
	if (!modulePathGetted)
	{
		ATLASSERT(hInstance);
		GetModuleFileName(hInstance,moduleFileName,MAX_PATH);
		int i;
		for (i=MAX_PATH-1;i>=0 && moduleFileName[i]!='\\';--i);
		moduleFileName[i]='\0';

		modulePathGetted=TRUE;
	}

	_tchdir(moduleFileName);
	return moduleFileName;
}


bool ChangeCurDir2PlaylistPath(bool bCreate)
{
	static TCHAR PlayListPath[MAX_PATH]={0};
	const TCHAR *modulePath=ChangeCurDir2ModulePath(NULL);

	if(PlayListPath[0]=='\0')
	{
		_tcscpy(PlayListPath,modulePath);
		_tcscat(PlayListPath,_T("\\playlists"));
	}

	if(_tchdir(PlayListPath)==0)
		return true;
	else
	{
		if(bCreate)
			//if the path not exist, create a new directory.
			_wmkdir(_T("playlists"));
		return _tchdir(PlayListPath)==0;
	}
}