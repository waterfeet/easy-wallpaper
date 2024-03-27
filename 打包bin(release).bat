@echo off
chcp 65001
setlocal enabledelayedexpansion
mkdir Wallpaper_V
mkdir Wallpaper_V\image
xcopy image Wallpaper_V\image /s/i
mkdir Wallpaper_V\Qss
xcopy Qss Wallpaper_V\Qss /s/i
mkdir Wallpaper_V\lib
xcopy lib Wallpaper_V\lib /s/i
copy release\wallpaper.exe Wallpaper_V\lib
mkdir Wallpaper_V\Cfg
echo. > Wallpaper_V\Cfg\config.cfg
echo 打包完成！
::pause>nul