#include <efi.h>
#include <efilib.h>

#include <intel-efi/efiConsoleControl.h>
#include <intel-efi/efiUgaDraw.h>

#include <memorymap.h>
#include <graphics.h>

// protos
VOID EFIAPI ErrorCheck(EFI_STATUS st, IN CHAR16 *fmt);
EFI_STATUS EFIAPI WaitForKeyWithMessage(IN CHAR16 *fmt);

static EFI_GUID ConsoleControlProtocolGuid = EFI_CONSOLE_CONTROL_PROTOCOL_GUID;
static EFI_CONSOLE_CONTROL_PROTOCOL *ConsoleControl = NULL;

static EFI_GUID UgaDrawProtocolGuid = EFI_UGA_DRAW_PROTOCOL_GUID;
static EFI_UGA_DRAW_PROTOCOL *UgaDraw = NULL;

static EFI_GUID GraphicsOutputProtocolGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
static EFI_GRAPHICS_OUTPUT_PROTOCOL *GraphicsOutput = NULL;

EFI_STATUS EFIAPI LoadProtocols(VOID) {
    EFI_STATUS Status;

    // get protocols
    Status = LibLocateProtocol(&ConsoleControlProtocolGuid, (VOID **)&ConsoleControl);
    if (EFI_ERROR(Status)) {
        ConsoleControl = NULL;
    }

    Status = LibLocateProtocol(&UgaDrawProtocolGuid, (VOID **)&UgaDraw);
    if (EFI_ERROR(Status)) {
        UgaDraw = NULL;
    }

    Status = LibLocateProtocol(&GraphicsOutputProtocolGuid, (VOID **)&GraphicsOutput);
    if (EFI_ERROR(Status)) {
        GraphicsOutput = NULL;
    }

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI SetTextMode(EFI_SYSTEM_TABLE *st) {
    if (ConsoleControl != NULL) {
        uefi_call_wrapper(ConsoleControl->SetMode, 2, ConsoleControl, EfiConsoleControlScreenText);
    }

    // clear out, in
    uefi_call_wrapper(st->ConOut->ClearScreen, 1, ST->ConOut);

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI LogProtocolStatus(VOID) {
    Print(L"Active protocols:\n");

    if (ConsoleControl != NULL) {
        Print(L"        Got ConsoleControl protocol\n");
    } else {
        Print(L"        No ConsoleControl protocol\n");
    }

    if (GraphicsOutput != NULL) {
        Print(L"        Got GraphicsOutput protocol\n");
    } else {
        Print(L"        No GraphicsOutput protocol\n");
    }

    if (UgaDraw != NULL) {
        Print(L"        Got UgaDraw protocol\n");
    } else {
        Print(L"        No UgaDraw protocol\n");
    }

    Print(L"\n");

    return EFI_SUCCESS;
}

VOID EFIAPI ErrorCheck(EFI_STATUS status, IN CHAR16 *fmt) {
    if (EFI_ERROR(status)) {
        Print(fmt);
        Print(L"ErrorCode: %r\n", status);
    }
}


void write_string( int color, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = color;
    }
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *st) {
    InitializeLib(imageHandle, st);
    LoadProtocols();
    SetTextMode(st);
    LogProtocolStatus();
    init_graphics(GraphicsOutput);

    // sync memory map
    struct efi_memorymap map;
    EFI_STATUS err = memorymap_sync(st, &map);
    if (EFI_ERROR(err)) {
        Print(L"Error in memorymap_sync: %r\n", err);
       return err;
    }

    err = uefi_call_wrapper(st->BootServices->ExitBootServices, 2, imageHandle, map.key);
    if (EFI_ERROR(err)) {
        Print(L"Error in ExitBootServices #1: %r mapKey: %d\n", err, map.key);
        return err;
    }
    
    uefi_call_wrapper(st->RuntimeServices->SetVirtualAddressMap, 4, map.map_size, map.desc_size, map.desc_ver, map.map);
    
    // Some work, blends in the lithuanian flag
    for(uint8_t o = 0; o <= 100; o += 1) {
        for(int x = 0; x < 1920; x += 1) {
            for(int y = 0; y < 360; y += 1) {
                uint32_t r = (0xfd * o / 100 ) << 16;
                uint32_t g = (0xb9 * o / 100 ) << 8;
                uint32_t b = (0x13 * o / 100 );
                set_pixel(x, y, r | g | b);
            }
        }
        for(int x = 0; x < 1920; x += 1) {
            for(int y = 360; y < 720; y += 1) {
                uint32_t g = (0x6a * o / 100) << 8;
                uint32_t b = (0x44 * o / 100);
                set_pixel(x, y, g | b);
            }
        }
        for(int x = 0; x < 1920; x += 1) {
            for(int y = 720; y < 1080; y += 1) {
                uint32_t r = (0xc1 * o / 100) << 16;
                uint32_t g = (0x27 * o / 100) << 8;
                uint32_t b = (0x2d * o / 100);
                set_pixel(x, y, r | g | b);
            }
        }
    }
    
    
    for (;;) {
        __asm__("hlt");
    }

    return EFI_SUCCESS;
}
