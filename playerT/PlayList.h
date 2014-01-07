#pragma once
#include "MySerialize.h"
#include "LrcMng.h"
#include "fileMonitor.h"


#include <iostream>
#include <fstream>
#include <string>
#include <list>

//for parse the ID3 tag
#include <direct.h>
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
#include <tbytevector.h>
#include <attachedpictureframe.h>

class PlayList;
class CPlayListView;
/*
PlayListItem is outside of playlistitemproxy
PlayListItemProxy is true data item ,datas from file
, count based
if count =0 , it will dealloced
*/


class FileTrack: public SerializeObj<FileTrack>
{
public:
	FILE& operator<<(FILE& f);
	FILE& operator>>(FILE& f);
private:
	//PlayListItem *parent;
public:
	FileTrack():playCount(0),starLvl(0)
		,pPicBuf(NULL),img(NULL)
		,m_bLrcInner(FALSE),m_bLrcFromLrcFile(FALSE)
		,bUnsynLyc(FALSE),m_bStatus(UNKNOWN),year(_T("???")),uYear(0)
	{

	}


	FileTrack(std::tstring &url):url(url),
		playCount(0),starLvl(0)
		,pPicBuf(NULL),img(NULL)
		,m_bLrcInner(FALSE),m_bLrcFromLrcFile(FALSE)
		,bUnsynLyc(FALSE),m_bStatus(UNKNOWN),year(_T("???")),uYear(0)
	{

	}

	~FileTrack();


public:
	std::tstring url;
	UINT          playCount;
	UINT		  starLvl;
	
	std::tstring  title,artist,album,genre,year,comment;
	UINT                                   uYear;
	TagLib::ByteVector *pPicBuf;
	CImage *img;
	//cimg_library_suffixed::CImg   *img;

	//lyrics stuffs
	BOOL m_bLrcInner;
	std::tstring  lyricInner;

	BOOL m_bLrcFromLrcFile;
	std::tstring  lycPath;

	BOOL bUnsynLyc;
public:

	enum ID3Status
	{
		UNKNOWN,
		INVALIE,
		ID3V2,
		ID3V1
	};
	ID3Status m_bStatus;

	BOOL  IsFileExist();
	BOOL  ScanId3Info( BOOL bRetainPic=FALSE,BOOL forceRescan=TRUE);
	const TCHAR* GetTitle(){return title.c_str();}
	BOOL  GetLrcFileFromLib(BOOL forceResearch=FALSE);
	BOOL  HaveKeywords(TCHAR *keywords);
	void TryLoadLrcFile(std::tstring &filename,BOOL forceLoad=FALSE);
public:
	//void SetParent(PlayListItem *p){parent=p;}
	//PlayListItem *GetParent(){return parent;}
private:
	void Buf2Img(BYTE* lpRsrc,DWORD len);
	BOOL LrcFileMacth(std::tstring &filename);
};



class PlayListItem
 {
	 static UINT m_globalid;
public:
	 UINT m_id;
public:
	PlayListItem():index(-1)
	{
		m_id=m_globalid++;
	}

	PlayListItem(PlayList *playlist,std::tstring url)
		:pPL(playlist),filetrack(new FileTrack(url)),index(-1)
	  {
		  //pfiletrack=&filetrack;
		  //filetrack->SetParent(this);
		  m_id=m_globalid++;
	  }

	  PlayListItem(PlayList *playlist)
		  :pPL(playlist),filetrack(new FileTrack()),index(-1)
	  {
		  // pfiletrack=&filetrack;
		  //filetrack->SetParent(this);
		  m_id=m_globalid++;
	  }


	  ~PlayListItem()
	  {

	  };

	  //Move赋值拷贝
	  PlayListItem& operator=(PlayListItem&& b)
	  {
		  if (this!=&b)
		  {
			  this->SetPlayList(b.GetPlayList());
			  this->filetrack=b.filetrack;
		  }

		  return *this;
	  }

	  //copy a new one
	  PlayListItem* Duplicate()
	  {
		  PlayListItem *item=new PlayListItem;
		  *item=*this;
		  return item;
	  }

	  bool operator==(const PlayListItem &other);


	  inline PlayList * GetPlayList(){return this->pPL;}
	  inline void  SetPlayList(PlayList *pPL){this->pPL=pPL;}

	  //proxy functions
	  FILE& operator<<(FILE& f){return *GetFileTrack()<<f;}
	  FILE& operator>>(FILE& f){return *GetFileTrack()>>f;}



	  BOOL  ScanId3Info( BOOL bRetainPic=FALSE,BOOL forceRescan=TRUE){return filetrack->ScanId3Info(bRetainPic,forceRescan);}
	  const TCHAR* GetTitle(){return filetrack->GetTitle();}
	  BOOL  GetLrcFileFromLib(BOOL forceResearch=FALSE){return filetrack->GetLrcFileFromLib(forceResearch);}
	  BOOL  HaveKeywords(TCHAR *keywords){return filetrack->HaveKeywords(keywords);}
	  void TryLoadLrcFile(std::tstring &filename,BOOL forceLoad=FALSE){return filetrack->TryLoadLrcFile(filename,forceLoad);}
	  BOOL  IsFileExist(){return filetrack->IsFileExist();}



FileTrack* GetFileTrack(){return filetrack.get();}
int GetIndex(){return index;}
bool isValide(){return GetIndex()!=-1;}
void SetIndex(int index){this->index=index;}
private:
	PlayList *pPL;

	shared_ptr<FileTrack> filetrack;
	
	//index in playlist
	int index;
};



typedef PlayListItem*  _songContainerItem;
#define NULLITEM NULL;
typedef  bool (*CompareProc)(PlayListItem *a, PlayListItem *b);
class PlayList:
	public SerializeObj<PlayList>
{
public:
	PlayList(void);
	PlayList(std::tstring &name);
	~PlayList(void);
public:
	FILE& operator<<(FILE& f);
	FILE& operator>>(FILE& f);

public:
	typedef vector<_songContainerItem> _songContainer;
private:
	_songContainer m_songList;
public:
	int GetItemCount(){return m_songList.size();}

	_songContainerItem GetItem(int nItem)
	{
		if(nItem<0 || nItem >= GetItemCount())
		{
			return NULLITEM;
		}
		else
			return m_songList[nItem];
	}

	std::tstring       m_playlistName;
	void Rename(TCHAR *newName){m_playlistName=newName;}


public:
	//_songContainerItem nextTrack();
	
	void SortItems(CompareProc compFunc);

	void ReverseItems();
public:
	//operation
	void DeleteItem(int nItem);
	//void DeleteTrack(int nItem,int nLastItem);
	void DeleteAllItems();
	void DeleteTrackByPath(TCHAR *path);
	void ChangeTrackPath(TCHAR *from,TCHAR *to);
	//void DeleteTrack(PlayListItem* track);

public:
	HANDLE hAddDir;
	HANDLE AddFolderByThread(LPCTSTR pszFolder);
	void TerminateAddDirThread();

	//return file added
	int AddFolder(LPCTSTR pszFolder,BOOL bIncludeDir=FALSE);
	BOOL AddFile(TCHAR *filepath);
	void AddItem(_songContainerItem item);
	_songContainerItem GetNextTrackByOrder(BOOL bMoveCur=TRUE);

public:

	//自动播放列表？
	BOOL m_bAuto,m_bSearch;//是否为搜索列表
	inline int GetPlayingIndex(){return nItemPlaying;}
	inline void SetPlayingIndex(int nItem){nItemPlaying=nItem;}
	PlayListItem *GetPlayingItem(){return GetItem(GetPlayingIndex());}


	void SetSelectedIndex(int i){selectedIndex=i;}
	int  GetSelectedIndex(){return selectedIndex;}
	PlayListItem *GetSelectedItem(){return GetItem(GetSelectedIndex());}

	void SetTopVisibleIndex(int i){topVisibleIndex=i;}
	int  GetTopVisibleIndex(){return topVisibleIndex;}
private:
	int nItemPlaying;//正在播放的项目
	//this data will used in list view when display
	int topVisibleIndex;
	int selectedIndex;	
};