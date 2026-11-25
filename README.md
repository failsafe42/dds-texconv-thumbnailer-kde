**NOTE:** image thumbnailer from KIO-extras package now support DDS file format
(https://github.com/KDE/kio-extras/blob/master/thumbnail/imagethumbnail.json#L30).

# dds-texconv-thumbnailer-kde

dds-texconv-thumbnailer-kde is a plugin for KDE 6 that creates thumbnail for Direct 
Draw Surface (DDS) images using [Texconv-Custom-DLL](https://github.com/matyalatte/Texconv-Custom-DLL).

## Build and install

Install dependencies:

 - Arch: `sudo pacman -S cmake extra-cmake-modules qt6-base kio`

Build:

```
mkdir build
cd build
cmake -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DBUILD_WITH_QT6=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
sudo cmake --install .
kbuildsycoca6
```

You can then restart Dolphin and enable the plugin in `Configure Dolphin`>`Interface`>`Previews`.
