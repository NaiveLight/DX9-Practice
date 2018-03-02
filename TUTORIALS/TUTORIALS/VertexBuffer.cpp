#include "stdafx.h"

// global variables
LPDIRECT3D9				g_pd3d;
LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3DVERTEXBUFFER9 g_pVB;

// InitVB : Creates a vertex buffer and fills it with our vertices.

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
		g_pd3dDevice->SetStreamSource(
			0,
			g_pVB,
			0,
			sizeof(MYVERTEX)
		);

		g_pd3dDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

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


