set PATH=%PATH%;%~dp0/../dependencies/tools/bison/bin;%~dp0/../dependencies/tools/flex/bin
if not exist "%~dp0/../autogen/" (
    mkdir "%~dp0/../autogen/"
)

win_bison.exe -Wconflicts-sr "%~dp0/../flex-bison/sdl_specification.y" --defines="%~dp0/../autogen/sdl_parser.auto.h" --output="%~dp0/../autogen/sdl_parser.auto.cpp"
win_flex.exe -+ -wincompat -o"%~dp0/../autogen/sdl_scanner.auto.cpp" "%~dp0/../flex-bison/sdl_scanner.l"
