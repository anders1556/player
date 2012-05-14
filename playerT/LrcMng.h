/*

���������Ƿ������ף����ڷ����С�[*:*]����ʽ�Ķ�Ӧ��Ϊ�Ǳ�ǩ
  �����Ǳ�ǩ����Ӧ��ʾ
��  ���Ǳ�ǩ���ұ�ð�ŷָ��������ֶ�Ϊ�Ǹ��� ����Ӧ��Ϊ��ʱ���ǩ  (0������)
��  ���ڷǱ�׼��ʽ��ʱ���ǩҲӦ��ʶ����[0:0]��  
 �� ���Ǳ�ǩ���ҷ�ʱ���ǩ�ģ�Ӧ��Ϊ�Ǳ�ʶ��ǩ
    ��ʶ���д�Сд�ȼ�
  ��Ϊ�������� ��Ӧ��δ������±�ǩ�����Դ���
����Ӧ��ע�ͱ�ǩ��[:]�����ͬһ�����������Դ���
����Ӧ����һ���д��ڶ����ǩ��������ȷ���� 
  ��Ӧ����ȷ����δ����ı�ǩ��

*/




#ifndef LRCMNG_H
#define LRCMNG_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class LrcTime
{
public:
	LrcTime(UINT min,UINT sec)
		:minute(min),second(sec)
	{
	}
	UINT minute;
	UINT second;

	UINT GetTotalSec(){return minute*60+second;}
};

class LrcLine{
public:
	LrcLine(UINT min,UINT sec,std::tstring text)
		:time(min,sec),text(text)
	{}
	LrcTime time;
	std::tstring text;
	/*return second between*/
	int operator - (LrcLine& r)
	{
		return (this->time.minute-r.time.minute)*60+(this->time.second-r.time.second);
	}
};




class LrcMng
{
public:
	vector<LrcLine> lib;
	void SortLrcLib();
	void InsertIntoLib(UINT minute,UINT second,std::tstring& pLine)
	{
		LrcLine temp(minute,second,pLine);
		lib.push_back(temp);
	}
public:
	LrcMng(void);
	static LrcMng* Get();

public:
	void Open(LPTSTR pstrPath)
	{ 
		std::locale loc1 = std::locale::global(std::locale(".936"));
		tifstream fin(pstrPath);
		std::tstring s;
		while(getline(fin,s))
		{
			Parse(s);
		}
		SortLrcLib();
		fin.close();
		std::locale::global(std::locale(loc1));
	}




private:
	void Parse(std::tstring& s)
	{
		static bool shouldparseideinfo=true;

		std::tstring *pLine=new std::tstring;
		std::tstring temp;
		bool Parsing=false;
		for (int i=0;i<s.length();i++)
		{
			if (s[i]=='[')
			{
				Parsing=true;
				continue;
			}
			if (s[i]==']')
			{
				if (Parsing)//if has '[' before
				{
					Parsing=false;//ƥ��ɹ�
					continue;
				}
			}

			if (Parsing)    //�ռ���ǩ��Ϣ,��00:00 00:11 00:18 
				temp+=s[i];
			else            //�ռ������Ϣ
			(*pLine)+=s[i];
		}

		if (shouldparseideinfo)
			shouldparseideinfo=parseIDEinfo(temp);
		if (!shouldparseideinfo)
			parseTimeInfo(temp,pLine);
	}



	bool parseIDEinfo(std::tstring& temp)
	{
		const std::tstring torken_[]={_T("id"),_T("encoding"),_T("ti"),_T("ar"),_T("al"),_T("by")};

		for (int i=0;i<6;i++)
		{
			std::tstring::iterator f;
			f=find_end(temp.begin(),temp.end(),torken_[i].begin(),torken_[i].end());
			if (f!=temp.end())
			{
				return true;
			}
		}
		return false;
	}

	void parseTimeInfo(std::tstring& temp,std::tstring* pLine)
	{
		int length=temp.length();

		int timeInfoNum=length/5;

		LPCTSTR pbuf=temp.c_str();
		int minute=-2,second=-2;

		for (int i=0;i<timeInfoNum;i++)
		{
			_stscanf(pbuf,_T("%2d:%2d"),&minute,&second);
			InsertIntoLib(minute,second,*pLine);
			pbuf+=5;
		}
	}


public:
	~LrcMng(void);
};

#endif
