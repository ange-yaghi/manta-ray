![Alt text](docs/public/mantaray_banner_v2.jpg?raw=true)


**MantaRay** is an open-source renderer currently under development. It uses modern rendering techniques to generate photorealistic images of complex scenes. 
 
### Setup Instructions

Only a few steps are required to begin developing on MantaRay:
1. Make sure Python 3 is installed
2. Clone the MantaRay repository
3. Open **mantaray.sln** in Microsoft Visual Studio
4. Right click on **mantaray_demos** and select 'Set as StartUp Project'
5. Right click on **mantaray_demos** then click 'Properties'
6. For configuration 'All Configurations' and platform 'x64', navigate to 'Debugging' under 'Configuration Properties' and set **Environment** to: ```PATH=%PATH%;$(SolutionDir)\..\dependencies\runtime\x64```
-- **Note:** For x86 change this to ```runtime\x86``` (running in 64-bit mode, however, is recommended)
7. Run the application

All output from the demos is by default directed to: ```workspace/render/bitmap```

#### You are now ready to begin development!
