#include "StdAfx.h"
#include "PlayList.h"
//#include "MusicFile.h"
//#include "WaveFileEx.h"
//#include "Mp3File.h"
#include <direct.h>
//#include <id3/tag.h>



#include <stdlib.h>
#include <tbytevector.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <id3v1tag.h>
#include <apetag.h>
#include <taglib.h>

#include <fileref.h>
using namespace TagLib;



LPSTR Unicode2Ansi(LPCWSTR s)
{
	DWORD dwNum= WideCharToMultiByte (CP_ACP, 0, s, -1, NULL, 0,0,0);
	LPSTR target=new char[dwNum];
	WideCharToMultiByte(CP_ACP,0,s,-1,target,dwNum,0,0);
	return target;
}



LPWSTR Ansi2Unicode(LPSTR s)
{
	DWORD dwNum= MultiByteToWideChar (CP_ACP, 0,(LPCSTR) s, -1, NULL, 0);
	LPWSTR target=new WCHAR[dwNum];
	MultiByteToWideChar(CP_ACP,0,(LPCSTR)s,-1,target,dwNum);
	return target;
}

LPWSTR UTF82Unicode(LPSTR s)
{
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, s, -1, NULL, 0);
	LPWSTR target=new WCHAR[dwNum];
	MultiByteToWideChar(CP_UTF8,0,(LPCSTR)s,-1,target,dwNum);
	return target;
}


PlayList& MyLib::GetPlayListObj()
{
	static PlayList playList_;
	return playList_;
}



PlayList::PlayList(void)
{
}

PlayList::~PlayList(void)
{
}

void PlayList::AddFolderToCurrentPlayList(LPCTSTR pszFolder)
{
	MyLib::GetPlayListObj().AddFolder(pszFolder);
}

void PlayList::scanAllId3Info()
{

}

BOOL PlayList::AddFolder(LPCTSTR pszFolder)
{
	// for each mp3 file in this folder
	// data . add

	//忽略了子目录下的mp3文件

	//改变当前目录
	_tchdir(pszFolder);

	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	hFind=::FindFirstFile(_T("*.mp3"),&findFileData);

	if(hFind!=INVALID_HANDLE_VALUE)
	{
		if (1)//(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			TCHAR *_findName=new TCHAR[MAX_PATH];
			_tcscpy(_findName,findFileData.cFileName);
			TCHAR *pathName=new TCHAR[MAX_PATH+1];
			_tcscpy(pathName,pszFolder);
			_tcscat(pathName,_T("\\"));


			_tcscat(pathName,_findName);
			std::tstring str(pathName);
			//m_songList.push_back(str);

			PlayListItem item(&str);
			item.scanId3Info();
			m_songList.push_back(item);
		}

		BOOL ret;
		while(ret=FindNextFile(hFind,&findFileData))
		{
			if (1)//(!(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
			{
				TCHAR *_findName=new TCHAR[MAX_PATH];
				_tcscpy(_findName,findFileData.cFileName);
				TCHAR *pathName=new TCHAR[MAX_PATH+1];
				_tcscpy(pathName,pszFolder);
				_tcscat(pathName,_T("\\"));
				_tcscat(pathName,_findName);
				std::tstring str(pathName);
				//m_songList.push_back(str);

				PlayListItem item(&str);
				item.scanId3Info();
				m_songList.push_back(item);
				


			}
		}
	}
	FindClose(hFind);

	return TRUE;
}



BOOL PlayListItem::scanId3Info()
{
	BOOL bInvalidID3V2=FALSE;

	MPEG::File f(url.c_str());

	ID3v2::Tag *id3v2tag = f.ID3v2Tag();
	if(id3v2tag) 
	{
		title=id3v2tag->title().toWString();
		artist=id3v2tag->artist().toWString();
		album=id3v2tag->album().toWString();
		genre=id3v2tag->genre().toWString();

		year=id3v2tag->year();

		if ( title.empty() && artist.empty() && album.empty())
			bInvalidID3V2=TRUE;
	}
	else
		bInvalidID3V2=TRUE;
	

	if(bInvalidID3V2)
	{
		ID3v1::Tag *id3v1tag = f.ID3v1Tag();
		if(id3v1tag) 
		{
			title=id3v1tag->title().toWString();
			artist=id3v1tag->artist().toWString();
			album=id3v1tag->album().toWString();
			genre=id3v1tag->genre().toWString();

			year=id3v1tag->year();
		}
	}

	return TRUE;
}