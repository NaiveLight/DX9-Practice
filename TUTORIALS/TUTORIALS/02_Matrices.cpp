#include "stdafx.h"

// global variables
LPDIRECT3D9				g_pd3d;
LPDIRECT3DDEVICE9		g_pd3dDevice;
LPDIRECT3DVERTEXBUFFER9 g_pVB;

HRESULT InitD3D(HWND hWnd);
HRESULT InitVB();
VOID CleanUp();
VOID SetUpMatrices();
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

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	return S_OK;
}

HRESULT InitVB()
{
	MYGEOVERTEX vertices[] =
	{
		//	x, y, z, color
		{ -1.0f,-1.0f, 0.0f, 0xffff0000, },
		{ 1.0f,-1.0f, 0.0f, 0xff0000ff, },
		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
	};

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
		3 * sizeof(MYGEOVERTEX),
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

	if (FAILED(g_pVB->Lock(	0, sizeof(vertices), (void**)&pVertices, 0 ) ) )
	{
		return E_FAIL;
	}

	memcpy(pVertices, vertices, sizeof(vertices));

	g_pVB->Unlock();

	return S_OK;
}

VOID CleanUp()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pd3d != NULL)
		g_pd3d->Release();
}

VOID SetUpMatrices()
{
	D3DXMATRIXA16 matWorld;

	ULONGLONG iTime = GetTickCount64() % 1000;
	//UINT iTime = timeGetTime() % 1000;

	FLOAT fAngle = iTime * (1.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixRotationY(&matWorld, fAngle);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID Render()
{
	g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		SetUpMatrices();

		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(MYGEOVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

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

	HWND hWnd = CreateWindow(__T("D3D Tutorial"), __T("02.Matrices"),
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

