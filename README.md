# NativeHost
A small framework for the creation of Chrome native hosts, only Windows supported.  Because of the inability to use cout, a logger is implemented to see data sent/received by the native host for better debugging.

The test folder provides a base example chrome extension, to demonstrate calling c++ functions from chrome.

To Install:
1. Using MSVC, compile the Test executable, and keep note of the executable filepath.
2. Run Install.bat with the full filepath of the executable, ex: ```Install.bat C:\Users\tdela\source\repos\NativeHost\NativeHost\x64\Debug\Test.exe```  The install script will add the native host to the registry, and generate the manifest.
3. Next, refer to https://developer.chrome.com/docs/extensions/mv3/getstarted/ steps 1-3 to create the extension in chrome.
4. Copy the id from the newly created extension, and paste in the generated manifest, ex: ```chrome-extension://hfpkmanmjadadekcaifmgnmfbbjcigfi/```.  
5. When the extension runs, you should be able to see the logger window.

To Uninstall, ```Install.bat C:\Users\tdela\source\repos\NativeHost\NativeHost\x64\Debug\Test.exe -uninstall``` and manually remove the extension from chrome.
