#include "PlayList.h"
#pragma once


class MyLib:
	public SerializeObj
{
	//-----------------------------------------
	//
public:
	vector<std::tstring> lrcDirs;//歌词搜索路径表
	vector<std::tstring> dataPaths;

	void ClearLrcSearchLib()
	{
		lrcDirs.clear();
	}

	void AddFolder2LrcSearchLib(LPCTSTR pszFolder)
	{
		lrcDirs.push_back(pszFolder);
	}

	static void ImportLycByPath(std::tstring path);
	void MyLib::InitLrcLib();

public:
	virtual int SerializeB(FILE* pFile);
	virtual int ReSerialize(FILE* pFile);

	//-----------------------------------------



public:
	MyLib():m_pActivePlaylist(NULL),m_pSelPlaylist(NULL)
		,playorder(Default)
	{
	};

	~MyLib();
	static MyLib* shared();
public:
	HWND hMain;
	static void SetMain(HWND hMain);
	static HWND GetMain();

public:
	//data member
	typedef list<PlayList*> PLList;
	PLList m_playLists;

	//自动播放列表,媒体库专用.
	PlayList* GetAutoPlaylist();
	vector<std::tstring> mediaPaths;
	void ClearMediaPath();
	void AddMediaPath(std::tstring &path);
	void DelMediaPath(std::tstring &path);

	//播放列队
	typedef list<_songContainerItem> PlayQueueContainer;
	PlayQueueContainer playQueue;
	void PushPlayQueue(_songContainerItem item);
	PlayListItem PopTrackFromPlayQueue();

	//to test track weather in the queue
	//use return value 's empty funtion
	vector<int> GetIndexInPlayQueue(_songContainerItem item);
 
	void DeleteFromPlayQueue(_songContainerItem item);
	void ClearPlayQueue();

private:
	//pointers
	PlayList*      m_pActivePlaylist;  //playing
	PlayList*      m_pSelPlaylist;//operating
public:
	_songContainerItem lastPlayingItem;
	_songContainerItem nextPlayingItem;
	_songContainerItem curSelectedItem;
private:
	_songContainerItem WaitPlayItem;
public:
	void SetWaitPlayItem(_songContainerItem item)
	{
		WaitPlayItem=item;
	}

	_songContainerItem *GetWaitPlayItem(){
		return &WaitPlayItem;}
public:

	void SetSelPlaylist(PlayList* p){m_pSelPlaylist=p;}
	PlayList* SelPlaylist(){return m_pSelPlaylist;}

	void SetActivePlaylist(PlayList* p){m_pActivePlaylist=p;}
	PlayList* ActivePlaylist(){return m_pActivePlaylist;}

	PlayList* NewPlaylist(std::tstring playlistname=_T("新建播放列表1"),bool bAutoPL=false);
	void DeletePlayList(PlayList *pl);

	
	_songContainerItem playingItem;
	_songContainerItem * GetPlayingItem(){return &playingItem;}
	void  SetPlayingItem(_songContainerItem item){playingItem=item;}
	bool isPlaying();

	static PlayList* AddFolderToCurrentPlayList(LPCTSTR pszFolder);
	void playAfterSlowDown(FileTrack * item);
	void play(FileTrack  *item);//set active track, play 
	static void pause();
	static void stop();
	void playNext(BOOL scanID3=TRUE);
	

	
	static	BOOL SavePlaylist(PlayList *pl,LPTSTR filepath);
	PlayList* LoadPlaylist(LPTSTR filepath);

	#define CFGFILENAME _T("coredata")

	BOOL LoadCoreCfg();
	BOOL SaveCoreCfg();

	//-----------------------------------------
	enum PlayOrder
	{
		Default,
		Repeat_playlist,
		Repeat_track,
		Random,
		Shuffle_tracks,
		Shuffle_albums,
		Shuffle_folders,
	};
	PlayOrder playorder;
	void SetPlayOrder(enum PlayOrder index){playorder=index;}

	template<class _InIt> inline
	_InIt GetNextByOrder(_InIt _cur)
	{
		_InIt next;

		if (playorder==Default)
		{
			next=++_cur;
		}
		else if (playorder==Repeat_playlist)
		{
			next=++(++_cur);
		}
		else if (playorder==Repeat_track)
		{
			next=_cur;
		}
		else if (playorder==Random)
		{
			next=++_cur;
		}
		else if (playorder==Shuffle_tracks)
		{
			next=++_cur;
		}
		else if (playorder==Shuffle_albums)
		{
			next=++_cur;
		}
		else if (playorder==Shuffle_folders)
		{
			next=++_cur;
		}

		return next;
	}
};

//count=sizeof(gPlayOrderStr)/sizeof(int)
static const TCHAR *gPlayOrderStr[] =
{
	_T("Default"),
	_T("Repeat (playlist)"),
	_T("Repeat (track)"),
	_T("Random"),
	_T("Shuffle (tracks)"),
	_T("Shuffle (albums)"),
	_T("Shuffle (folders)"),
};