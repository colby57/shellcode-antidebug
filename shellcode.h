#pragma once

#include <cstdint>
#include <Windows.h>
#include "Log.h"

uintptr_t init_shellcode( void* address, void* function )
{
	uint8_t shellcode[] = { 0xFF, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	*reinterpret_cast< void** > ( shellcode + 0x6 ) = function;

	memcpy( address, shellcode, 32 );

	log("[ ! ] shellcode initialized\n");

	return reinterpret_cast< uintptr_t > ( address );
}

uintptr_t msgbox()
{
	MessageBoxA(0, "One will say Easton - the rest will exclaim gay!", 0, 0);
	return 0;
}

uintptr_t peb_shellcode()
{
	bool peb_check = 0;
	
	log("[ ! ] reading peb address..\n");

	__asm
	{
		mov rax, qword ptr gs : [ 0x60 ]
		movzx eax, byte ptr ds : [ rax + 0x2 ]
		mov byte ptr ds : [ peb_check ], al
	}

	if( peb_check == 1 )
	{
		log("\n[ ! ] debugger detected\n");

		const auto allocated_memory = VirtualAlloc( 0, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

		if( allocated_memory )
			log("[ ! ] memory allocated on 0x%" PRIXPTR "\n", reinterpret_cast< uintptr_t > ( allocated_memory ) );

		log("[ ! ] waiting for the filling of the shellcode\n");

		init_shellcode( allocated_memory, msgbox );

		static_cast< uintptr_t( __stdcall * )( ) > ( allocated_memory )( );

		if( VirtualFree( allocated_memory, 0, MEM_RELEASE ) )
		{
			log("[ ! ] memory released\n");
		}

		return peb_check;
	}

	log("[ ! ] all good\n");

	return peb_check;
}