#include <windows.h>
#include <stdio.h>

#include <string>
#include <map>
#include <iostream>
#include <fstream>

#include "thirdparty\json\json.hpp"
#include "thirdparty\minhook\minhook.h"

#pragma comment(lib, "libminhook-x86-v141-mdd.lib")

#define RTA_HANDLE_PACKET										0x0032C980

using json = nlohmann::json;

typedef uint8_t(__cdecl* ft_handle_packet)(uint32_t payload_address);

static ft_handle_packet fp_handle_packet = nullptr;

static std::map<uint32_t, uint32_t> skin_transforms = std::map<uint32_t, uint32_t>();

static json quickskin_configuration;

uint8_t __cdecl hf_handle_packet(uint32_t payload_address)
{
	if (*((uint16_t*) (payload_address + 4)) == 0x39)
	{
		std::cout << "Received `PKT_S2C_CreateHero_s`." << std::endl;

		uint32_t name_address = (payload_address + 0x1A);

		if (*((uint32_t*) (payload_address + 0x2E)) >= 16)
			name_address = *((uint32_t*) (payload_address + 0x1A));

		const char* name_value = ((const char*) name_address);

		std::string name(name_value);

		for (json::iterator it = quickskin_configuration.begin(); it != quickskin_configuration.end(); ++it)
		{
			std::string element_name = it.key();

			element_name.erase(std::remove(element_name.begin(), element_name.end(), ' '), element_name.end());

			std::cout << "element_name: `" << element_name << "` (" << element_name.size() << ")." << std::endl;
			std::cout << "name: `" << name << "` (" << name.size() << ")." << std::endl;

			if (element_name == name)
			{
				std::cout << "Found matching configuration entry: " << element_name << "." << std::endl;

				uint32_t skin_id = it.value();
				uint32_t skin_value = skin_transforms[skin_id];

				std::cout << "Modifying Skin ID " << skin_id << " with Skin Value " << skin_value << "." << std::endl;

				*((uint32_t*) (payload_address + 0x50)) = skin_value;

				break;
			}
		}
	}

	return fp_handle_packet(payload_address);
}

void initialize()
{
	skin_transforms.emplace(0, 0xBCBCBCBC);
	skin_transforms.emplace(1, 0xBCBCBC3C);
	skin_transforms.emplace(2, 0xBCBCBCBB);
	skin_transforms.emplace(3, 0xBCBCBC3B);
	skin_transforms.emplace(4, 0xBCBCBC8C);
	skin_transforms.emplace(5, 0xBCBCBC0C);
	skin_transforms.emplace(6, 0xBCBCBC8B);
	skin_transforms.emplace(7, 0xBCBCBC0B);
	skin_transforms.emplace(8, 0xBCBCBC7C);
	skin_transforms.emplace(9, 0xBCBCBCFC);
	skin_transforms.emplace(10, 0xBCBCBC7B);
	skin_transforms.emplace(11, 0xBCBCBCFB);
	skin_transforms.emplace(12, 0xBCBCBC4C);
	skin_transforms.emplace(13, 0xBCBCBCCC);
	skin_transforms.emplace(14, 0xBCBCBC4B);

	FILE* console_stream = nullptr;

	::AllocConsole();
	::SetConsoleTitle("QuickSkin Debug Console");

	::freopen_s(&console_stream, "conout$", "w", stdout);

	std::cout << "Hello, world!" << std::endl;

	uint32_t base_address = ((uint32_t) GetModuleHandle(nullptr));
	uint32_t ata_handle_packet = (base_address + RTA_HANDLE_PACKET);

	std::cout << "Base Address: 0x" << std::hex << base_address << std::dec << ", Handle Packet Address: 0x" << std::hex << ata_handle_packet << std::dec << "." << std::endl;

	std::ifstream quickskin_configuration_file("quickskin.json");

	if (quickskin_configuration_file.good())
	{
		quickskin_configuration_file >> quickskin_configuration;

		std::cout << "Configuration was successfully loaded from disk." << std::endl;
	}
	else
	{
		std::cout << "Couldn't find `quickskin.json` configuration file." << std::endl;

		return;
	}
		
	if (::MH_Initialize() != MH_OK)
	{
		std::cout << "Failed to initialize hook library." << std::endl;

		return;
	}

	if (::MH_CreateHook((void*) ata_handle_packet, (void*) &hf_handle_packet, reinterpret_cast<void**>(&fp_handle_packet)) != MH_OK)
	{
		std::cout << "Failed to create target function hook." << std::endl;

		return;
	}

	if (::MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		std::cout << "Failed to enable target function hook." << std::endl;

		return;
	}
}

__declspec(dllexport) BOOL APIENTRY DllMain(HMODULE module, DWORD reason, void* reserved)
{
	if (reason != DLL_PROCESS_ATTACH)
		return FALSE;

	::DisableThreadLibraryCalls(module);

	initialize();

	return TRUE;
}