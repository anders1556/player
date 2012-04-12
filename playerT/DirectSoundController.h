#pragma once

#include "dsConfig.h"

class CDirectSoundController
{
private:
	BOOL m_bInit;
	LPDIRECTSOUND m_ppDS;
	LPDIRECTSOUNDBUFFER m_dsBuffer2;
public:
	CDirectSoundController(void);
	~CDirectSoundController(void);
	

//mp3file openandidv3
	//getformat
	//this init create ds dv



	LPDIRECTSOUND DSoundDeviceCreate(LPGUID lpGuid =NULL )
	{
		LPDIRECTSOUND lpDSound;

		if(FAILED(DirectSoundCreate(lpGuid,&lpDSound,NULL) ))return NULL;

		//��������������Ĳ���ģʽ�ļ���
		HWND hWnd=GetForegroundWindow();
		if (!hWnd)
			hWnd=GetDesktopWindow();

		if(FAILED(m_ppDS->SetCooperativeLevel( hWnd, DSSCL_NORMAL) ))return NULL;
                               
		return lpDSound;
	}


	void* DSoundBufferCreate(WAVEFORMATEX *pwfx,INT timesSecond = DEFAULTTIMESBUFFER /*����Ļ��� */)
	{
		//DSBSIZE_MIN  DSBSIZE_MAX
		//�����λ�����
		DSBUFFERDESC dsBufferDesc;
		ZeroMemory(&dsBufferDesc,sizeof(dsBufferDesc) );
		dsBufferDesc.dwSize=sizeof(DSBUFFERDESC);
		dsBufferDesc.dwFlags=DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_GLOBALFOCUS;
		dsBufferDesc.dwBufferBytes=pwfx->nAvgBytesPerSec * timesSecond;   //1�뻺��
		dsBufferDesc.lpwfxFormat=pwfx;

		LPDIRECTSOUNDBUFFER lpDSBuffer=NULL;
		if(FAILED(m_ppDS->CreateSoundBuffer(&dsBufferDesc,&lpDSBuffer,NULL)))return NULL;
		
		return (void*)lpDSBuffer;
	}




};
