#include "MySerialize.h"
#include "customMsg.h"

class MyConfigs:public SerializeObj<MyConfigs>
{
public:
	MyConfigs():bResumeOnReboot(1),playersVolume(100),playorder(0)
		,playlistIndex(-1),trackIndex(-1),playingStatus(status_stopped)
	{

	}
public:
	FILE& operator<<(FILE& f);
	FILE& operator>>(FILE& f);
public:
	
	inline int getResumeOnReboot(){return bResumeOnReboot;}
	inline int getPlayersVolume(){return playersVolume;}

	inline void setResumeOnReboot(int bResumeOnReboot){this->bResumeOnReboot=bResumeOnReboot;}
	inline void setPlayersVolume(int playersVolume){ this->playersVolume=playersVolume;}

public:
	//������ָ�����
	int bResumeOnReboot;
	//�����ָ������������Ϣ
	
	int playlistIndex;     //�ڼ��������б�
	int trackIndex;                //�б��еĵڼ�������
	
	PlayingStatus playingStatus;
	trackPosInfo pos;    //����Ĳ��Ž���


	//������������
	int playersVolume;

	//���Ŵ���
	int playorder;
};

MyConfigs* GetMyConfigs();