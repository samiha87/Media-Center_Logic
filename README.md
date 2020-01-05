# Status
CMake compiles and building software succeeeds. When running software on RPI immediate segmentation fault appears.
When building with Qt Compiler with same arm compiler, software runs without issues.
TODO is the issue with linking libraries?

# Drivers
## Onkyo TX-NR900E

Onkyo TX-NR900E
[CR] = 0x0D
Power
!1PWR01[CR]
!1PWR00[CR]
!1PWRQSTN[CR] -> !1PWR01<0x1a>
Testing RS232 commands through USB-RS232
On pi setup rs232 port to baud 9600, 8 databits, parity none, 1 stop bits  
	stty -F /dev/ttyUSB0 speed 9600 cs8 -cstopb -parenb
	echo '!1PWR00' > /dev/ttyUSB0 to power on
	echo '!1PWR01' > /dev/ttyUSB0 to power off
  
# Install Spotify
sudo apt-get update 
sudo apt-get upgrade

sudo apt install -y apt-transport-https curl
curl -sSL https://dtcooper.github.io/raspotify/key.asc | sudo apt-key add -v -
echo 'deb https://dtcooper.github.io/raspotify raspotify main' | sudo tee /etc/apt/sources.list.d/raspotify.list

sudo apt update
sudo apt install raspotify

Configure spotify

sudo nano /etc/default/raspotify
Uncomment DEVICE_NAME="raspotify" and change raspotify to what you want device to be called
Uncomment OPTIONS="--username <USERNAME> --password <PASSWORD>" Change username and password to your Spotify credentials
Apply changes by sudo systemctl restart raspotify or by rebooting raspberry

If you want to adjust audio on Raspberry via terminal, type alsamixer. This opens audio settings on terminal, works over ssh.

# Raspbian edited version for QT 5.12.6

# Cartracker_AVControl
Raspberry Pi running with Qt, controls AV systems
1.Download OSMC for Raspberry https://osmc.tv/download/
2.Copy the image to SD-Card with D or Balena Etcher.
3.Put the SD-Card to raspberry and start OSMC
4. Once OSMC has started properly, connect with ssh to pi
  - User: osmc, password: osmc
5. Open raspbery package manager sources
- sudo nano /etc/apt/sources.list
6. Add following
- deb-src http://raspbian.raspberrypi.org/raspbian/ stretch main contrib non-free rpi
7. Update sources
- sudo apt update
8. Install following packages
```shell
$ sudo apt-get build-deb qt4-x11
$ sudo apt-get build-deb libqt5gui5
$ sudo apt-get install libudev-dev libinput-dev 
$ sudo apt-get install libts-dev libxcb-xinerama0-dev
$ sudo apt-get install libxcb-xinerama0
$ sudo apt-get install gstreamer1.0-omx libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
$ sudo apt-get install python-pip
$ pip install pyserial
$ pip install alsaaudio
$ sudo apt-get install python-alsaaudio
```

! The toolset from raspberry site contains quite old tools for compilers. After updating the compiler for arm from https://releases.linaro.org/components/toolchain/binaries/6.1-2016.08/arm-linux-gnueabihf/ building of the program succeeded.

To build use following command line 
cmake -DCMAKE_SKIP_BUILD_RPATH=FALSE -DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE -DCMAKE_INSTALL_RPATH="/home/sami/raspi/sysroot/lib/arm-linux-gnueabihf;/home/sami/raspi/sysroot/usr/lib/arm-linux-gnueabihf;" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE ../


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
$ sudo apt-get build-dep qtbase-opensource-src
$ sudo apt-get install libegl1-mesa libegl1-mesa-dev libgles2-mesa libgles2-mesa-dev
```
6. Make target directory
- sudo mkdir /usr/local/qt5pi
- chown pi:pi /usr/local/qt5pi
  
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
$ chmod +x sysroot-relativelinks.py
$ ./sysroot-relativelinks.py sysroot
```
10. Download and configure Qt for Raspberry Pi. Download 5.10 due it has been tested with Raspberry Pi 1 and device is set as linux-rasp-pi-g++ Raspberry version 1.
```shell
$ git clone git://code.qt.io/qt/qtbase.git -b 5.10
$ cd qtbase
$ ./configure -release -opengl es2 -device linux-rasp-pi-g++ -device-option CROSS_COMPILE=~/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf- -sysroot ~/raspi/sysroot -opensource -confirm-license -make libs -prefix /usr/local/qt5pi -no-use-gold-linker -extprefix ~/raspi/qt5pi -hostprefix ~/raspi/qt5 -v 
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
13. Connect to raspberry, links to qt libs and run program 
```shell
$ ssh user@ip -X
$ echo /usr/local/qt5pi/lib | sudo tee /etc/ld.so.conf.d/qt5pi.conf
$ sudo ldconfig
$ ./qopenglwidget
```
14. Update to latest distro
- sudo apt-get update
- sudo apt-get upgrade

## Video playing
To run Qt Video player install all gstreamer-0.10 packages. Otherwise will complain Gstreamer missing plugin.
sudo apt-get install h264enc. Couldnt figure this
Trying to compile qt multimeida module wiht gstreamer 1.0
open multimedia module folder
..raspi/qt5/bin/qmake -r GST_VERSION=1.0

# OSMC
OSMC control: https://github.com/bwssytems/ha-bridge/wiki/Kodi-volume-control
# Cartracker_AVControl
Raspberry Pi running with Qt, controls AV systems
1.Download OSMC for Raspberry https://osmc.tv/download/
2.Copy the image to SD-Card with D or Balena Etcher.
3.Put the SD-Card to raspberry and start OSMC
4. Once OSMC has started properly, connect with ssh to pi
  - User: osmc, password: osmc
5. Open raspbery package manager sources
- sudo nano /etc/apt/sources.list
6. Add following
- deb-src http://raspbian.raspberrypi.org/raspbian/ stretch main contrib non-free rpi
7. Update sources
- sudo apt update
8. Install following packages
```shell
$ sudo apt-get build-deb qt4-x11
$ sudo apt-get build-deb libqt5gui5
$ sudo apt-get install libudev-dev libinput-dev 
$ sudo apt-get install libts-dev libxcb-xinerama0-dev
$ sudo apt-get install libxcb-xinerama0
$ sudo apt-get install gstreamer1.0-omx libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
$ sudo apt-get install python-pip
$ pip install pyserial
$ pip install alsaaudio
$ sudo apt-get install python-alsaaudio
```

! The toolset from raspberry site contains quite old tools for compilers. After updating the compiler for arm from https://releases.linaro.org/components/toolchain/binaries/6.1-2016.08/arm-linux-gnueabihf/ building of the program succeeded.

To build use following command line 
cmake -DCMAKE_SKIP_BUILD_RPATH=FALSE -DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE -DCMAKE_INSTALL_RPATH="/home/sami/raspi/sysroot/lib/arm-linux-gnueabihf;/home/sami/raspi/sysroot/usr/lib/arm-linux-gnueabihf;" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=TRUE ../


## Cross-compiling with Qt memo, Tested with Raspberry PI 1. Cross-compile tutorial from wiki.qt.io
### Host side
!Old verison of raspberry and Qt used due confict with compiler libraries
libicui18.so caused trouble, thus sticking with older versions.
Compailer and libraries of Raspberry pi did not match.

1. Download latest raspberry image
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
$ sudo apt-get build-deb qt4-x11 or sudo apt-get install libx11-dev libxcb1-dev libxkbcommon-x11-dev libx11-xcb-dev libxext-dev
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
1. https://wiki.qt.io/RaspberryPi2EGLFS#Qt_Creator
2. https://github.com/bwssytems/ha-bridge/wiki/Kodi-volume-control
3. https://wiki.qt.io/RaspberryPi2EGLFS
4. https://pimylifeup.com/raspberry-pi-spotify/

