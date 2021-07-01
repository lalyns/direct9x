#include "d3dUtility.h"


bool d3d::InitD3D(
	HINSTANCE hInstance, 
	int width, int height, 
	bool windowed,
	D3DDEVTYPE deviceType, 
	IDirect3DDevice9 ** device)
{
	WNDCLASS wc;
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= (WNDPROC)d3d::WndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(0, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(0, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= "Direct3D9App";

	if (!RegisterClass(&wc)) {
		::MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = CreateWindow("Direct3D9App", "Direct3D9App",
		WS_EX_TOPMOST, 0, 0, width, height,
		0 /* parent hwnd */, 0 /* menu */, hInstance, 0 /* extra */);

	if (!hwnd) {
		::MessageBox(0, "CreateWindow() - FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	/*
			Init D3D
	*/

	HRESULT hr = 0;

	// 1�ܰ� : IDirect3D9 ��ü�� �����.
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9) {
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// 2�ܰ� : �ϵ���� ���ؽ� ���μ����� Ȯ���Ѵ�.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, // �⺻ ���÷��� ��͸� ǥ���Ѵ�.
		deviceType,			// ��ġ Ÿ���� �����Ѵ�. ������ D3DEVTYPE_HAL �̴�.
		&caps);				// �⺻ ���÷��� ����� Ư������ ä���� D3DCAPS9 ����ü�� ��ȯ�Ѵ�.

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// �ϵ���� ���ؽ� ���μ����� �����ȴ�
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// �ϵ���� ���ؽ� ���μ����� �������� �ʴ´�.
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3�ܰ� D3DPRESENT_PARAMETERS ����ü ä���
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth				= width;							// �ȼ� ������ �ĸ� ���� ����
	d3dpp.BackBufferHeight				= height;							// �ȼ� ������ �ĸ� ���� ����
	d3dpp.BackBufferFormat				= D3DFMT_A8R8G8B8;					// �ĸ� ������ �ȼ� ����
	d3dpp.BackBufferCount				= 1;								// �̿��� �ĸ� ������ ��. ������ "1"�� �����ϰ� �� ���� �ĸ� ���۸� �̿��Ѵ�.
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;				// �ĸ� ���ۿ� �̿��� ��Ƽ ���ø��� Ÿ��.
	d3dpp.MultiSampleQuality			= 0;								// ��Ƽ ���ø��� ����
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;			// �ø��� ü�� ���۰� ��ȯ�Ǵ� ����� ����
	d3dpp.hDeviceWindow					= hwnd;								// ���񽺿� ����� ������ �ڵ�. ����� ����� �� ���ø����̼� â�� ����
	d3dpp.Windowed						= windowed;							// [true] ������ ��� / [false] ��ü ȭ�� ���
	d3dpp.EnableAutoDepthStencil		= true;								// [true] Direct3D�� �ڵ����� ����/���ٽ� ���۸� ����� ����
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;						// ����/���ٽ� ������ ����
	d3dpp.Flags							= 0;								// �ĸ���ۿ� Ư���� �ο��Ѵ� [0]�� Ư������.
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// ������� �����Ѵ�
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// ��� ������ �����Ѵ� 

	// 4�ܰ� ��ġ�� �����Ѵ�.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,	// �⺻ �����
		deviceType,			// ��ġ Ÿ��
		hwnd,				// ��ġ�� ����� â
		vp,					// ���ؽ� ���μ��� Ÿ��
		&d3dpp,				// �ÿ� ����
		device);			// ������ ��ġ

	if (FAILED(hr)) {
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			device);
	}

	if (FAILED(hr)) {
		d3d9->Release();
		::MessageBox(0, "CreateDevice() - FAILED", 0, 0);
		return false;
	}

	d3d9->Release();
	return true;
}

int d3d::EnterMsgLoop(bool(*ptr_display)(float timeDelta))
{
	MSG msg;
	::ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f;

			ptr_display(timeDelta);		// ���÷��� �Լ��� ȣ���Ѵ�.

			lastTime = currTime;
		}
	}
	return msg.wParam;
}