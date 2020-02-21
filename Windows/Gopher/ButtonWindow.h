#pragma once
#include "BaseWindow.cpp"
#define BUTTON_MENU1 101
#define BUTTON_ID1 1



//template <class T>
//template <template <class> class T>
//template <template <class> class DERIVED_TYPE>
class ButtonWindow : public BaseWindow<ButtonWindow>//BaseWindow<ButtonWindow<T>>//<ButtonWindow<T>>//<MainWindow<T>>
{
	public:
		PCWSTR  ClassName() const { return L"BUTTON"; }

		//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

		
		
		
		//template <class DERIVED_TYPE>
		//template <template <class> class DERIVED_TYPE>
		LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			PAINTSTRUCT ps;
			HDC hdc;
			TCHAR greeting[] = TEXT("Hello, World!");
			static HWND hButton;

			
			

			switch (uMsg)
			{
			/*case WM_CREATE:
				hButton = CreateWindow(L"button", L"Label",
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
				100, 200,
				50, 20,
				m_hwnd, (HMENU)BUTTON_ID1,
				GetModuleHandle(NULL), NULL);

				break;*/
/*
			case WM_PAINT:
				PAINTSTRUCT ps;
				HDC hdc;
				RECT rect;



				hdc = BeginPaint(m_hwnd, &ps);

				// Here your application is laid out.
				// For this introduction, we just print out "Hello, World!"
				// in the top left corner.
				TextOut(hdc,
					5, 5,
					greeting, (0));
				// End application-specific layout section.



				GetClientRect(m_hwnd, &rect);

				SetTextColor(hdc, RGB(0, 0, 0));
				SetBkMode(hdc, TRANSPARENT);
				DrawText(hdc, TEXT("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				EndPaint(m_hwnd, &ps);
				break;
				//DrawText(hdc, TEXT("PS_DOT"), -1, &textRect, DT_CENTER | DT_NOCLIP);

				EndPaint(Window(), &ps);
				break;*/
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			default:
				return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
				break;
			}

			return 0;
		}




		template <class DERIVED_TYPE>
		BOOL Create(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0,
			HINSTANCE hInstance = GetModuleHandle(NULL)
		)
			/*
			HWND hwndButton = CreateWindow(
			L"BUTTON",  // Predefined class; Unicode assumed
			L"OK",      // Button text
			WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
			10,         // x position
			10,         // y position
			100,        // Button width
			100,        // Button height
			m_hwnd,     // Parent window
			NULL,       // No menu.
			(HINSTANCE)GetWindowLong(m_hwnd, GWL_HINSTANCE),
			NULL);*/

		{
			WNDCLASS wc = { 0 };

			wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
			wc.hInstance = hInstance;
			wc.lpszClassName = ClassName();

			RegisterClass(&wc);

			m_hwnd = CreateWindowEx(
				dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
			);

			return (m_hwnd ? TRUE : FALSE);
		}
};