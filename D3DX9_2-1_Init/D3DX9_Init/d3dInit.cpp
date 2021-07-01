//////////////////////////////////////////////////////////////////////////////////////////////////
// 
// File: d3dinit.cpp
// 
// Author: Frank Luna (C) All Rights Reserved
//
// System: AMD Athlon 1800+ XP, 512 DDR, Geforce 3, Windows XP, MSVC++ 7.0 
//
// Desc: Demonstrates how to initialize Direct3D, how to use the book's framework
//       functions, and how to clear the screen to black.  Note that the Direct3D
//       initialization code is in the d3dUtility.h/.cpp files.
//          
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"

//
// Globals
//

IDirect3DDevice9* Device = 0;

//
// Framework Functions
//

// 자원이 할당이나 장치 특성의 확인, 어플리케이션 상태의 설정 등과 같이 예제를 실행하기 위한 사전 준비 작업이 이 함수에서 수행된다.
bool Setup()
{
	// Nothing to setup in this sample.

	return true;
}

// Setup 함수에서 할당된 자원을 해제하는 작업을 수행한다.
void Cleanup()
{
	// Nothing to cleanup in this sample.
}

// 이 함수는 모든 드로잉 코드를 구현하며, 물체의 위치를 수정하는 등의 매 프레임마다 일어나야 하는 작업을 수행한다. 
// timeDelta 인자는 각 프레임의 경과 시간이며, 초당 프레임과 애니메이션 동기화에 이용된다.
bool Display(float timeDelta)
{
	if (Device) // Only use Device methods if we have a valid device.
	{
		// 후면 버퍼를 소거하고 깊이/스텐실 버퍼를 각각 검은색과 1.0으로 초기화하는 매서드 호출
		/* Clear 매소드 성분
		STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
		 Count - pRects 배열 내 사각형의 수
		 pRects - 소거하려는 화면 사각형의 배열, 표면의 특정 영역만을 소거할 수 있도록 해준다.
		 Flags - 어떤 표면을 소거할지 지정한다. 다름과 같은 표면들을 소거할 수 있다.
		  - D3DCLEAR_TARGET - 렌더 대상 표면, 보통은 후면 버퍼이다.
		  - D3DCLEAR_ZBUFFER - 깊이 버퍼
		  - D3DCLEAR_STENCLL - 스텐실 버퍼
		 Color - 렌더 대상을 소거할 색상
		 Z - 깊이 버퍼(z-버퍼)에 지정하고자 하는 값
		 Stencil - 스텐실 버퍼에 지정하고자 하는 값
		 */

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


		// 후면 버퍼 시연.
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
// 몇 가지 이벤트를 처리하기 위한 윈도우 프로시저 매소드
//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
// 1. 메인 디스플레이 윈도우와 Direct3D를 초기화한다.
// 2. Setup 함수를 호출하여 애플리케이션 사전 준비를 수행한다.
// 3. Display를 디스플레이 함수로 이용하여 메시지 루프에 진입한다.
// 4. 애플리케이션을 정리하고 최종적으로 IDIrect3DDevice9 객체를 해제한다.
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		640, 480, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}