#pragma  once

static DWORD g_dwMaxDSBufferLen;  //ds buffer length 
static DWORD g_dwSleepTime;       //  1/4 of the ds buffer
#define  DEFAULTBUFFERSIZE  16000  //one time read from file

inline int DSoundGetDistance(int maxDSBufferLen , int pos1 , int pos2)
{
	int distance=pos2-pos1;
	if (distance<0)
		distance=distance+maxDSBufferLen;
	return distance;
}


LPDIRECTSOUND DSoundDeviceCreate(LPGUID lpGuid =NULL )
{
	LPDIRECTSOUND lpDSound;
	if(FAILED(DirectSoundCreate(lpGuid,&lpDSound,NULL) ))return NULL;

	//��������������Ĳ���ģʽ�ļ���
	HWND hWnd=GetForegroundWindow();
	if (!hWnd)
		hWnd=GetDesktopWindow();

	if(FAILED(lpDSound->SetCooperativeLevel( hWnd, DSSCL_NORMAL) ))return NULL;
	return lpDSound;
}


LPDIRECTSOUNDBUFFER DSoundBufferCreate(LPDIRECTSOUND lpDSound,WAVEFORMATEX *pwfx)
{
	//DSBSIZE_MIN  DSBSIZE_MAX
	//�����λ�����
	DSBUFFERDESC dsBufferDesc;
	ZeroMemory(&dsBufferDesc,sizeof(dsBufferDesc) );
	dsBufferDesc.dwSize=sizeof(DSBUFFERDESC);
	dsBufferDesc.dwFlags=DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS |DSBCAPS_CTRLVOLUME ;
	dsBufferDesc.dwBufferBytes=pwfx->nAvgBytesPerSec;   //1�뻺��
	dsBufferDesc.lpwfxFormat=pwfx;

	LPDIRECTSOUNDBUFFER lpDSBuffer=NULL;
	if(FAILED(lpDSound->CreateSoundBuffer(&dsBufferDesc,&lpDSBuffer,NULL)))return NULL;

	//-------------------
	//ds��������С    1��    11.025�ζ��ļ�
	g_dwMaxDSBufferLen=pwfx->nAvgBytesPerSec;
	g_dwSleepTime=(dsBufferDesc.dwBufferBytes/pwfx->nAvgBytesPerSec)*1000/10;//����ʱ���1/12  <  1/11
	//-------------------

	return lpDSBuffer;
}


//����ʣ���д����
DWORD DS_GetAvailable(int maxDSBufferLen , int playCursor, int curWritePos)
{
	if (playCursor>curWritePos)
		return playCursor-curWritePos;
	else
		return maxDSBufferLen+playCursor-curWritePos;
}

