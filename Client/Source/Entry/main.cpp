#include "ClientPCH/ClientPCH.h"
#include "Engine/Core/Engine.h"

// Test
//#include "Engine/Resource/ResourceManager.h"
//#include "Engine/Resource/Texture/Texture.h"
//#include "Engine/Graphics/D3D11/D3D11Renderer.h"
//#include "Engine/Graphics/PSO/GraphicsPSO.h"
//#include "Engine/Graphics/Mesh/SpriteMesh.h"
//#include "Engine/Graphics/ConstantBuffer/SpriteConstantBuffer.h"
//#include "Engine/Graphics/Command/DrawCommand.h"

#include "Engine/Core/World/World.h"
#include "Engine/Core/World/Level.h"
#include "Levels/DemoLevel.h"

#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst; 
HWND g_hMainWindow = nullptr;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void DisableRoundedCorner(HWND hWnd)
{
    enum class DwmWindowCornerPreference 
    {
        Default = 0,
        DoNotRound = 1,
        Round = 2,
        RoundSmall = 3
    };

    DwmWindowCornerPreference pref = DwmWindowCornerPreference::DoNotRound;
    DwmSetWindowAttribute(
        hWnd,
        DWMWA_WINDOW_CORNER_PREFERENCE,
        &pref,
        sizeof(pref)
    );
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    wcscpy_s(szTitle, L"ShyplantsClientWindow");      // 창 제목
    wcscpy_s(szWindowClass, L"ShyplantsClientClass"); // 창 클래스 이름

    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    g_hMainWindow = InitInstance(hInstance, nCmdShow);

    if (!g_hMainWindow)
    {
        return FALSE;
    }

    Engine::Create();
    Engine& engine = Engine::Get();
    
    if (!engine.Init(g_hMainWindow))
    {
        __debugbreak();
        return -1;
    }

    World* world = engine.GetWorld();
    world->LoadLevel(std::make_unique<DemoLevel>(world));

    // Test
    /*D3D11Renderer* renderer = engine.GetRenderer();

    ResourceManager& resourceManager = ResourceManager::Get();
    auto iuTexture = resourceManager.Load<Texture>(L"../Resources/iu.png");

    std::unique_ptr<DrawCommand> drawCommand = std::make_unique<DrawCommand>();

    SpriteMesh* spriteMesh = renderer->GetSpriteMesh();
    GraphicsPSO* pso = renderer->GetPSO(L"SpritePSO");

    drawCommand->PSOHashID = pso->GetHashID();
    drawCommand->sortKey.PSOHashID = pso->GetHashID();
    drawCommand->sortKey.renderLayer = 0;
    drawCommand->sortKey.contentLayer = 0;

    drawCommand->primitiveType = pso->GetPrimitiveType();

    drawCommand->vertexBuffer = spriteMesh->GetVertexBuffer();
    drawCommand->vertexStride = spriteMesh->GetVertexStride();
    drawCommand->vertexOffset = spriteMesh->GetVertexOffset();
    drawCommand->indexBuffer = spriteMesh->GetIndexBuffer();
    drawCommand->indexFormat = DXGI_FORMAT_R16_UINT;
    drawCommand->numIndices = spriteMesh->GetIndexCount();

    drawCommand->spriteConstantBuffer.ScreenRes.x = static_cast<float>(renderer->INL_GetScreenWidth());
    drawCommand->spriteConstantBuffer.ScreenRes.y = static_cast<float>(renderer->INL_GetScreenHeight());
    drawCommand->spriteConstantBuffer.Pos = DirectX::XMFLOAT2(100.0f, 20.0f);
    drawCommand->spriteConstantBuffer.Scale = DirectX::XMFLOAT2(1.0f, 1.0f);
    drawCommand->spriteConstantBuffer.Z = +0.5f;
    drawCommand->spriteConstantBuffer.Alpha = 1.0f;
    drawCommand->spriteConstantBuffer.PivotMode = 0;

    drawCommand->spriteConstantBuffer.TexSize.x = static_cast<float>(iuTexture->GetWidth());
    drawCommand->spriteConstantBuffer.TexSize.y = static_cast<float>(iuTexture->GetHeight());
    drawCommand->spriteConstantBuffer.TexSamplePos.x = 0.0f;
    drawCommand->spriteConstantBuffer.TexSamplePos.y = 0.0f;
    drawCommand->spriteConstantBuffer.TexSampleSize.x = static_cast<float>(iuTexture->GetWidth());
    drawCommand->spriteConstantBuffer.TexSampleSize.y = static_cast<float>(iuTexture->GetHeight());

    drawCommand->VSConstantBuffer = renderer->GetSpriteVSConstantBuffer();
    drawCommand->PSTextureSRV = iuTexture->GetSRV();
    drawCommand->PSSampler = renderer->GetDefaultSamplerState();*/



    MSG msg;

    while (1)
    {
        BOOL bHasMsg = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);

        if(bHasMsg)
        {
            if (msg.message == WM_QUIT)
            {
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Test
            /*std::unique_ptr<DrawCommand> tempCommand = std::make_unique<DrawCommand>(*drawCommand);
            renderer->Submit(std::move(tempCommand));*/

            engine.Run();

            
        }
    }

    engine.Destroy();
    return (int)msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex = {};

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;

    wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    SetProcessDPIAware();

    LONG clientWidth = 1200;
    LONG clientHeight = 900;

    DWORD dwStyle = WS_OVERLAPPEDWINDOW;

    RECT windowRect = { 0, 0, clientWidth, clientHeight };
    AdjustWindowRect(&windowRect, dwStyle, FALSE);


    int windowWidth = windowRect.right - windowRect.left;
    int windowHeight = windowRect.bottom - windowRect.top;


    HWND hWnd = CreateWindowW(szWindowClass, szTitle, dwStyle,
        CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeight, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return nullptr;
    }

    DisableRoundedCorner(hWnd);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return hWnd;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}