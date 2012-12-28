#include "MyLib.h"
#include "BasicPlayer.h"
#include "ProgertyDlg.h"
//-----------------------------------------
//

PlayList*  MyLib::NewPlaylist(std::tstring playlistname)
{
	PlayList *l=new PlayList(playlistname);
	m_playLists.push_back(l);
	return l;
}

PlayList* MyLib::AddFolderToCurrentPlayList(LPCTSTR pszFolder)
{
	PlayList *p=MyLib::shared()->m_pSelPlaylist;
	p->AddFolderByThread(pszFolder);
	return p;
}

MyLib* MyLib::shared()
{
	static MyLib* p=NULL;
	return p==NULL?p=new MyLib:p;
}

MyLib::~MyLib()
{
	SaveCoreCfg();
	CBasicPlayer::shared()->stop();


	PLList::iterator i;
	for (i=m_playLists.begin();i!=m_playLists.end();i++)
		delete *i;
	
}

void MyLib::playAfterSlowDown()
{
	CBasicPlayer::shared()->OpenAfterSlowDown();
}


//-----------------------------------------

void MyLib::playNext(BOOL scanID3)
{	
	PlayListItem *track;
 	if (!playQueue.empty())
		track=PopTrackFromPlayQueue();
	else
		track=ActivePlaylist()->GetNextTrackByOrder();

	if(!track) return;

	//todo update listview's item
	track->ScanId3Info(TRUE);

	CBasicPlayer *sbp=CBasicPlayer::shared();
	if ( track==ActivePlaylist()->lastTrack())
	{
		sbp->m_bFileEnd=FALSE;
		sbp->play();
	}
	else
	{
		sbp->stop();
		sbp->open(track);
		sbp->play();
	}
}

//��˲���,ǰ��ȡ��
void MyLib::PushPlayQueue(PlayListItem* item)
{
	playQueue.push_back(item);
}

PlayListItem* MyLib::PopTrackFromPlayQueue()
{
	PlayQueueContainer::iterator i=playQueue.begin();
	PlayListItem* item=*i;
	playQueue.erase(i);

	return item;
}

vector<int> MyLib::GetIndexInPlayQueue(PlayListItem* item)
{
	vector<int> v;
	int idx=0;
	for(PlayQueueContainer::iterator i=playQueue.begin();
		i!=playQueue.end();++i,++idx)
		if(*i == item)
			v.push_back(idx);
	
	return v;
}

void MyLib::DeleteFromPlayQueue(PlayListItem* item)
{
	for(PlayQueueContainer::iterator i=playQueue.begin();
		i!=playQueue.end();)
	{
		PlayQueueContainer::iterator k=i;
		++i;

		if(*k == item)
			playQueue.erase(k);
	}
}

void MyLib::ClearPlayQueue()
{
	playQueue.clear();
}


LRESULT CPropertyDlgMediaLib::OnBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UINT uFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	uFlags&=~BIF_NONEWFOLDERBUTTON ;
	CFolderDialog dlg(m_hWnd,_T("��ѡ��Ҫ��ӵ��ļ���"),uFlags);
	if (dlg.DoModal()==IDOK)
	{
		LPCTSTR path=dlg.GetFolderPath();
		m_list.InsertItem(0,path);
	}

	return 0;
}

LRESULT CPropertyDlgMediaLib::OnBtnDel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_list.DeleteItem(m_list.GetSelectedIndex());

	return 0;
}

BOOL CPropertyDlgLyricsLib::OnInitDialog(CWindow wndFocus, LPARAM lInitParam)
{
	DoDataExchange();

	UINT style;

	style=::GetWindowLong(list.m_hWnd,GWL_STYLE);
	style|=LVS_SINGLESEL;
	::SetWindowLong(list.m_hWnd,GWL_STYLE,style);

	style=list.GetExtendedListViewStyle();
	style|= LVS_EX_FULLROWSELECT;
	list.SetExtendedListViewStyle(style);

	list.InsertColumn(0,_T("·��"),LVCFMT_LEFT,220);


	vector<std::tstring>::iterator i;int index;
	for (i=MyLib::shared()->lrcDirs.begin(),index=0;i!=MyLib::shared()->lrcDirs.end();i++,index++)
	{
		std::tstring str=*i;
		int nItem=list.InsertItem(index,str.c_str());

		int len=_tcslen(str.c_str());
		LPTSTR pathCopy=new TCHAR[len+1];
		memset(pathCopy,0,(len+1)*sizeof(TCHAR));
		_tcsncpy(pathCopy,str.c_str(),len);
		list.SetItemData(nItem,(DWORD_PTR)pathCopy);
	}
	
	return TRUE;
}


LRESULT CPropertyDlgLyricsLib::OnBtnAdd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	UINT uFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE;
	uFlags&=~BIF_NONEWFOLDERBUTTON ;
	CFolderDialog dlg(m_hWnd,_T("��ѡ��Ҫ��ӵ��ļ���"),uFlags);
	if (dlg.DoModal()==IDOK)
	{
		LPCTSTR path=dlg.GetFolderPath();
		int index=list.GetItemCount();
		index=list.InsertItem(index,path);
		
		int len=_tcslen(path);
		LPTSTR pathCopy=new TCHAR[len+1];
		memset(pathCopy,0,(len+1)*sizeof(TCHAR));
		_tcsncpy(pathCopy,path,len);
		list.SetItemData(index,(DWORD_PTR)pathCopy);
	}

	return 0;
}

LRESULT CPropertyDlgLyricsLib::OnBtnDel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int i=list.GetSelectedIndex();
	LPCTSTR path=(LPCTSTR)list.GetItemData(i);
	list.DeleteItem(i);
	delete[] path;
	return 0;
}


LRESULT CPropertyDlgLyricsLib::OnCfgToSave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{	
	MyLib::shared()->ClearLrcSearchLib();

	int count=list.GetItemCount();
	for (int i=0;i<count;i++)
	{
		LPCTSTR path=(LPCTSTR)list.GetItemData(i);
		MyLib::shared()->AddFolder2LrcSearchLib(path);
	}

	MyLib::shared()->InitLrcLib();
	return 0;
}

void MyLib::InitLrcLib()
{
	dataPaths.clear();
	for_each(lrcDirs.begin(),lrcDirs.end(),&MyLib::ImportLycByPath);
}

void MyLib::ImportLycByPath(std::tstring path)
{
	LPCTSTR pszFolder=path.c_str();

	TCHAR* curPath=new TCHAR[MAX_PATH];
	_tgetcwd(curPath,MAX_PATH);

	//�ı䵱ǰĿ¼
	_tchdir(pszFolder);

	BOOL findResult=FALSE;
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;

	hFind=::FindFirstFile(_T("*.lrc"),&findFileData);
	if(hFind!=INVALID_HANDLE_VALUE){
		findResult=TRUE;
		while(findResult)
		{
			TCHAR *_findName=new TCHAR[MAX_PATH];
			_tcscpy(_findName,findFileData.cFileName);
			TCHAR *pathName=new TCHAR[MAX_PATH+1];
			_tcscpy(pathName,pszFolder);
			_tcscat(pathName,_T("\\"));
			_tcscat(pathName,_findName);
			//std::tstring str(pathName);
			
			std::tstring str(pathName);
			MyLib::shared()->dataPaths.push_back(str);

			findResult=FindNextFile(hFind,&findFileData);
		}
		FindClose(hFind);
	}

	//�ظ���ǰĿ¼
	_tchdir(curPath);
}



void MyLib::DeletePlayList(PlayList *pl)
{
	m_playLists.remove(pl);
	delete pl;
}