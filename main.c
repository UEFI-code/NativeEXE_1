#include <ntddk.h>

void NOP_Toy(void);

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);
	while (1)
	{
		NOP_Toy();
	}
	
}