#pragma once
#include <Windows.h>



template <class DERIVED_TYPE>
class BaseWindow
{
	public:
		
		virtual LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		BOOL Create(
			PCWSTR lpWindowName,
			DWORD dwStyle,
			DWORD dwExStyle,
			int x,
			int y,
			int nWidth,
			int nHeigh,
			HWND hWndParent,
			HMENU hMenu
		);
		//LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

		BaseWindow() : m_hwnd(NULL) { };

	protected:
		virtual PCWSTR  ClassName() const = 0;
		inline virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
		HWND m_hwnd;

		//BaseWindow();

};


