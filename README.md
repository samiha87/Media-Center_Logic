# Cartracker_AVControl
Raspberry Pi running with Qt, controls AV systems

## Cross-compiling with Qt memo, Tested with Raspberry PI 1. Cross-compile tutorial from wiki.qt.io
### Host side
!Old verison of raspberry and Qt used due confict with compiler libraries
libicui18.so caused trouble, thus sticking with older versions.
Compailer and libraries of Raspberry pi did not match.

1. Download raspberry image 2015-09-24
2. DD image to SD-Card with Balena Etcher

### RPI side
3. Configure Raspberry Pi 
```shell
$ sudo raspi-config
```
- Advenced options -> Enable ssh
- Boot options -> Set Console only
- Advenced options ->Set GPU size to 256
- Exit options
```shell
$ sudo reboot
```
4. After reboot type allow updates from source list. Uncomment the deb-src line.
```shell
$ sudo nano /etc/apt/sources.list
```
5. Install required libraries
```shell
$ sudo apt-get build-deb qt4-x11
$ sudo apt-get build-deb libqt5gui5
$ sudo apt-get install libudev-dev libinput-dev 
$ sudo apt-get install libts-dev libxcb-xinerama0-dev
$ sudo apt-get install libxcb-xinerama0
```
6. Make target directory
  6.1 sudo mkdir /usr/local/qt5pi
  6.2 chown pi:pi /usr/local/qt5pi
  
### Host side
7. Create a working dir and download toolset
```shell
$ mkdir ~/raspi
$ cd ~/raspi
$ git clone https://github.com/raspberrypi/tools
```
8. Create system root folder for synching with Raspberry Pi. This will take time.
```shell
$ mkdir sysroot sysroot/usr sysroot/opt
$ rsync -avz pi@raspberrypi.local:/lib sysroot
$ rsync -avz pi@raspberrypi.local:/usr/include sysroot/usr
$ rsync -avz pi@raspberrypi.local:/usr/lib sysroot/usr
$ rsync -avz pi@raspberrypi.local:/opt/vc sysroot/opt
```
9. Set systemlinks
```shell
$ wget https://raw.githubusercontent.com/Kukkimonsuta/rpi-buildqt/master/scripts/utils/sysroot-relativelinks.py
$chmod +x sysroot-relativelinks.py
$./sysroot-relativelinks.py sysroot
```
10. Download and configure Qt for Raspberry Pi. Download 5.10 due it has been tested with Raspberry Pi 1 and device is set as linux-rasp-pi-g++ Raspberry version 1.
```shell
$ git clone git://code.qt.io/qt/qtbase.git -b 5.10
$ cd qtbase
$ ./configure -release -opengl es2 -device linux-rasp-pi-g++ -device-option CROSS_COMPILE=~/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf- -sysroot ~/raspi/sysroot -opensource -confirm-license -make libs -prefix /usr/local/qt5pi -no-use-gold-linker
$ make
$ make install
```
11. Synch built qt with Raspberry
```shell
$ cd ..
$ rsync -avz qt5pi pi@raspberrypi.local:/usr/local
```
12. Build test program and copy to Raspberry
```shell
$ cd qtbase/examples/opengl/qopenglwidget
$ ~/raspi/qt5/bin/qmake
$ make
$ scp qopenglwidget pi@raspberrypi.local:/home/pi
```
13. Connect to raspberr, links to qt libs and run program 
```shell
$ ssh user@ip -X
$ echo /usr/local/qt5pi/lib | sudo tee /etc/ld.so.conf.d/qt5pi.conf
$ sudo ldconfig
$ ./qopenglwidget
```
## Video playing
To run Qt Video player install all gstreamer-0.10 packages. Otherwise will complain Gstreamer missing plugin.
sudo apt-get install h264enc. Couldnt figure this
Trying to compile qt multimeida module wiht gstreamer 1.0
open multimedia module folder
..raspi/qt5/bin/qmake -r GST_VERSION=1.0

# References
1.https://wiki.qt.io/RaspberryPi2EGLFS#Qt_Creator

