#include <cinttypes>
#include <iostream>

#include "shellcode.h"

int main()
{
	LoadLibraryA("user32.dll");
	
	const auto allocated_memory = VirtualAlloc( 0, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );

	if( allocated_memory )
		log("[ ! ] memory allocated on 0x%" PRIXPTR "\n", reinterpret_cast< uintptr_t > ( allocated_memory ) );
	
    	log("[ ! ] waiting for the filling of the shellcode\n");

	init_shellcode( allocated_memory, peb_shellcode );

	static_cast< uintptr_t( __stdcall * )( ) > ( allocated_memory )( );

	if( VirtualFree( allocated_memory, 0, MEM_RELEASE ) )
	{
		log("[ ! ] memory released\n");
	}

	getchar();

	return 0;
}
