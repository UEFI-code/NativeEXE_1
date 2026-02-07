#include <ntddk.h>
#include "utils.h"

void execute_command(char* command)
{
    if (my_strcmp(command, "HI") == 0)
    {
        PrintString("Hello, Native World!\n");
    }
    else if (my_strcmp(command, "SLEEP") == 0)
    {
        PrintString("Sleeping for 2 seconds...\n");
        native_sleep(2000);
        PrintString("Awake!\n");
    }
    else if (my_strncmp(command, "DIR ", 4) == 0)
    {
        PrintString("Listing DIR: %s\n", command + 4);
        list_dir(command + 4);
    }
    else if (my_strncmp(command, "LSDEV ", 6) == 0)
    {
        PrintString("Listing DEV: %s\n", command + 6);
        list_dev(command + 6);
    }
    else if(my_strcmp(command, "EXIT") == 0)
    {
        PrintString("Exiting...\n");
        RtlExitUserProcess(0);
    }
    else if(my_strcmp(command, "INT 3") == 0)
    {
        PrintString("Triggering breakpoint exception...\n");
        NOP_Toy();
    }
    else
    {
        PrintString("Unknown command: %s\n", command);
    }
}