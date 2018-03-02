#include "stdafx.h"

//	global variables
LPDIRECT3D9			g_pd3d = nullptr;			// D3D장치를 생성하는데 사용됨
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
	// IDirect3D9 개체의 인스턴스를 생성한다.
	// 실패 시 null 반환
	if (nullptr == (g_pd3d = Direct3DCreate9(D3D_SDK_VERSION)))
	{
		return E_FAIL;
	}

	// D3DPRESENT_PARAMETERS
	// 프레젠테이션 파라미터에 대한 구조체
	// UINT BackBufferWidth, BackBufferHeight;
	// 새로운 스왑체인의 백 버퍼의 너비와 높이.
	// D3DFORMAT BackBufferFormat;
	// 백 버퍼의 포맷. 자세한 정보는 D3DFORMAT 참조
	// UINT BackBufferCount;
	// 사용 가능한 스왑 이펙트세트에 영향을 준다.
	// D3DMULTISAMPLE_TYPE MultiSampleType;
	// 멀티샘플링은 스왑 이펙트가 D3DSWAPEFFECT_DISCARD인 경우만 지원 된다.
	// DWORD MultiSampleQuality;
	// 품질의 레벨, 0부터 IDirect3D9::CheckDeviceMultiSampleType로 pQualityLevels에 받는 레벨로부터 - 1 까지 크면 에러남
	// D3DSWAPEFFECT SwapEffect;
	// 버퍼 스왑 동작에 관한 설정.
	// HWND hDeviceWindow;
	// 풀 스크린 시 커버 윈도우, 윈도우 모드일 시  IDirect3DDevice9::Present 에 대한 디폴트의 타겟 윈도우
	// BOOL Windowed;
	// 윈도우 모드로 동작할 것인지 풀 스크린 모드로 동작할 것인지 설정
	// BOOL EnableAutoDepthStencil;
	// 깊이 버퍼 사용 여부
	// D3DFORMAT AutoDepthStencilFormat;
	// 자동 스텐실 포맷 EnableAutoDepthStencil가 true 아닐 시 무시됨
	// DWORD Flags;
	// D3DPRESENTFLAG 플래그
	// UINT FullScreen_RefreshRateInHz;
	// 윈도우 모드일 시는 값이 0이어야함
	// UINT PresentationInterval;
	// 어댑터 refresh rate와IDirect3DDevice9::Present 처리를 실행하는 레이트의 관계를 기술한다
	
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	// Creates a device to represent the display adapter.
	if (FAILED(g_pd3d->CreateDevice(
		D3DADAPTER_DEFAULT,						// UINT Adpater			 : 디스플레이 어댑터의 수
		D3DDEVTYPE_HAL,							// D3DDEVTYPE DeviceType : 장치 타입을 나타냄
		hWnd,									// HWND hFocusWindow	 : foreGround에서 backGround로 전환할 때
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,	// DWORD BehaviorFlags	 : 장치 생성을 제어하는 플래그
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

	//	후면 버퍼를 지정한 값으로 지운다.
	g_pd3dDevice->Clear(
		0,
		nullptr,
		D3DCLEAR_TARGET,
		D3DCOLOR_XRGB(0, 0, 255),
		1.0f,
		0
	);

	//	Rendering 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// 렌더링 명령들 작성
		//			.
		//			.
		//			.

		// 렌더링 종료 
		g_pd3dDevice->EndScene();
	}

	//	후면 버퍼를 현재 화면으로 바꿈
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
	//윈도우 클래스 등록
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX),			//	UINT	cbsize			: 구조체의 크기
		CS_HREDRAW | CS_VREDRAW,	//	UINT	style			: 윈도우 클래스의 스타일, 윈도우 그리는 방식? |(or)을 사용하여 플래그 설정 가능
		MsgProc,					//	WNDPROC	lpfnWndProc		: 윈도우 프로시저 함수의 포인터.
		0L,							//	int		cbClsExtra		: 윈도우 클래스 구조체에 할당되는 여분의 바이트 수 (거의 사용 x)
		0L,							//	int		cbWndExtra		: 윈도우 인스턴스 다음에 할당되는 여분의 바이트 수 (거의 사용 x)
		GetModuleHandle(nullptr),	//	HANDLE	hInstance		: 클래스의 윈도우 프로시저를 포함하고 있는 인스턴스의 핸들. 매개 변수로 받은 hInstance를 넣어줌
		nullptr,					//	HICON	hIcon			: 클래스 아이콘의 핸들. alt + tab시 나타내고 싶은 아이콘을 리소스에 대한 핸들로 넘겨주어야 함. LoadIcon();
		nullptr,					//	HCURSOR	hCursor			: 클래스 커서의 핸들.	마우스 커서에 대한 핸들. 아이콘과 동일하게 LoadCursor로 넘김
		nullptr,					//	HBRUSH	hbrBackground	: 클래스 배경브러쉬의 핸들. 간단하게 배경색이라고 생각하면 됨. Direct 사용 시에는 사용할 필요 없어 보임
		nullptr,					//	LPCTSTR	lpszMenuName	: 특정 클래스 메뉴의 리소스 이름에 대한 문자열 포인터. 메뉴 만들때 사용, 메뉴의 이름을 지정
		__T("D3D Tutorial"),		//	LPCTSTR	lpszClassName	: 윈도우 클래스의 이름
		nullptr						//	HICON	hIconSm			: 작업 표시줄과 왼쪽 상단에 나타나는 아이콘의 핸들
	};

	RegisterClassEx(&wc);

	//어플리케이션의 윈도우 창 생성
	HWND hWnd
		= CreateWindowEx(
			NULL,					// DWORD dxExStyle			: 확장 윈도우 스타일
			__T("D3D Tutorial"),	// LPCTSTR lpClassName		: 등록된 윈도우 클래스에 대한 포인터
			__T("00.CreateDevice"),	// LPCTSTR lpWindowName		: 윈도우 이름에 대한 포인터, 왼쪽 상단바 타이틀
			WS_OVERLAPPEDWINDOW,	// DWORD dwStyle			: 윈도우 스타일, OVERLAPPEDWINDOW는 모든 스타일을 포함한다.
			100,					// int	x					: X 방향 위치 값
			100,					// int	y					: Y 방향 위치 값
			500,					// int	nWidth				: 윈도우 너비
			500,					// int	nHeight				: 윈도우 높이
			nullptr,				// HWND	hWndParent			: 부모 윈도우 또는 소유 윈도우의 핸들, null일 시 생성되는 윈도의 부모 윈도우는 데스크탑임을 의미
			nullptr,				// HMENU hMenu				: 메뉴 또는 자식 윈도우 식별자의 핸들, 윈도우 상단에 표시되는 메뉴 핸들
			wc.hInstance,			// HINSTANCE hInstance		: 응용프로그램 인스턴스의 핸들
			nullptr					// LPVIOD lpParam			: 윈도우 생성 데이터에 대한 포인터, MDI(Multiple Document Interface) 윈도우 생성
		);

	// Direct3D 초기화
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// 윈도우 창 띄우기

		//bool ShowIndow(HWND hwnd, int nCmdShow)
		//윈도우 핸들과 윈도우가 어떻게 표시될 지에 대한 플래그를 받음
		ShowWindow(hWnd, SW_SHOWDEFAULT);

		//bool UpdateWindow(HWND hWnd)
		//윈도우의 클라이언트 영역을 그려줌
		UpdateWindow(hWnd);

		// 메세지 루프 진입
		MSG msg;
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			//bool TranslateMessage(const MSG* lpmsg)
			//virtual-key 메시지를 character 메시지로 변환
			//character 메시지는 스레드가 GetMessage 또는 PeekMessage함수를 다음에 호출할 때
			//읽을 수 있도록 호출 스레드의 메시지 대기열에 post된다.
			TranslateMessage(&msg);

			//LRESULT DispatchMessage(const MSG* lpmsg)
			//윈도우 프로시저(WinProc)에 메시지를 전달해준다.
			//Get or Peek 메시지로 검색한 메시지를 전달하는데 사용된다.
			DispatchMessage(&msg);
		}
	}

	//bool UnregisterClass(LPCTSTR lpClassName, HINSTANCE hInstance)
	//등록된 윈도우 클래스를 해제하고 클래스에게 요구되었던 메모리를 확보(해제)한다.
	UnregisterClass(__T("D3D Tutorial"), wc.hInstance);
	return 0;
}