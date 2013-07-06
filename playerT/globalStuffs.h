#include <string>
#include <vector>
#pragma once
class CMainFrame;
class CMultiSpliltWnd;
class PlayList;
class PlayListItem;

CMainFrame* GetMainFrame();
CMultiSpliltWnd* GetSplitter();

//��ǰ���ڲ�����ɼ��Ĳ����б�
//��һ�������ڲ��ŵ��������ڵ��б�
PlayList* ActivePlaylist();
void SetActivePlaylist(PlayList* p);


PlayListItem  GetPlayingItem();
void  SetPlayingItem(PlayListItem item);



class MYTREE;
void SetRootTree(MYTREE *root);
MYTREE *UISplitterTreeRoot();




#define CFGFILENAME _T("Cfg")
#define UIFILENAME  _T("UsrInterfaceCfg")
bool LoadCoreCfg();
bool SaveCoreCfg();

bool LoadUICfg();
bool SaveUICfg();


void CollectInfo();
void ValidateCfg();



//���浱ǰ������Ŀ�����
// int GetPlayingIndex();
// void SetPlayingIndex();

//�Ƿ���������Ŀ¼��Ϊֹ,��Ļ�,��������ȫƥ��ļ���.
bool GetSearchLrcUntilEnd();
void SetSearchLrcUntilEnd(bool b);


enum _match_type
{
	perfect=0,
	without_artist=0x0001,
	without_title=0x0002,
	without_artist_title=without_artist| without_title,
	artist_mismatch=0x0004,
	title_mismatch=0x0008,
	artist_mismatch_no_title=artist_mismatch|without_title,
	title_artist_mismatch_no_artist=title_mismatch|without_artist,
	title_artist_mismatch=title_mismatch| artist_mismatch ,
	invalide=0xFFFF
};

struct LrcMatchItem
{
public:
	LrcMatchItem():match_type(invalide)
	{}
	std::wstring path;
	_match_type match_type;

	


	const TCHAR* LrcMatchItem::GetReason()
	{
		static const TCHAR* reason[]=
		{
			_T("��ȫƥ��"),
			_T("~������"),
			_T("~����"),
			_T("~���� ~������"),
			_T("!������"),
			_T("!����"),
			_T("~����!������"),
			_T("!���� ~������"),
			_T("!���� !������"),
			_T("�������!")
		};

		int iMatch;
		switch (match_type)
		{
		case perfect:
			iMatch=0;
			break;
		case without_artist:
			iMatch=1;
			break;
		case without_title:
			iMatch=2;
			break;
		case without_artist_title:
			iMatch=3;
			break;
		case artist_mismatch:
			iMatch=4;
			break;
		case title_mismatch:
			iMatch=5;
			break;
		case artist_mismatch_no_title:
			iMatch=6;
			break;
		case title_artist_mismatch_no_artist:
			iMatch=7;
			break;
		case title_artist_mismatch:
			iMatch=8;
			break;
		case invalide:
			iMatch=9;
			break;
		}

		return reason[iMatch];
	}
};


std::vector<LrcMatchItem> GetLrcMatchList();
void ClearLrcMatchList();
void AddToLrcMatchList(LrcMatchItem &item);

LrcMatchItem GetHighestMatchLrc();
void SetHighestMatchLrc(LrcMatchItem &item);



struct trackPosInfo;
trackPosInfo *getTrackPosInfo();





struct TrackFormatInfo
{
	int type;
	int rate;
	DWORD  nSamplesPerSec;
	int mode; 
};