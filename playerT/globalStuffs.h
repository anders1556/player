
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


PlayListItem * GetPlayingItem();
void  SetPlayingItem(PlayListItem *item);
