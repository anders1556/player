#include "stdafx.h"
#include "BasicPlayer.h"
#include "MusicFile.h"
#include "PlayerThread.h"
#include "SpectrumAnalyser.h"
#include "Mp3File.h"
#include "WaveFileEx.h"

 CBasicPlayer* CBasicPlayer::shared()
 {
 	static CBasicPlayer *pGlobalBasePlayer=NULL;
 	if(!pGlobalBasePlayer)
 		pGlobalBasePlayer=new CBasicPlayer;
 	return pGlobalBasePlayer;
 }


 CBasicPlayer :: CBasicPlayer(void):
 m_bStopped(TRUE),m_bPaused(TRUE),m_pFile(NULL),m_bFileEnd(FALSE)
{
	m_pPlayerThread=new CPlayerThread(this);
	m_pSpectrumAnalyser=new CSpectrumAnalyser;
}

CBasicPlayer :: ~CBasicPlayer(void)
{
	if(!m_pPlayerThread) delete m_pPlayerThread;
	if (!m_pSpectrumAnalyser) delete m_pSpectrumAnalyser; 
}



void CBasicPlayer::play()
{
	if (m_bFileEnd)
		return;
	
	if (!m_bStopped)
		if (m_bPaused)      //δֹͣ����ͣ
		{
			pause();        //����
			return;
		}
		else               //���ڲ���
			stop();        //��ֹͣ

	m_bStopped=FALSE;
	m_bPaused=FALSE;

	///set file to Init
	m_pPlayerThread->reset();
	m_pPlayerThread->CleanDSBufferByTrackBegin();
	m_pPlayerThread->WriteDataToDSBuf();
	m_pPlayerThread->m_lpDSBuffer->Play( 0, 0, DSBPLAY_LOOPING);
	m_pPlayerThread->m_lpDSBuffer->SetVolume(DSBVOLUME_MAX);
	m_pPlayerThread->Resume();
}


void CBasicPlayer::pause()
{
	if (m_bStopped)
		return;

	if (m_bPaused)    //resume
	{
		m_pPlayerThread->m_lpDSBuffer->SetVolume(DSBVOLUME_MAX);
		m_pPlayerThread->m_lpDSBuffer->Play( 0, 0, DSBPLAY_LOOPING);
		m_pPlayerThread->Resume();
		m_bPaused=FALSE;
	}
	else               //pause
	{
		m_pPlayerThread->m_lpDSBuffer->SetVolume(DSBVOLUME_MIN);
		m_pPlayerThread->m_lpDSBuffer->Stop();
		m_pPlayerThread->Suspend();
		m_bPaused=TRUE;
	}
}

void CBasicPlayer::stop()
{
	if(!m_bStopped)
	{
		m_bStopped=TRUE;
		m_bPaused=TRUE;

		m_pPlayerThread->m_lpDSBuffer->SetVolume(DSBVOLUME_MIN);
		m_pPlayerThread->m_lpDSBuffer->Stop();
		m_pPlayerThread->Suspend();

		//m_pPlayerThread->Renew();
	}
}


void CBasicPlayer::open( LPTSTR filepath )
{
	int len=_tcslen(filepath);
	TCHAR* p=filepath+len;
	while (p--)
	{
		if ((TCHAR)(*p)=='.')
		{
			p++;
			break;
		}
	}

	if(m_pFile)
		delete m_pFile;
	m_bFileEnd=FALSE;

	if (_tcscmp(p,_T("wav"))==0)
	{
		m_pFile=new WaveFile();
	}
	if (_tcscmp(p,_T("mp3"))==0)
	{
		m_pFile=new Mp3File();
	}
	if (_tcscmp(p,_T("wma"))==0)
	{
		m_pFile=new Mp3File();
	}

	m_pFile->OpenAndReadID3Info(filepath);
	
	mpg123_id3v1 *id3v1=m_pFile->m_pMpg123_id3v1;
}