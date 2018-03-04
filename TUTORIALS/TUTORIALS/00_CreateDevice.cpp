#include "stdafx.h"

//	global variables
LPDIRECT3D9			g_pd3d = nullptr;			// D3D��ġ�� �����ϴµ� ����
LPDIRECT3DDEVICE9	g_pd3dDevice = nullptr;	// Rendering Device

//	InitD3D:	Initalize Driect3D 9 device.
//	CleanUp:	Releases all objects.
//	Render:		Draw the scene.
//	MsgProc:	Window Message handler
//	WinMain:	Main (Application's entry point)

HRESULT InitD3D(HWND hWnd);
VOID CleanUp();
VOID Render();
LRESULT	CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
INT APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT);

HRESULT InitD3D(HWND hWnd)
{
	// IDirect3D9* Direct3DCreate9(UINT SDKVersion)
	// IDirect3D9 ��ü�� �ν��Ͻ��� �����Ѵ�.
	// ���� �� null ��ȯ
	if (nullptr == (g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	// D3DPRESENT_PARAMETERS
	// ���������̼� �Ķ���Ϳ� ���� ����ü
	// UINT BackBufferWidth, BackBufferHeight;
	// ���ο� ����ü���� �� ������ �ʺ�� ����.
	// D3DFORMAT BackBufferFormat;
	// �� ������ ����. �ڼ��� ������ D3DFORMAT ����
	// UINT BackBufferCount;
	// ��� ������ ���� ����Ʈ��Ʈ�� ������ �ش�.
	// D3DMULTISAMPLE_TYPE MultiSampleType;
	// ��Ƽ���ø��� ���� ����Ʈ�� D3DSWAPEFFECT_DISCARD�� ��츸 ���� �ȴ�.
	// DWORD MultiSampleQuality;
	// ǰ���� ����, 0���� IDirect3D9::CheckDeviceMultiSampleType�� pQualityLevels�� �޴� �����κ��� - 1 ���� ũ�� ������
	// D3DSWAPEFFECT SwapEffect;
	// ���� ���� ���ۿ� ���� ����.
	// HWND hDeviceWindow;
	// Ǯ ��ũ�� �� Ŀ�� ������, ������ ����� ��  IDirect3DDevice9::Present �� ���� ����Ʈ�� Ÿ�� ������
	// BOOL Windowed;
	// ������ ���� ������ ������ Ǯ ��ũ�� ���� ������ ������ ����
	// BOOL EnableAutoDepthStencil;
	// ���� ���� ��� ����
	// D3DFORMAT AutoDepthStencilFormat;
	// �ڵ� ���ٽ� ���� EnableAutoDepthStencil�� true �ƴ� �� ���õ�
	// DWORD Flags;
	// D3DPRESENTFLAG �÷���
	// UINT FullScreen_RefreshRateInHz;
	// ������ ����� �ô� ���� 0�̾����
	// UINT PresentationInterval;
	// ����� refresh rate��IDirect3DDevice9::Present ó���� �����ϴ� ����Ʈ�� ���踦 ����Ѵ�
	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Creates a device to represent the display adapter.
	if (FAILED(g_pd3d->CreateDevice(
		D3DADAPTER_DEFAULT,						// UINT Adpater			 : ���÷��� ������� ��
		D3DDEVTYPE_HAL,							// D3DDEVTYPE DeviceType : ��ġ Ÿ���� ��Ÿ��
		hWnd,									// HWND hFocusWindow	 : foreGround���� backGround�� ��ȯ�� ��
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// DWORD BehaviorFlags	 : ��ġ ������ �����ϴ� �÷���
		&d3dpp,									// D3DPRESENT_PARAMETHERS *pPresentationParameters
		&g_pd3dDevice)))						// IDirect3DDevice9 **ppReturnedDeviceInterface
	{
		return E_FAIL;
	}

	return S_OK;
}

VOID CleanUp()
{
	if (nullptr != g_pd3d)
		g_pd3d->Release();

	if (nullptr != g_pd3dDevice)
		g_pd3dDevice->Release();
}

VOID Render()
{
	if (nullptr == g_pd3dDevice)
		return;

	//	�ĸ� ���۸� ������ ������ �����.
	g_pd3dDevice->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0
	);

	//	Rendering ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// ������ ��ɵ� �ۼ�
		//			.
		//			.
		//			.

		// ������ ���� 
		g_pd3dDevice->EndScene();
	}

	//	�ĸ� ���۸� ���� ȭ������ �ٲ�
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

	case WM_PAINT:
		Render();
		ValidateRect(hWnd, nullptr);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

INT APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	//������ Ŭ���� ���
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),			//	UINT	cbsize			: ����ü�� ũ��
		CS_HREDRAW | CS_VREDRAW,	//	UINT	style			: ������ Ŭ������ ��Ÿ��, ������ �׸��� ���? |(or)�� ����Ͽ� �÷��� ���� ����
		MsgProc,					//	WNDPROC	lpfnWndProc		: ������ ���ν��� �Լ��� ������.
		0L,							//	int		cbClsExtra		: ������ Ŭ���� ����ü�� �Ҵ�Ǵ� ������ ����Ʈ �� (���� ��� x)
		0L,							//	int		cbWndExtra		: ������ �ν��Ͻ� ������ �Ҵ�Ǵ� ������ ����Ʈ �� (���� ��� x)
		GetModuleHandle(nullptr),	//	HANDLE	hInstance		: Ŭ������ ������ ���ν����� �����ϰ� �ִ� �ν��Ͻ��� �ڵ�. �Ű� ������ ���� hInstance�� �־���
		nullptr,					//	HICON	hIcon			: Ŭ���� �������� �ڵ�. alt + tab�� ��Ÿ���� ���� �������� ���ҽ��� ���� �ڵ�� �Ѱ��־�� ��. LoadIcon();
		nullptr,					//	HCURSOR	hCursor			: Ŭ���� Ŀ���� �ڵ�.	���콺 Ŀ���� ���� �ڵ�. �����ܰ� �����ϰ� LoadCursor�� �ѱ�
		nullptr,					//	HBRUSH	hbrBackground	: Ŭ���� ���귯���� �ڵ�. �����ϰ� �����̶�� �����ϸ� ��. Direct ��� �ÿ��� ����� �ʿ� ���� ����
		nullptr,					//	LPCTSTR	lpszMenuName	: Ư�� Ŭ���� �޴��� ���ҽ� �̸��� ���� ���ڿ� ������. �޴� ���鶧 ���, �޴��� �̸��� ����
		__T("D3D Tutorial"),		//	LPCTSTR	lpszClassName	: ������ Ŭ������ �̸�
		nullptr						//	HICON	hIconSm			: �۾� ǥ���ٰ� ���� ��ܿ� ��Ÿ���� �������� �ڵ�
	};

	RegisterClassEx(&wc);

	//���ø����̼��� ������ â ����
	HWND hWnd
		= CreateWindowEx(
			NULL,					// DWORD dxExStyle			: Ȯ�� ������ ��Ÿ��
			__T("D3D Tutorial"),	// LPCTSTR lpClassName		: ��ϵ� ������ Ŭ������ ���� ������
			__T("00.CreateDevice"),	// LPCTSTR lpWindowName		: ������ �̸��� ���� ������, ���� ��ܹ� Ÿ��Ʋ
			WS_OVERLAPPEDWINDOW,	// DWORD dwStyle			: ������ ��Ÿ��, OVERLAPPEDWINDOW�� ��� ��Ÿ���� �����Ѵ�.
			100,					// int	x					: X ���� ��ġ ��
			100,					// int	y					: Y ���� ��ġ ��
			500,					// int	nWidth				: ������ �ʺ�
			500,					// int	nHeight				: ������ ����
			nullptr,				// HWND	hWndParent			: �θ� ������ �Ǵ� ���� �������� �ڵ�, null�� �� �����Ǵ� ������ �θ� ������� ����ũž���� �ǹ�
			nullptr,				// HMENU hMenu				: �޴� �Ǵ� �ڽ� ������ �ĺ����� �ڵ�, ������ ��ܿ� ǥ�õǴ� �޴� �ڵ�
			wc.hInstance,			// HINSTANCE hInstance		: �������α׷� �ν��Ͻ��� �ڵ�
			nullptr					// LPVIOD lpParam			: ������ ���� �����Ϳ� ���� ������, MDI(Multiple Document Interface) ������ ����
		);

	// Direct3D �ʱ�ȭ
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// ������ â ����

		//bool ShowIndow(HWND hwnd, int nCmdShow)
		//������ �ڵ�� �����찡 ��� ǥ�õ� ���� ���� �÷��׸� ����
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		//bool UpdateWindow(HWND hWnd)
		//�������� Ŭ���̾�Ʈ ������ �׷���
		UpdateWindow(hWnd);

		// �޼��� ���� ����
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			//bool TranslateMessage(const MSG* lpmsg)
			//virtual-key �޽����� character �޽����� ��ȯ
			//character �޽����� �����尡 GetMessage �Ǵ� PeekMessage�Լ��� ������ ȣ���� ��
			//���� �� �ֵ��� ȣ�� �������� �޽��� ��⿭�� post�ȴ�.
			TranslateMessage(&msg);

			//LRESULT DispatchMessage(const MSG* lpmsg)
			//������ ���ν���(WinProc)�� �޽����� �������ش�.
			//Get or Peek �޽����� �˻��� �޽����� �����ϴµ� ���ȴ�.
			DispatchMessage(&msg);
		}
	}

	//bool UnregisterClass(LPCTSTR lpClassName, HINSTANCE hInstance)
	//��ϵ� ������ Ŭ������ �����ϰ� Ŭ�������� �䱸�Ǿ��� �޸𸮸� Ȯ��(����)�Ѵ�.
	UnregisterClass(__T("D3D Tutorial"), wc.hInstance);
	return 0;
}