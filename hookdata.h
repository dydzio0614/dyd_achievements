//PLAYER_DIE

__declspec(naked) void player_die_patchdata()
{
	__asm
	{
		push player_die
		ret
	}
}

__declspec(naked) void player_die_entry()
{
	__asm //original func data
	{
		push ebp
		mov ebp, esp
			sub esp, 192
	}

	execute_address(0x200d15f9); //original player_die + offset
}
