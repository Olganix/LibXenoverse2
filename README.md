

LibXenoverse is a simple library to handle Dragon Ball Xenoverse's file formats.
This version is specialy for DragonBall Xenoverse2 modding.
Supports both little endian (PC) and big endian (Xbox 360) files (Olganix: todo recheck Xbox360 files)

Up to that library, there is tools to :
-unpack some files
-convert into Xml to edit.
-edit/merge file directly
-convert into fbx for blender edit.
You can also integrate LibXenoverse into your own applications 
 if you wish to add support for these files on a custom game engine, 
 previewer, plugin, etc. Simply build LibXenoverse.lib and link it.



#### Builds
Win32 builds of the tools are provided on the /bin/ directory.
you have to unzip "\depends\fbxsdk.zip".

All projects are for visual studio 2013 update 5.

#### [Tools](https://github.com/DarioSamo/LibXenoverse/wiki#tools)
#### [Building (Only required if you want to contribute code)](https://github.com/DarioSamo/LibXenoverse/wiki#building)
#### [Tutorials](https://github.com/DarioSamo/LibXenoverse/wiki#tutorials)

#### Modding the game
It's recommended to use the data folder inside of data2.cpk to override the files from data.cpk.
	It's much smaller and faster to repack than the main big data.cpk. 
	Use the CPKPack tool to extract and repack Dragon Ball Xenoverse's CPK files.
WARNING: Repacking data2.cpk with the uncompressed files seem to just crash the game at the moment for some reason. 
	Try extracting the files first with extract-data2.bat (leaves the files compressed) and repack those instead.

Olganix: now , it's more using Eternity's Yace to extract, and his patcher, so you could live edit some files. 
				 (using CPKPack to repack make a security exception in game).



#### Use XenoViewer
-add esk file for add skeleton
-add emd for add meshes. it's will automatically use emm (material) and emd (textures), and dyt.emb (color degrade texture)
-add ean files  to use animations.

-drag and drop emd on skeleton to apply skinning.
-double-click on an animations to view the result

-for scd, do the same, and after put the scd.esk on body esk. 

in view part:
-use left click to turn camera.
-use right click to move camera.
-use mouse wheel to zoom/unzoom.
Note : view part could didn't work with more than one monitor.
 
-Use the right click to see a context menu. in it, you can apply some default material, change light, display debug like skeleton.


#### Authors
The original version of Libxenoverse is done by Dario (and may be with help of Eternity).
The files relative of Emo and Ema are from Eternity.
Olganix make corrections on that, add some tools, with the help from Eternity and the Zteam (Komodo, val, etc...).




