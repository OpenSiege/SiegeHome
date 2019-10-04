# SiegeHome
Custom launcher to help facilitate installing DS1 / DS2 toolkits


## Features
* Auto updater
* Ability to read the appropriate DS1 / DS2 registry keys and display them
* Ability to write the appropriate DS1 / DS2 registry keys after Steam or Discs is installed
* Ability to check for Gmax installation
* Ability to download Dungeon Siege 1 toolkit and install it
* Ability to download Dungeon Siege 2 toolkit and install it
* Front end GUI powered by QT so no command line
* Built in tank extractor to the front end

## OpenSSL build if required
```
https://stackoverflow.com/a/45495686
perl Configure --prefix=E:\Programming\Libraries\bin\OpenSSL VC-WIN64A no-shared
```

## Minimal QT build if required
```
configure -static -static-runtime -no-pch -optimize-size -debug-and-release -opensource -nomake examples -nomake tests -prefix E:\Programming\Libraries\bin\Qt\5.12.5 -skip qt3d -skip qtactiveqt -skip qtandroidextras -skip qtcanvas3d -skip qtcharts -skip qtconnectivity -skip qtdatavis3d -skip qtdoc -skip qtgamepad -skip qtimageformats -skip qtlocation -skip qtmacextras -skip qtmultimedia -skip qtnetworkauth -skip qtpurchasing -skip qtremoteobjects -skip qtscript -skip qtscxml -skip qtsensors -skip qtserialbus -skip qtserialport -skip qtspeech -skip qtsvg -skip qttranslations -skip qtwayland -skip qtwebchannel -skip qtwebengine -skip qtwebglplugin -skip qtwebsockets -skip qtwebview -skip qtwinextras -skip qtx11extras -opengl desktop -ssl -openssl-linked OPENSSL_PREFIX=E:\Programming\Libraries\bin\OpenSSL
```