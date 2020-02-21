/*-------------------------------------------------------------------------------
    Gopher free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
---------------------------------------------------------------------------------*/


/* ---------------------------------- CHANGELOG ----------------------------------

	changes 0.96 -> 0.97:	speed variable is global, detects bumpers, all timed (no enter), lbumper speed toggler
	changes 0.97 -> 0.98:	performance improvements, operational volume function, shorter beeps, no XY text
	changes 0.98 -> 0.985:	144Hz, Y to hide window(added float stillHoldingY), code cleanup, comments added
	changes 0.985 -> 0.986: Adding configuration file, changing from beeps to vibration.
	changes 0.986 -> 0.989: Improved speeds and speed reporting, created automatic config generator!
	changes 0.989 -> 0.99:	Added speed vibration intensity to vary depending on speed. 
							Implemented SendInput to facilitate (game) UI functionality.


	TODO FOR FUTURE VERSIONS:	
								- fix input lag when changing speed
								- put all buttons into single input device and merely change flags
										perhaps with a map to map controller buttons and read them?

---------------------------------------------------------------------------------- */
#pragma once
#include <windows.h> // for Beep()
#include <iostream>
#include <thread>

//#include "GUI.h"
#include "BaseWindow.cpp"
#include "ButtonWindow.h"

#pragma comment(lib, "XInput9_1_0.lib")
#pragma comment(lib, "winmm") // for volume



#include "Gopher.h"

bool ChangeVolume(double nVolume, bool bScalar); // Not used yet
BOOL isRunningAsAdministrator(); // Check if administrator, makes on-screen keyboard clickable

/* TODO:
 * Enable/disable button
 * Key Codes:
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/dd375731%28v=vs.85%29.aspx
 * xinput:
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/microsoft.directx_sdk.reference.xinput_gamepad%28v=vs.85%29.aspx
 */

void gopherLoop(Gopher gopher)
{
	while (true)
	{
		gopher.loop();
	}
}

void GUILoop(MSG msg = {})
{
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(hwnd, L"Really quit?", L"My application", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(hwnd);
		}
		// Else: User canceled. Do nothing.
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);



			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hwnd, &ps);
		}

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{

	MainWindow win;
	MainWindow extraWin;

	if (!win.Create(L"Gopher Renewed", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN))
	{
		return 0;
	}

	HWND window = win.Window();

	ShowWindow(window, nCmdShow);
	UpdateWindow(window);
	//template <class DERVIED_TYPE>
	ButtonWindow buttonWin;
	if (!buttonWin.Create<ButtonWindow>(L"OK",      // Button text
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 0, // Styles
		100,         // x position
		200,         // y position
		100,        // Button width
		100,        // Button height
		window,     // Parent window
		(HMENU) 101,
		(HINSTANCE)GetWindowLong(window, GWL_HINSTANCE)

	))
	{
		printf("Oh shi- %d\n", GetLastError());
		return 0;
	}

	ShowWindow(buttonWin.Window(), nCmdShow);
	UpdateWindow(buttonWin.Window());
	/*

	buttonWin.Create<ButtonWindow>(
		L"OK", 0,     // Button text
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
		10,         // x position
		10,         // y position
		100,        // Button width
		100//,        // Button height
		//window     // Parent window
		);

	extraWin.Create(
		L"OK",      // Button text 
		WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
		0,			// DwExStyle
		10,         // x position 
		10,         // y position 
		100,        // Button width
		100,        // Button height
		window,     // Parent window
		NULL);/* ,       // No menu.
		(HINSTANCE)GetWindowLong(win.Window(), GWL_HINSTANCE),
		NULL);      // Pointer not needed.
		*/
	
	//ShowWindow(extraWin.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = {};


	CXBOXController controller(1);
	Gopher gopher(&controller);

	gopher.loadConfigFile();
	std::thread t1(gopherLoop, gopher);
	
	//InvalidateRect(extraWin.Window(), NULL, TRUE);
	//SendMessage(extraWin.Window(), WM_PAINT, 0, 0);
	//InvalidateRect(extraWin.Window(), NULL, FALSE);
	//PostMessage(win.Window(), );

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	

	// Start the Gopher program loop
	/*while (true)
	{
		gopher.loop();
	}*/

	/*
	LPCWSTR g_szClassName = (LPCWSTR)"myWindowClass";
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	//Step 1: Registering the Window Class
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Step 2: Creating the Window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Gopher Renewed",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);


	// Step 3: The Message Loop -> Need to run this alongside gopher loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
	TranslateMessage(&Msg);
	DispatchMessage(&Msg);
	}

	*/



	







	//return Msg.wParam;

	/* --- OLD CONSOLE (GUI) CODE ---
	
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	SetConsoleTitle(TEXT("GopherRenewed"));

	system("Color 1D");

	printf("Welcome to GopherRenewed - a VERY fast and lightweight controller-to-keyboard & mouse input tool.\n");
	printf("All you need is an Xbox360/Xbone controller (wired or wireless adapter), or DualShock (with InputMapper 1.5+)\n");
	printf("Gopher will autofind the xinput device and begin reading input - if nothing happens, verify connectivity.\n");
	printf("See the GitHub repository at bit.ly/1syAhMT for more info. Twitter contact: TylerAt60FPS\n\n-------------------------\n\n");

	SetConsoleTextAttribute(hConsole, 23);
	printf("Gopher is free (as in freedom) software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
	printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.");
	SetConsoleTextAttribute(hConsole, 29);
	printf("\n\n-------------------------\n\n");

	SetConsoleTextAttribute(hConsole, 5); // set color to purple on black (windows only)
										  // 29 default

										  // dump important tips
	printf("Tip - Press left and right bumpers simultaneously to toggle speeds! (Default is left and right bumpers, configurable in config.ini)\n");

	if (!isRunningAsAdministrator())
	{
		printf("Tip - Not running as an admin! Windows on-screen keyboard and others won't work without admin rights.\n");
	}*/
}

/*

int main()
{
  CXBOXController controller(1);
  Gopher gopher(&controller);
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


  SetConsoleTitle( TEXT( "GopherRenewed" ) );

  system("Color 1D");

  printf("Welcome to GopherRenewed - a VERY fast and lightweight controller-to-keyboard & mouse input tool.\n");
  printf("All you need is an Xbox360/Xbone controller (wired or wireless adapter), or DualShock (with InputMapper 1.5+)\n");
  printf("Gopher will autofind the xinput device and begin reading input - if nothing happens, verify connectivity.\n");
  printf("See the GitHub repository at bit.ly/1syAhMT for more info. Twitter contact: TylerAt60FPS\n\n-------------------------\n\n");
  
  SetConsoleTextAttribute(hConsole, 23);
  printf("Gopher is free (as in freedom) software: you can redistribute it and/or modify\nit under the terms of the GNU General Public License as published by\nthe Free Software Foundation, either version 3 of the License, or\n(at your option) any later version.\n");
  printf("\nYou should have received a copy of the GNU General Public License\nalong with this program. If not, see http://www.gnu.org/licenses/.");
  SetConsoleTextAttribute(hConsole, 29);
  printf("\n\n-------------------------\n\n");

  SetConsoleTextAttribute(hConsole, 5); // set color to purple on black (windows only)
  // 29 default

  // dump important tips
  printf("Tip - Press left and right bumpers simultaneously to toggle speeds! (Default is left and right bumpers, configurable in config.ini)\n");

  if (!isRunningAsAdministrator())
  {
    printf("Tip - Not running as an admin! Windows on-screen keyboard and others won't work without admin rights.\n");
  }

  gopher.loadConfigFile();

  // Start the Gopher program loop
  while (true)
  {
    gopher.loop();
  }
}*/

BOOL isRunningAsAdministrator()
{
  BOOL   fRet = FALSE;
  HANDLE hToken = NULL;

  if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
  {
    TOKEN_ELEVATION Elevation;
    DWORD cbSize = sizeof( TOKEN_ELEVATION );

    if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof( Elevation), &cbSize))
    {
      fRet = Elevation.TokenIsElevated;
    }
  }

  if (hToken)
  {
    CloseHandle(hToken);
  }

  return fRet;
}

// This works, but it's not enabled in the software since the best button for it is still undecided
bool ChangeVolume(double nVolume, bool bScalar) //o b
{
  HRESULT hr = NULL;
  bool decibels = false;
  bool scalar = false;
  double newVolume = nVolume;

  CoInitialize(NULL);
  IMMDeviceEnumerator *deviceEnumerator = NULL;
  hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER,
                        __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
  IMMDevice *defaultDevice = NULL;

  hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
  deviceEnumerator->Release();
  deviceEnumerator = NULL;

  IAudioEndpointVolume *endpointVolume = NULL;
  hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume),
                               CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
  defaultDevice->Release();
  defaultDevice = NULL;

  // -------------------------
  float currentVolume = 0;
  endpointVolume->GetMasterVolumeLevel(&currentVolume);
  //printf("Current volume in dB is: %f\n", currentVolume);

  hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);
  //CString strCur=L"";
  //strCur.Format(L"%f",currentVolume);
  //AfxMessageBox(strCur);

  // printf("Current volume as a scalar is: %f\n", currentVolume);
  if (bScalar == false)
  {
    hr = endpointVolume->SetMasterVolumeLevel((float)newVolume, NULL);
  }
  else if (bScalar == true)
  {
    hr = endpointVolume->SetMasterVolumeLevelScalar((float)newVolume, NULL);
  }
  endpointVolume->Release();

  CoUninitialize();

  return FALSE;
}
