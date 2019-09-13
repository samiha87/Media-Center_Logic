
# Hardware Layer

### To run on boot
Using rc.local
sudo vim/etc/rc.local
sudo python /home/pi/HardwareHandler/HardwareHandler.py &
sudo reboot

# to control audio from python
sudo apt-get install python-alsaaudio
import alsaaudio
m = alsaaudio.Mixer()
current_volume = m.getvolume() # Get the current Volume
m.setvolume(70) # Set the volume to 70%.
m = alsaaudio.Mixer('PCM')
