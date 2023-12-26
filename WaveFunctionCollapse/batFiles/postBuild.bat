echo %cd%
echo Copying the build to c# tests
xcopy ..\x64\Release\WaveFunctionCollapse.dll ..\c#Tests\libs /y
echo Copying the build to WaveFunctionCollapse Unity Project
xcopy ..\x64\Release\WaveFunctionCollapse.dll C:\GameDev\WaveFunctionCollapse\Assets\Library\WaveFunctionCollapse\dll /y
echo Completed Copying Successfully