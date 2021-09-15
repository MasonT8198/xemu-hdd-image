#include <winapi/windows.h>
#include <hal/debug.h>
#include <hal/video.h>
#include <hal/xbox.h>
#include <xboxkrnl/xboxkrnl.h>
#include <nxdk/mount.h>

#define TrayStateHasMedia 0x60

int main(void)
{
    XVideoSetMode(640, 480, 16, REFRESH_DEFAULT);

    if (!nxMountDrive('A', "\\Device\\CdRom0\\")) {
        debugPrint("Unable to mount CdRom!\n");
        Sleep(1000);
        return 1;
    }
    
    debugPrint("Please insert a disc...\n");

    ULONG state;
    ULONG oldstate;

    while (1) {
        XVideoWaitForVBlank();

        HalReadSMCTrayState(&state, NULL);
        
        if (state == TrayStateHasMedia) {
            if ((GetFileAttributesA("A:\\default.xbe")) != INVALID_FILE_ATTRIBUTES) {
                debugPrint("Launching...\n");
                XLaunchXBE("\\Device\\CdRom0\\default.xbe");
            }
            else
            {
                while (oldstate != state)
                {
                    debugPrint("Please double check if you've packed your ISO correctly.\n");
                    debugPrint("REDUMP isos do not work in Xemu at the moment.\nPlease avoid using them.\n");
                    oldstate = state;
                }
            }
        }


    }

    return 1;
}
