#pragma once 

void SdMsg(UINT msg,BOOL bPost=TRUE,WPARAM wparam=NULL,LPARAM lparam=NULL);

struct trackPosInfo
{
	double used;
	double left;
};

#define  WM_TRACKPOS          (WM_USER+22)  

#define  WM_TRACK_REACH_END   (WM_USER+53)//���쵽��ĩβ
#define  WM_TRACKSTOPPED      (WM_USER+34)//���첥��ֹͣ
#define  WM_NEW_TRACK_STARTED (WM_USER+37)//��ʼ���첥��
#define  WM_PAUSED            (WM_USER+38)//��ͣ���첥��
#define  WM_PAUSE_START            (WM_USER+39)//��ʼ��ͣ�����첥��

#define  WM_PLAY_DIRECTLY     (WM_USER+41)

//��������ļ�����  
//wParam TCHAR *filename;
//lParam BOOL , 2 -->file ,1-->started : 0 -->ended
#define  WM_FILE_FINDED  (WM_USER+40) 
#define  WM_DRAWSPECTRUM (WM_PAINT+913)
#define  WM_ADDFOLDERED  (WM_USER+35)     //playlistView will reload playlists

//������Ҫ��Ч�ͱ���
#define  WM_CONFIGTOSAVE (WM_USER+36)


//׼��������ʾ���
#define  WM_LYRIC_RELOAD  (WM_USER+50)

//added or deleted one playlist
//PlayList changed,added or deleted
//playlist added number
//BOOL bAdd=(BOOL)lParam;
//PlayList *pl=(PlayList*)wParam;
#define  WM_PL_CHANGED    (WM_USER+51)

//PlayList
//track number added
#define  WM_PL_TRACKNUM_CHANGED (WM_USER+52)

