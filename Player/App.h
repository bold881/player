#pragma once
#include "StdAfx.h"
#include "AVPlayer.h" // vlc_ctrl
#include "PlayerArea.h"
#include "UIMenu.h"


class App : public DuiLib::WindowImplBase
{
private:
	// variable
	CAVPlayer m_cAVPlayer; // ��������
	CPlayerCtrlArea *m_pCPlayerCtrl; // ��������
	// function
	virtual DuiLib::CDuiString GetSkinFolder();
	virtual DuiLib::CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(DuiLib::TNotifyUI& msg);
public:
	// custom function
	void OnInit(void);
};