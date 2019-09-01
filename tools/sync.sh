
# Check if argument for ip has been set
if [[ "$#" -eq 0 ]]; then
	echo "User argument is missing, please pass user ./sync.sh <user> <IP_address>"
	echo "IP address is optional, if not set using default 10.42.0.232"
fi
USER=$1
RASPBERRY_IP=10.42.0.232
if [[ $# < 2 ]]; 
then 
	echo "IP address not set, using default address"
	echo "Default address 10.42.0.232"
else
	RASPBERRY_IP=$2
fi


echo "Starting to sync with Raspberry"
echo "User: " $USER
echo "Address: " $RASPBERRY_IP

rsync -avz $USER@$RASPBERRY_IP:/lib sysroot
rsync -avz $USER@$RASPBERRY_IP:/usr/include sysroot/usr
rsync -avz $USER@$RASPBERRY_IP:/usr/lib sysroot/usr
rsync -avz $USER@$RASPBERRY_IP:/opt/vc sysroot/opt
