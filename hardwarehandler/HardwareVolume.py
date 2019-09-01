import alsaaudio

def setVolumeHDMI(volume):
	print("setVolumeHDMI")
	m = alsaaudio.Mixer('PCM')
	m.setvolume(volume)
	print(m.getvolume())

def getVolumeHDMI():
	print("getVolumeHDMI")
	m = alsaaudio.Mixer('PCM')
	vol = m.getvolume()
	return vol[0]

def setVolumeHeadphone(volume):
	print("setVolumeHeadphone")
	m = alsaaudio.Mixer('Headphone')
	m.setvolume(volume)

def getVolumeHeadphone():
	print("getVolumeHeadphone")
	m = alsaaudio.Mixer('Headphone')
	vol = m.getvolume()
	return vol[0]

def processMessage(msg):
	print("HW volume ")
	print(msg)
	if "Vol,Up" in msg:
		currentVol = getVolumeHDMI()
		newVol = currentVol + 1
		print(newVol)
		setVolumeHDMI(newVol)
	if "Vol,Down" in msg:
		currentVol = getVolumeHDMI()
		newVol = currentVol - 1
		print(newVol)
		setVolumeHDMI(newVol)
	return NONE


#newvol = getVolumeHDMI()
#print(newvol)
#newvol = newvol + 1
#setVolumeHDMI(newvol)
#print(getVolumeHDMI())

