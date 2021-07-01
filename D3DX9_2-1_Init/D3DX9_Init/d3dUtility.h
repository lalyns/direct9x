#ifndef __d3dUtilityH__
#define __d3dUtilityH__

#include <d3dx9.h>
#include <string>

namespace d3d
{
	/* 
	���� ���ø����̼� â�� �ʱ�ȭ �ϰ� Direct3D �ʱ�ȭ �ڵ带 �����Ѵ�.
	�Լ��� ���������� return�� ��� IDirect33DDevice �������̽��� ����Ű�� �����͸� ��ȯ�Ѵ�.
	�������� ũ�⸦ ������ �� �ְ� ���ִ� ���ڿ� ������ ��峪 ��ü ȭ�� ��带 ������ �� �ְ� ���ִ� ���ڰ� ������ Ȯ������.
	*/
	bool InitD3D(
		HINSTANCE hInstance,				// [Input}  ���ø����̼� �ν��Ͻ�
		int width, int height,				// [Input]  �ĸ� ���� ũ��
		bool windowed,						// [Input]  â���(true) Ȥ�� ��üȭ��(false)
		D3DDEVTYPE deviceType,				// [Input]  HAL or REF
		IDirect3DDevice9** device);			// [Output] ������� ��ġ

	/*
	���ø����̼��� �޽��� ������ �����Ѵ�. �Լ��� ���÷��� �Լ��� �� �Լ��� �����͸� �Ű������� �޴´�.
	*/
	int EnterMsgLoop(
		bool(*ptr_display) (float timeDelta));

	// ���� ���ø����̼� â�� ���� ������ ���ν��� ����
	LRESULT CALLBACK WndProc(
		HWND hwnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);

	// COM(Component Object Model) �������̽��� �����ϰ� null�� �����ϴ� ���ø� �Լ�
	template<class T> void Release(T t)
	{
		if (t) {
			t->Release();
			t = 0;
		}
	}

	// ������� ��ü�� �����ϰ� �����͸� null�� �����ϴ� ���ø� �Լ�
	template<class T> void Delete(T t) 
	{
		if (t) {
			delete t;
			t = 0;
		}
	}
}

#endif // __d3dUtilityH__
