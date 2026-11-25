# dds-texconv-thumbnailer-kde

dds-texconv-thumbnailer-kde is a plugin for KDE 6 that creates thumbnails for Direct 
Draw Surface (DDS) images using [Texconv-Custom-DLL](https://github.com/matyalatte/Texconv-Custom-DLL).

## Build and install
Install libtexconv.so from [Texconv-Custom-DLL](https://github.com/matyalatte/Texconv-Custom-DLL)

Install other dependencies:

 - Arch: `sudo pacman -S cmake extra-cmake-modules qt6-base kio`
 - openSUSE: `sudo zypper install cmake kf6-extra-cmake-modules qt6-core-devel qt6-gui-devel kf6-kio-devel`

Build:

```
cmake -B build -DKDE_INSTALL_USE_QT_SYS_PATHS=ON -DBUILD_WITH_QT6=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build build
sudo cmake --install build
kbuildsycoca6
```

You can then restart Dolphin and enable the plugin in `Configure Dolphin`>`Interface`>`Previews`.
