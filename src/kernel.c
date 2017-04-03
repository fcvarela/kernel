#include <efi.h>
#include <efilib.h>

#include <intel-efi/efiConsoleControl.h>
#include <intel-efi/efiUgaDraw.h>

#include <memorymap.h>

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

EFI_STATUS EFIAPI SetTextMode(VOID) {
    if (ConsoleControl != NULL) {
        uefi_call_wrapper(ConsoleControl->SetMode, 2, ConsoleControl, EfiConsoleControlScreenText);
    }

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI LogProtocolStatus(VOID) {
    Print(L"Active protocols:\n");

    if (ConsoleControl != NULL) {
        Print(L"    Got ConsoleControl protocol\n");
    } else {
        Print(L"    No ConsoleControl protocol\n");
    }

    if (GraphicsOutput != NULL) {
        Print(L"    Got GraphicsOutput protocol\n");
    } else {
        Print(L"    No GraphicsOutput protocol\n");
    }

    if (UgaDraw != NULL) {
        Print(L"    Got UgaDraw protocol\n");
    } else {
        Print(L"    No UgaDraw protocol\n");
    }

    Print(L"\n");

    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI WaitForKeyWithMessage(IN CHAR16 *fmt) {
    EFI_INPUT_KEY efi_input_key;

    // wait for a keystroke
    Print(fmt);
    WaitForSingleEvent(ST->ConIn->WaitForKey, 0);
    EFI_STATUS status = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &efi_input_key);

    Print(L"\nScanCode: %xh  UnicodeChar: %xh CallRtStatus: %x\n\n", efi_input_key.ScanCode, efi_input_key.UnicodeChar, status);
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);

    return EFI_SUCCESS;
}

VOID EFIAPI ErrorCheck(EFI_STATUS status, IN CHAR16 *fmt) {
    if (EFI_ERROR(status)) {
        Print(fmt);
        Print(L"ErrorCode: %r\n", status);
    }
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE *st) {
    InitializeLib(imageHandle, st);
    LoadProtocols();
    SetTextMode();

    Print(L"Starting EFI boot...\n\n");
    LogProtocolStatus();
    WaitForKeyWithMessage(L"Press a key to continue...");

    // sync memory map
    memorymap_sync(st);

    EFI_STATUS status = uefi_call_wrapper(st->BootServices->ExitBootServices, imageHandle, memorymap_mapkey);
    if (EFI_ERROR(status)) {
        Print(L"Error in ExitBootServices #1: %r\n", status);
        return status;
    }

    // Whenceforth lies the kernel

    // catch
    for (;;) {}

    return EFI_SUCCESS;
}
