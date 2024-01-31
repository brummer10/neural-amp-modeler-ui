# Neural Amp Modeler LV2 GUI

This is a Gui for the [Neural Amp Modeler LV2 plugin](https://github.com/mikeoliphant/neural-amp-modeler-lv2) by Mike Oliphant

![Neural Amp Modeler UI](https://raw.githubusercontent.com/brummer10/neural-amp-modeler-ui/master/Neural_Amp_Modeler.png)

It requires that the [Neural Amp Modeler LV2 plugin](https://github.com/mikeoliphant/neural-amp-modeler-lv2)
is installed. Otherwise it will refuse to be installed.

## Dependencies

- libcairo2-dev
- libx11-dev
- lv2-dev

## Build
- git submodule init
- git submodule update
- make

Depending on how you've installed the [Neural Amp Modeler LV2 plugin](https://github.com/mikeoliphant/neural-amp-modeler-lv2)
you must install the GUI to the same location. So, if you've installed [Neural Amp Modeler LV2 plugin](https://github.com/mikeoliphant/neural-amp-modeler-lv2)
with sudo, use sudo as well to install the GUI, if you've installed [Neural Amp Modeler LV2 plugin](https://github.com/mikeoliphant/neural-amp-modeler-lv2)
into your home .lv2 folder, install the GUI without using sudo.

- make install # will install into ~/.lv2 ... AND/OR....
- sudo make install # will install into /usr/lib/lv2

