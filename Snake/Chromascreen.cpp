#include "Chromascreen.h"
#include <tchar.h>

#ifdef _WIN64
#define CHROMASDKDLL        _T("RzChromaSDK64.dll")
#else
#define CHROMASDKDLL        _T("RzChromaSDK.dll")
#endif
using namespace ChromaSDK;
using namespace ChromaSDK::Keyboard;
using namespace std;

typedef RZRESULT(*INIT)(void);
typedef RZRESULT(*UNINIT)(void);
typedef RZRESULT(*CREATEEFFECT)(RZDEVICEID DeviceId, ChromaSDK::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*CREATEKEYBOARDEFFECT)(ChromaSDK::Keyboard::EFFECT_TYPE Effect, PRZPARAM pParam, RZEFFECTID* pEffectId);
typedef RZRESULT(*SETEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*DELETEEFFECT)(RZEFFECTID EffectId);
typedef RZRESULT(*REGISTEREVENTNOTIFICATION)(HWND hWnd);
typedef RZRESULT(*UNREGISTEREVENTNOTIFICATION)(void);
typedef RZRESULT(*QUERYDEVICE)(RZDEVICEID DeviceId, ChromaSDK::DEVICE_INFO_TYPE& DeviceInfo);

INIT Init = nullptr;
UNINIT UnInit = nullptr;
CREATEEFFECT CreateEffect = nullptr;
CREATEKEYBOARDEFFECT CreateKeyboardEffect = nullptr;
SETEFFECT SetEffect = nullptr;
DELETEEFFECT DeleteEffect = nullptr;
QUERYDEVICE QueryDevice = nullptr;



BOOL Chromascreen::IsDeviceConnected(RZDEVICEID DeviceId)
{
	if (QueryDevice != nullptr)
	{
		ChromaSDK::DEVICE_INFO_TYPE DeviceInfo = {};
		auto Result = QueryDevice(DeviceId, DeviceInfo);

		return DeviceInfo.Connected;
	}

	return FALSE;
}


Chromascreen::Chromascreen() :m_ChromaSDKModule(nullptr)
{
}
Chromascreen::~Chromascreen()
{
}

BOOL Chromascreen::Initialize()
{
	if (m_ChromaSDKModule == nullptr)
	{
		m_ChromaSDKModule = LoadLibrary(CHROMASDKDLL);
		if (m_ChromaSDKModule == nullptr)
		{
			return FALSE;
		}
	}

	if (Init == nullptr)
	{
		auto Result = RZRESULT_INVALID;
		Init = reinterpret_cast<INIT>(GetProcAddress(m_ChromaSDKModule, "Init"));
		if (Init)
		{
			Result = Init();
			if (Result == RZRESULT_SUCCESS)
			{
				CreateEffect = reinterpret_cast<CREATEEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateEffect"));
				CreateKeyboardEffect = reinterpret_cast<CREATEKEYBOARDEFFECT>(GetProcAddress(m_ChromaSDKModule, "CreateKeyboardEffect"));
				SetEffect = reinterpret_cast<SETEFFECT>(GetProcAddress(m_ChromaSDKModule, "SetEffect"));
				DeleteEffect = reinterpret_cast<DELETEEFFECT>(GetProcAddress(m_ChromaSDKModule, "DeleteEffect"));
				QueryDevice = reinterpret_cast<QUERYDEVICE>(GetProcAddress(m_ChromaSDKModule, "QueryDevice"));

				if (CreateEffect &&
					CreateKeyboardEffect &&
					SetEffect &&
					DeleteEffect &&
					QueryDevice)
				{
					return TRUE;
				}
				else
				{
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

void Chromascreen::ResetEffects()
{
	if (CreateKeyboardEffect)
	{
		CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_NONE, nullptr, nullptr);
	}
}


BOOL Chromascreen::setKeyboard(COLORREF screen[][ChromaSDK::Keyboard::MAX_COLUMN], int size_X, int size_Y) {

	ChromaSDK::Keyboard::CUSTOM_EFFECT_TYPE Keyboard = {};

	for (int row = 0; row < size_Y; row++) {
		for (int col = 0; col < size_X; col++) {
			Keyboard.Color[col][row] = screen[col][row];
		}

	}

	RZRESULT Result_Keyboard = CreateKeyboardEffect(ChromaSDK::Keyboard::CHROMA_CUSTOM, &Keyboard, nullptr);


	return Result_Keyboard;
	
	return FALSE;
}

