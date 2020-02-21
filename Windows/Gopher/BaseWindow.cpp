#pragma once
#include <windows.h>
#include "resource.h"


template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE *pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = ClassName();
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
		//wc.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, 0);

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }

protected:

	virtual PCWSTR  ClassName() const = 0;
	inline virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};


class MainWindow : public BaseWindow<MainWindow>
{
public:
	PCWSTR  ClassName() const { return L"Sample Window Class"; }
	inline LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};


LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hwnd, &ps);

		RECT clientRect;
		RECT textRect;
		HRGN bgRgn;
		HBRUSH hBrush;
		HPEN hPen;

		// Fill the client area with a brush
		GetClientRect(this->Window(), &clientRect);
		bgRgn = CreateRectRgnIndirect(&clientRect);
		hBrush = CreateSolidBrush(RGB(0, 200, 0));
		FillRgn(hdc, bgRgn, hBrush);




		hPen = CreatePen(PS_DOT, 1, RGB(0, 255, 0));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(0, 0, 0));
		Rectangle(hdc, 10, 10, 200, 200);

		// Text caption
		SetBkColor(hdc, RGB(255, 255, 255));
		SetRect(&textRect, 10, 210, 200, 200);
		DrawText(hdc, TEXT("PS_DOT"), -1, &textRect, DT_CENTER | DT_NOCLIP);


		hPen = CreatePen(PS_DASHDOTDOT, 1, RGB(0, 255, 255));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(255, 0, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 0)));
		Rectangle(hdc, 210, 10, 400, 200);

		// Text caption
		SetBkColor(hdc, RGB(255, 200, 200));
		SetRect(&textRect, 210, 210, 400, 200);
		DrawText(hdc, TEXT("PS_DASHDOTDOT"), -1, &textRect, DT_CENTER | DT_NOCLIP);


		hPen = CreatePen(PS_DASHDOT, 1, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(255, 255, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(100, 200, 255)));
		Rectangle(hdc, 410, 10, 600, 200);

		// Text caption
		SetBkColor(hdc, RGB(200, 255, 200));
		SetRect(&textRect, 410, 210, 600, 200);
		DrawText(hdc, TEXT("PS_DASHDOT"), -1, &textRect, DT_CENTER | DT_NOCLIP);


		// When fnPenStyle is PS_SOLID, nWidth may be more than 1.
		// Also, if you set the width of any pen to be greater than 1, 
		// then it will draw a solid line, even if you try to select another style.
		hPen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
		SelectObject(hdc, hPen);
		// Setting the background color doesn't matter 
		// when the style is PS_SOLID
		SetBkColor(hdc, RGB(255, 255, 255));
		SelectObject(hdc, CreateSolidBrush(RGB(200, 100, 50)));
		Rectangle(hdc, 10, 300, 200, 500);

		// Text caption
		SetBkColor(hdc, RGB(200, 200, 255));
		SetRect(&textRect, 10, 510, 200, 500);
		DrawText(hdc, TEXT("PS_SOLID"), -1, &textRect, DT_CENTER | DT_NOCLIP);

		hPen = CreatePen(PS_DASH, 1, RGB(0, 255, 0));
		SelectObject(hdc, hPen);
		SetBkColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(200, 200, 255)));
		Rectangle(hdc, 210, 300, 400, 500);

		// Text caption
		SetBkColor(hdc, RGB(255, 255, 200));
		SetRect(&textRect, 210, 510, 400, 200);
		DrawText(hdc, TEXT("PS_DASH"), -1, &textRect, DT_CENTER | DT_NOCLIP);

		hPen = CreatePen(PS_NULL, 1, RGB(0, 255, 0));
		SelectObject(hdc, hPen);
		// Setting the background color doesn't matter 
		// when the style is PS_NULL
		SetBkColor(hdc, RGB(0, 0, 0));
		SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 255)));
		Rectangle(hdc, 410, 300, 600, 500);

		// Text caption
		SetBkColor(hdc, RGB(200, 255, 255));
		SetRect(&textRect, 410, 510, 600, 500);
		DrawText(hdc, TEXT("PS_NULL"), -1, &textRect, DT_CENTER | DT_NOCLIP);


		// Clean up
		DeleteObject(bgRgn);
		DeleteObject(hBrush);
		DeleteObject(hPen);

		GetStockObject(WHITE_BRUSH);
		GetStockObject(DC_PEN);





		//FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(m_hwnd, &ps);
	}
	return 0;

	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
	return TRUE;
}
