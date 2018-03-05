#include "stdafx.h"

LPDIRECT3D9 g_pd3d = nullptr;
LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
LPDIRECT3DVERTEXBUFFER9 g_pVB = nullptr;

HRESULT InitD3D(HWND hWnd);
HRESULT InitVB();
VOID CleanUp();
VOID SetUpMatrices();
VOID SetUpLIghts();
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

	// 추가된 코드
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

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

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);

	return S_OK;
}

HRESULT InitVB()
{
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(
		50 * 2 * sizeof(MY3DVERTEX),
		0,
		D3DFVF_XYZ | D3DFVF_NORMAL,
		D3DPOOL_DEFAULT,
		&g_pVB,
		nullptr
	)))
	{
		return E_FAIL;
	}

	MY3DVERTEX* pVertices;

	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}


	// 위 아래가 뚫린 원기둥 (실린더) 생성
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
		pVertices[2 * 1 + 0].position = D3DXVECTOR3(sinf(theta), -1.f, cosf(theta));
		pVertices[2 * 1 + 0].normal = D3DXVECTOR3(sinf(theta), 0.f, cosf(theta));
		pVertices[2 * 1 + 1].position = D3DXVECTOR3(sinf(theta), 1.f, cosf(theta));
		pVertices[2 * 1 + 1].normal = D3DXVECTOR3(sinf(theta), 0.f, cosf(theta));
	}
	
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
	// Set World Matrix
	D3DXMATRIXA16 matWorld;

	// 단위 행렬
	D3DXMatrixIdentity(&matWorld);
	// X축 회전
	D3DXMatrixRotationX(&matWorld, GetTickCount64() / 500.f);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set View Matrix
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// Set Projection View
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetUpLights()
{
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 0.f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.f;

	g_pd3dDevice->SetMaterial(&mtrl);

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 1.f;
	light.Diffuse.g = 1.f;
	light.Diffuse.b = 1.f;

	vecDir = D3DXVECTOR3(
		cosf(GetTickCount64() / 350.f),
		1.f,
		sinf(GetTickCount64() / 350.f));

	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 1000.f;
	g_pd3dDevice->SetLight(0, &light);
	g_pd3dDevice->LightEnable(0, true);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);
}

VOID Render()
{
	g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.f, 0);

	// BeginScene과 EndScene 사이는 최대한 짧은 것이 좋다.
	SetUpLights();
	SetUpMatrices();

	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(MY3DVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2*50 - 2);

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

	HWND hWnd = CreateWindow(__T("D3D Tutorial"), __T("03.Matrices"),
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