#include "MySerialize.h"
#include "customMsg.h"

class MyConfigs:public SerializeObj
{
public:
	MyConfigs():bResumeOnReboot(1),playersVolume(100),playorder(0)
	{

	}
public:
	int SerializeB(FILE *pFile);

	int ReSerialize(FILE *pFile);
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
	trackPosInfo pos;    //����Ĳ��Ž���

	//������������
	int playersVolume;

	//���Ŵ���
	int playorder;
};

MyConfigs* GetMyConfigs();