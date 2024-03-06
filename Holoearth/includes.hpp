#pragma once
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <d3d11.h>
#include <dxgi.h>
#include <MinHook.h>
#include <format>
#include "3rdparty/ImGui/imconfig.h"
#include "3rdparty/ImGui/imgui_impl_dx11.h"
#include "3rdparty/ImGui/imgui.h"
#include "3rdparty/ImGui/imgui_internal.h"
#include "3rdparty/ImGui/imstb_rectpack.h"
#include "3rdparty/ImGui/imstb_textedit.h"
#include "3rdparty/ImGui/imstb_truetype.h"
#include "3rdparty/kiero/kiero.h"
#include "3rdparty/ImGui/imgui_impl_win32.h"
#include "nimbussansfont.hpp"
#include "3rdparty/UnityResolve.hpp"
using I = UnityResolve;
using IM = UnityResolve::Method;
using IC = UnityResolve::Class;
using IT = UnityResolve::Type;
using IF = UnityResolve::Field;
using IA = UnityResolve::Assembly;
using II = UnityResolve::UnityType;
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "minhook.x64.lib")
typedef UINT64 QWORD;
#include "3rdparty/console-color.hpp"
using namespace std;
typedef HRESULT(__stdcall* Present) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef LRESULT(CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef uintptr_t PTR;
Present oPresent;
using namespace std;
// Declare internal and external function for handling Win32 window messages
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
bool init = false;
HWND holoearth = FindWindowA(0, "Holoearth");
using namespace cs;
void InitUnity_IL2Cpp() {
	I::Init(GetModuleHandle((LPCSTR)"GameAssembly.dll"), I::Mode::Il2Cpp);
	I::ThreadAttach();
}
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

void InitImgui() {
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	io.Fonts->AddFontFromMemoryTTF(nimbussans, sizeof(nimbussans), 18.F);
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}
void EndAndRender() {
	ImGui::End();
	ImGui::Render();
}
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer{ 0 };
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView); //False Positive Warning DirectX 11 SDK(Idk Why)
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImgui();
			cout << (("D3D11 Device is Founded Successfully"_cs << color(0x7F8224))) << endl;
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}
	ImGuiWindowFlags fl = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Holoearth Cheat by RiritoNinigaya", 0, fl);
	ImGui::SetWindowSize(ImVec2(450, 600), 0);
	if (ImGui::BeginMenu("FoV Changer")) 
	{
		static float max_v = 0.0f;
		ImGui::SliderFloat("Value Change", &max_v, 0.F, 400.F, "%.3f");
		if (ImGui::Button("Set FoV")) 
		{
			auto instance = UnityResolve::UnityType::Camera::GetMain();
			if (!instance) {
				MessageBoxA(0, "Camera Not Founded", "Holoearth", MB_OK | MB_ICONERROR);
			}
			instance->SetFoV(max_v);
			std::string m = std::format("{}", instance->GetName());
			const char* UnityCameraMainName = m.c_str();
			cs::color_string str(UnityCameraMainName);
			cs::color_string color_str(str);
			color_str.use(color(155, 45, 50));
			cout << "Name Of Camera: " << color_str << endl;
		}
		ImGui::EndMenu();
	}
	EndAndRender();
	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}
	