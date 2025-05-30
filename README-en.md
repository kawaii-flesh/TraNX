# TraNX.ovl
TraNX - On-screen translator for Nintendo Switch

Инструкции на русском языке: [README.md](README.md)

中文说明: [README-zh.md](README-zh.md)

## Installation and Launch
1. Launch [TraNX-Server](https://github.com/kawaii-flesh/TraNX-Server)
2. Install [nx-ovlloader with extended memory](https://github.com/kawaii-flesh/nx-ovlloader/releases)
3. Download the `TraNX.ovl` file from the available releases
4. Place the `TraNX.ovl` file in the following path -> `sd:/switch/.overlays`
5. Launch `ovlmenu`
6. Select `TraNX` from the list of overlays (after the first launch, a configuration file will be created at `sd:/config/TraNX/config.ini`)
7. Exit the overlay (press `left stick`, press `left` on the d-pad)
8. In the configuration file, change the `host address` where `TraNX-Server` is running in the `uploadURL` field
9. Restart `TraNX`

## Controls
After launching `TraNX`, a rectangle will be displayed in the upper left corner, the color of which indicates the current mode:

`Green` - input focus on the game

`Yellow` - interactive mode:

Starting from version v1.1.0, it is possible to use combinations in both modes (green and yellow).
To do this, in the config `sd:/config/Trunk/config.ini`, an exclamation mark must be placed before the combination:
```ini
upload Combo=!RS ; now a translation request can be made in both modes without switching
```

Selecting the area for translation:
1. Hold `right` on the d-pad
2. Select the area by dragging on the touch screen
3. Release `right`

Selecting the translation output area (if not selected, it will output to the translation area):
1. Hold `up` on the d-pad
2. Select the area by dragging on the touch screen
3. Release `up`


- `right stick` - send translation request
- `down` - clear screen
- `left` - exit overlay
- `left stick` - return to green mode

## Screenshots

![1](/screenshots/1.jpg)

![2](/screenshots/2.jpg)

![3](/screenshots/3.jpg)

![4](/screenshots/4.jpg)