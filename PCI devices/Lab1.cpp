// Lab1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#pragma comment (lib, "Setupapi.lib")

using namespace std;

void printfVendor(string temp);
void printfDeviceID(string temp);

int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HDEVINFO hDevInfo;		//A handle to a device information set
	SP_DEVINFO_DATA deviceInfoData;		//structure defines a device instance that is a member of a device information set.
	deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

	//returns a handle to a device information set
	if ((hDevInfo = SetupDiGetClassDevs(NULL, REGSTR_KEY_PCIENUM, 0, DIGCF_ALLCLASSES)) == INVALID_HANDLE_VALUE)
	{
		exit(1);
	}

	//The SetupDiEnumDeviceInfo function returns a SP_DEVINFO_DATA structure that specifies a device information element in a device information set.
	for (DWORD i = 0; (SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData)); i++)		
	{
		DWORD data;
		LPTSTR buffer = NULL;
		DWORD bufferSize = 0;

		//The SetupDiGetDeviceRegistryProperty function retrieves a specified Plug and Play device property.
		while (!SetupDiGetDeviceRegistryProperty(hDevInfo,		
			&deviceInfoData,
			SPDRP_DEVICEDESC,		//return device name
			&data,
			(PBYTE)buffer,
			bufferSize,
			&bufferSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)		//The data area passed to a system call is too small.
			{
				if (buffer)
				{
					LocalFree(buffer);
				}
				buffer = (LPTSTR)LocalAlloc(LPTR, bufferSize * 2);
			}
			else
			{
				break;
			}
		}

		printf("%d. %- 80s", i + 1, buffer);

		while (!SetupDiGetDeviceRegistryProperty(hDevInfo,		
			&deviceInfoData,
			SPDRP_HARDWAREID,		//return deviceID and vendorID name
			&data,
			(PBYTE)buffer,
			bufferSize,
			&bufferSize))
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)		//The data area passed to a system call is too small.
			{
				if (buffer)
				{
					LocalFree(buffer);
				}
				buffer = (LPTSTR)LocalAlloc(LPTR, bufferSize * 2);
			}
			else
			{
				break;
			}
		}
		printfVendor(string(buffer));
		printfDeviceID(string(buffer));
		printf("\n");

		if (buffer)
		{
			LocalFree(buffer);
		}
	}

	printf("\n");

	SetupDiDestroyDeviceInfoList(hDevInfo);
	system("pause");
	return 0;
}

void printfVendor(string temp)
{
	int venPos = temp.find("VEN_");

	//parser
	printf("\tVendorID = ");
	for (int j = 4; j < 8; j++)
	{
		printf("%c", temp.at(venPos + j));
	}
}

void printfDeviceID(string temp)
{
	int devPos = temp.find("DEV_");

	//parser
	printf("\tDeviceID = ");
	for (int j = 4; j < 8; j++)
	{
		printf("%c", temp.at(devPos + j));
	}
}



