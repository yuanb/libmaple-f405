set flasher="%ProgramFiles(x86)%\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility\ST-LINK_CLI.exe"
set binary=build\discovery_f4.bin

%flasher% -c SWD -P %binary% 0x08010000 -V
pause