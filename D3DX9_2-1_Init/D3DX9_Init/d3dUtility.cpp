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

	// 1단계 : IDirect3D9 객체를 만든다.
	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9) {
		::MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return false;
	}

	// 2단계 : 하드웨어 버텍스 프로세싱을 확인한다.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT, // 기본 디스플레이 어뎁터를 표기한다.
		deviceType,			// 장치 타입을 지정한다. 보통은 D3DEVTYPE_HAL 이다.
		&caps);				// 기본 디스플레이 어뎁터의 특성으로 채워진 D3DCAPS9 구조체를 반환한다.

	int vp = 0;
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		// 하드웨어 버텍스 프로세싱이 지원된다
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		// 하드웨어 버텍스 프로세싱이 지원되지 않는다.
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// 3단계 D3DPRESENT_PARAMETERS 구조체 채우기
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth				= width;							// 픽셀 단위의 후면 버퍼 넓이
	d3dpp.BackBufferHeight				= height;							// 픽셀 단위의 후면 버퍼 높이
	d3dpp.BackBufferFormat				= D3DFMT_A8R8G8B8;					// 후면 버퍼의 픽셀 포멧
	d3dpp.BackBufferCount				= 1;								// 이용할 후면 버퍼의 수. 보통은 "1"을 지정하고 한 개의 후면 버퍼를 이용한다.
	d3dpp.MultiSampleType				= D3DMULTISAMPLE_NONE;				// 후면 버퍼에 이용할 멀티 샘플링의 타입.
	d3dpp.MultiSampleQuality			= 0;								// 멀티 샘플링의 레벨
	d3dpp.SwapEffect					= D3DSWAPEFFECT_DISCARD;			// 플리핑 체인 버퍼가 교환되는 방법을 지정
	d3dpp.hDeviceWindow					= hwnd;								// 서비스와 연결된 윈도우 핸들. 드로잉 대상이 될 어플리케이션 창을 지정
	d3dpp.Windowed						= windowed;							// [true] 윈도우 모드 / [false] 전체 화면 모드
	d3dpp.EnableAutoDepthStencil		= true;								// [true] Direct3D가 자동으로 깊이/스텐실 버퍼를 만들고 관리
	d3dpp.AutoDepthStencilFormat		= D3DFMT_D24S8;						// 깊이/스텐실 버퍼의 포멧
	d3dpp.Flags							= 0;								// 후면버퍼에 특성을 부여한다 [0]은 특성없음.
	d3dpp.FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;			// 재생율을 지정한다
	d3dpp.PresentationInterval			= D3DPRESENT_INTERVAL_IMMEDIATE;	// 재생 간격을 지정한다 

	// 4단계 장치를 생성한다.

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT,	// 기본 어댑터
		deviceType,			// 장치 타입
		hwnd,				// 장치와 연결된 창
		vp,					// 버텍스 프로세싱 타입
		&d3dpp,				// 시연 인자
		device);			// 생성된 장치

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

			ptr_display(timeDelta);		// 디스플레이 함수를 호출한다.

			lastTime = currTime;
		}
	}
	return msg.wParam;
}