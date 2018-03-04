#include "stdafx.h"

// global variables
LPDIRECT3D9				g_pd3d;
LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3DVERTEXBUFFER9 g_pVB;

// InitVB : Creates a vertex buffer and fills it with our vertices.
//			Must use lock & unlock, when write our vertices to vertex buffer.
//			D3D can also use Index buffer.

HRESULT InitD3D(HWND hWnd);
HRESULT InitVB();
VOID CleanUp();
VOID Render();
LRESULT	CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT);

HRESULT InitD3D(HWND hWnd)
{
	if (nullptr == (g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	if (FAILED(g_pd3d->CreateDevice(
		D3DADAPTER_DEFAULT,						
		D3DDEVTYPE_HAL,							
		hWnd,									
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	
		&d3dpp,									
		&g_pd3dDevice)))						
	{
		return E_FAIL;
	}

	return S_OK;
}

HRESULT InitVB()
{
	MYVERTEX vertices[] =
	{
		//	x, y, z, rhw, color
		{ 50.f, 50.f, 0.5f, 1.f, 0xffff0000},
		{ 250.f, 50.f, 0.5f, 1.f, 0xff00ff00 },
		{ 250.f, 250.f, 0.5f, 1.f, 0xff00ff00 },
	};

	//HRESULT CreateVertexBuffer(
	//	UINT Length,								// 버텍스 버퍼의 크기
	//	DWORD Usage,								// 버텍스 버퍼의 종류 혹은 처리방식 지정, no usage value. 만일 사용할 시 D3DUSAGE 상수 사용 
	//	DWORD FVF,									// 버텍스 구조체에 따라 선언된 FVF 플래그 값
	//	D3DPOOL Pool,								// 정점 버퍼가 저장될 메모리 위치와 관리방식 지정
	//	IDirect3DVertexBuffer9 **ppVertexBuffer,	// 
	//	HANDLE *pSharedHandle						//
	//)

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
		3 * sizeof(MYVERTEX),
		0,
		D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		D3DPOOL_DEFAULT,
		&g_pVB,
		nullptr
	)))
	{
		return E_FAIL;
	}

	VOID* pVertices;

	//HRESULT Lock(
	//	UINT OffsetToLock,	// lock을 걸 버퍼 대한 offset, 버텍스 버퍼 전체를 잠그려면 sizetolock과 offsettolock을 둘다 0으로 설정
	//	UINT SizeToLock,	// lock을 걸 버퍼 크기 Bytes
	//	VOID **ppbData,		// 반환된 정점 데이터를 가지고 있을 버퍼에 대한 포인터
	//	DWORD Flags			// lock 유형에 대한 플래그
	//)

	if (FAILED(g_pVB->Lock(
		0,
		sizeof(vertices),
		(void**)&pVertices,
		0
	)))
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

VOID CleanUp()
{
	if (nullptr != g_pVB)
		g_pVB->Release();

	if (nullptr != g_pd3d)
		g_pd3d->Release();

	if (nullptr != g_pd3dDevice)
		g_pd3dDevice->Release();
}

VOID Render()
{
	g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{

		//HRESULT SetStreamSource(

		//	UINT StreamNumber,					// 0 로부터 스트림의 최대수 -1 까지의 범위의 데이터 스트림을 지정
		//	IDirect3DVertexBuffer9 *pStreamData,// 바인딩할 버텍스 버퍼
		//	UINT OffsetInBytes,					// 스트림의 처음부터 버텍스 데이터 시작 부분까지의 바이트 단위 오프셋
		//	UINT Stride							// 컴포넌트의 폭
		//);

		g_pd3dDevice->SetStreamSource(
			0,
			g_pVB,
			0,
			sizeof(MYVERTEX)
		);

		// 버텍스 정보 설정
		g_pd3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

		// 그리기 IndexedPrimitive 알아봐야함.
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		g_pd3dDevice->EndScene();
	}

	g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

LRESULT	CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		CleanUp();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
		__T("D3D Tutorial"), nullptr
	};
	RegisterClassEx(&wc);

	HWND hWnd = CreateWindow(__T("D3D Tutorial"), __T("01.VertexBuffer"),
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		nullptr, nullptr, wc.hInstance, nullptr);

	if (SUCCEEDED(InitD3D(hWnd)))
	{
		if (SUCCEEDED(InitVB()))
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			MSG msg;
			ZeroMemory(&msg, sizeof(msg));

			while (msg.message != WM_QUIT)
			{
				//	GetMessage에서 PeekMessage로 변환된 이유
				//	GetMessage는 새로운 메시지가 발생하기 전까지 대기 상태로 전환되므로 이후 문장들 실행 X
				//	PeekMessage는 메시지가 있으면 처리하고 없으면 다음 문장 실행
				if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass(__T("D3D Tutorial"), wc.hInstance);

	return 0;
}


