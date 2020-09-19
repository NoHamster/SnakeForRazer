#pragma once
#include "RzChromaSDKDefines.h"
#include "RzChromaSDKTypes.h"
#include "RzErrors.h"
#include <windows.h>


#ifndef DLL_COMPILED
#define DLL_INTERNAL __declspec( dllexport )
#endif 


// Define all Colours you want
const COLORREF BLACK = RGB(0, 0, 0);
const COLORREF WHITE = RGB(255, 255, 255);
const COLORREF RED = RGB(255, 0, 0);
const COLORREF GREEN = RGB(0, 255, 0);
const COLORREF BLUE = RGB(0, 0, 255);
const COLORREF YELLOW = RGB(255, 255, 0);
const COLORREF PURPLE = RGB(128, 0, 128);
const COLORREF CYAN = RGB(00, 255, 255);
const COLORREF ORANGE = RGB(255, 165, 00);
const COLORREF PINK = RGB(255, 192, 203);
const COLORREF GREY = RGB(125, 125, 125);
//You dont have to define your colors as COLORREFs, just use the RGB(xxx,xxx,xxx) function like above

#define ALL_DEVICES         0
#define KEYBOARD_DEVICES    1


typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

//Class of your Chroma Implementation
class Chromascreen
{
public:
	Chromascreen();
	~Chromascreen();
	BOOL Initialize();


	void ResetEffects();
	//Define your methods here
	BOOL setKeyboard(COLORREF screen[][ChromaSDK::Keyboard::MAX_COLUMN], int size_X, int size_Y);


	BOOL IsDeviceConnected(RZDEVICEID DeviceId);

private:
	HMODULE m_ChromaSDKModule;

};

