#include "App.h"


#define WM_USER_POS_CHANGED (WM_USER + 1)

void CallbackPosChanged(void *data, int iPos)
{
	CAVPlayer *pAVPlayer = (CAVPlayer *) data;

	if (pAVPlayer)
	{
		HWND hWnd = pAVPlayer->GetHWND();

		if (::IsWindow(hWnd) && ::IsWindow(::GetParent(hWnd)))
		{
			::PostMessage(::GetParent(hWnd), WM_USER_POS_CHANGED, (WPARAM)data, iPos);
		}
	}
}


DuiLib::CDuiString App::GetSkinFolder()
{
	return ("skin\\player");
}


DuiLib::CDuiString App::GetSkinFile()
{
	return ("playerSkin.xml");
}


LPCTSTR App::GetWindowClassName(void) const
{
	return ("PlayerClass");
}


LRESULT App::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	::PostQuitMessage(0L);
	return 0;
}


LRESULT App::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(wParam)
	{
	case 1:
		KillTimer(m_hWnd, 1);
		m_cAVPlayer.Play("D:\\1.rmvb");
		break;
	}
	return 0;
}


LRESULT App::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		DuiLib::CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif

	if( !::IsIconic(*this)) {
		RECT host_rect; 
		GetWindowRect(m_hWnd, &host_rect);
		SetWindowPos(m_pCPlayerCtrl->GetHWND(), NULL, 1, 25, (host_rect.right-host_rect.left-2), (host_rect.bottom-host_rect.top-26), 0);
		m_pCPlayerCtrl->ShowWindow(true);
	}

	bHandled = FALSE;
	return 0;
}


LRESULT App::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	switch(uMsg)
	{
	case WM_TIMER: OnTimer( uMsg, wParam, lParam, bHandled); break;
	//case WM_SIZE: OnSize( uMsg, wParam, lParam, bHandled); break;
	default: break;
	}

	return 0;
}


void App::OnInit()
{
	m_pCPlayerCtrl = new CPlayerCtrlArea;
	m_pCPlayerCtrl->Create(m_hWnd, _T(""), UI_WNDSTYLE_CHILD, 0, 0,0,0,0, 0);
	RECT host_rect; 
	GetWindowRect(m_hWnd, &host_rect);
	SetWindowPos(m_pCPlayerCtrl->GetHWND(), NULL, 1, 25, (host_rect.right-host_rect.left-2), (host_rect.bottom-host_rect.top-26), 0);
	m_pCPlayerCtrl->ShowWindow(true);

	m_cAVPlayer.SetHWND(m_pCPlayerCtrl->GetHWND());
	m_cAVPlayer.SetCallback(CallbackPosChanged);
}


void App::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_CLICK) {
		// 点击
		if(msg.pSender->GetName() == _T("mini")) { // 最小化
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		} else if(msg.pSender->GetName() == _T("max")) { // 最大化
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			msg.pSender->SetVisible(false);
			DuiLib::CButtonUI *pRestore = NULL;
			pRestore = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("restore")));
			if(pRestore) {
				pRestore->SetVisible(true);
			}
			return;
		} else if(msg.pSender->GetName() == "restore") { // 恢复
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
			msg.pSender->SetVisible(false);
			DuiLib::CButtonUI *pMax = NULL;
			pMax = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("max")));
			if(pMax) {
				pMax->SetVisible(true);
			}
			return;
		} else if(msg.pSender->GetName() == _T("close")) { // 关闭
			PostQuitMessage(0L);
			return;
		} else if(msg.pSender->GetName() == _T("menu")) { // 菜单
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd(m_hWnd);
			DuiLib::CPoint point(1, 25);
			ClientToScreen(m_hWnd, &point);
			DuiLib::STRINGorID xml("mainmenu\\menulogo.xml");
			pMenu->Init(NULL, xml, "xml", point);
			pMenu->ShowWindow(true, true);
			return;
		}
	}
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	DuiLib::CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	App* pFrame = new App();
	if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, ("Player"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->SetIcon(IDI_ICON_LOGO);
	pFrame->OnInit();
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	DuiLib::CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}