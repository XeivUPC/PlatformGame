@echo off
setlocal enabledelayedexpansion

REM Set your desired new level number here
set "newLevel=Level1"  REM Change LevelX to your desired level number

REM Set the directory containing your files
set "directory=C:\Users\adriaga1\Documents\GitHub\PlatformGame\Assets\Maps\LevelsData\Level1"  REM Change this to your directory path

cd /d "%directory%"
for %%f in (*) do (
    set "filename=%%f"
    set "newname=!newLevel! - !filename:* - =!"
    ren "%%f" "!newname!"
)

echo Files renamed successfully!
pause