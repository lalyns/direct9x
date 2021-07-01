#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <d3dx9.h>
#include <string>

namespace d3d
{
	/* 
	메인 어플리케이션 창을 초기화 하고 Direct3D 초기화 코드를 구현한다.
	함수가 성공적으로 return될 경우 IDirect33DDevice 인터페이스를 가르키는 포인터를 반환한다.
	윈도우의 크기를 지정할 수 있게 해주는 인자와 윈도우 모드나 전체 화면 모드를 선택할 수 있게 해주는 인자가 있음을 확인하자.
	*/
	bool InitD3D(
		HINSTANCE hInstance,				// [Input}  어플리케이션 인스턴스
		int width, int height,				// [Input]  후면 버퍼 크기
		bool windowed,						// [Input]  창모드(true) 혹은 전체화면(false)
		D3DDEVTYPE deviceType,				// [Input]  HAL or REF
		IDirect3DDevice9** device);			// [Output] 만들어진 장치

	/*
	어플리케이션의 메시지 루프를 포장한다. 함수는 디스플레이 함수가 될 함수의 포인터를 매개변수로 받는다.
	*/
	int EnterMsgLoop(
		bool(*ptr_display) (float timeDelta));

	// 메인 어플리케이션 창을 위한 윈도우 프로시저 선언
	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	// COM(Component Object Model) 인터페이스를 해제하고 null로 지정하는 템플릿 함수
	template<class T> void Release(T t)
	{
		if (t) {
			t->Release();
			t = 0;
		}
	}

	// 저장고의 객체를 제거하고 포인터를 null로 지정하는 템플릿 함수
	template<class T> void Delete(T t) 
	{
		if (t) {
			delete t;
			t = 0;
		}
	}
}

#endif // __d3dUtilityH__
