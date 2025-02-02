How to:
- Use CMake to create the application visual studio project
- Open solution.
- Add existing project "WinInstaller.wapproj"
- The package.appxmanifest is just a decoy, only use it to generate images
- The actual images to be copied can be added by editing WinInstaller.wapproj
- There, add:
  <ItemGroup>
    <Content Include="BitFiddle\ascii.png" />
    ...
- Then, use visual studio, right click on the WinInstaller project and press Publish.



Certificate:

Right click on APPX file
Click Properties
Click Digital Signatures
Select Signature from the list
Click Details
Click View Certificate
Click Install Certificate
Choose Local Computer
Choose Trusted Base Certificate