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

// �ڿ��� �Ҵ��̳� ��ġ Ư���� Ȯ��, ���ø����̼� ������ ���� ��� ���� ������ �����ϱ� ���� ���� �غ� �۾��� �� �Լ����� ����ȴ�.
bool Setup()
{
	// Nothing to setup in this sample.

	return true;
}

// Setup �Լ����� �Ҵ�� �ڿ��� �����ϴ� �۾��� �����Ѵ�.
void Cleanup()
{
	// Nothing to cleanup in this sample.
}

// �� �Լ��� ��� ����� �ڵ带 �����ϸ�, ��ü�� ��ġ�� �����ϴ� ���� �� �����Ӹ��� �Ͼ�� �ϴ� �۾��� �����Ѵ�. 
// timeDelta ���ڴ� �� �������� ��� �ð��̸�, �ʴ� �����Ӱ� �ִϸ��̼� ����ȭ�� �̿�ȴ�.
bool Display(float timeDelta)
{
	if (Device) // Only use Device methods if we have a valid device.
	{
		// �ĸ� ���۸� �Ұ��ϰ� ����/���ٽ� ���۸� ���� �������� 1.0���� �ʱ�ȭ�ϴ� �ż��� ȣ��
		/* Clear �żҵ� ����
		STDMETHOD(Clear)(THIS_ DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) PURE;
		 Count - pRects �迭 �� �簢���� ��
		 pRects - �Ұ��Ϸ��� ȭ�� �簢���� �迭, ǥ���� Ư�� �������� �Ұ��� �� �ֵ��� ���ش�.
		 Flags - � ǥ���� �Ұ����� �����Ѵ�. �ٸ��� ���� ǥ����� �Ұ��� �� �ִ�.
		  - D3DCLEAR_TARGET - ���� ��� ǥ��, ������ �ĸ� �����̴�.
		  - D3DCLEAR_ZBUFFER - ���� ����
		  - D3DCLEAR_STENCLL - ���ٽ� ����
		 Color - ���� ����� �Ұ��� ����
		 Z - ���� ����(z-����)�� �����ϰ��� �ϴ� ��
		 Stencil - ���ٽ� ���ۿ� �����ϰ��� �ϴ� ��
		 */

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);


		// �ĸ� ���� �ÿ�.
		Device->Present(0, 0, 0, 0);
	}
	return true;
}

//
// WndProc
// �� ���� �̺�Ʈ�� ó���ϱ� ���� ������ ���ν��� �żҵ�
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
// 1. ���� ���÷��� ������� Direct3D�� �ʱ�ȭ�Ѵ�.
// 2. Setup �Լ��� ȣ���Ͽ� ���ø����̼� ���� �غ� �����Ѵ�.
// 3. Display�� ���÷��� �Լ��� �̿��Ͽ� �޽��� ������ �����Ѵ�.
// 4. ���ø����̼��� �����ϰ� ���������� IDIrect3DDevice9 ��ü�� �����Ѵ�.
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