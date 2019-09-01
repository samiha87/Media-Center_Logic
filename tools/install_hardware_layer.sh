
# Check if argument for ip has been set
if [[ "$#" -eq 0 ]]; then
	echo "User argument is missing, please pass user ./sync.sh <user> <IP_address>"
	echo "IP address is optional, if not set using default 10.42.0.232"
fi
USER=$1
RASPBERRY_IP=10.42.0.232
# Check if user set more than 1 argument
if [[ $# < 2 ]]; 
then 
	echo "IP address not set, using default address"
	echo "Default address 10.42.0.232"
else
	RASPBERRY_IP=$2
fi
# Copy files to raspberry
echo "Installing hardware layer"
scp -r -v ../hardwarehandler $USER@$RASPBERRY_IP:~/hardwarehandler/ > output.log 2>&1
input="output.log"
#Open output of scp and check if installation was succesfull
while IFS= read -r line
do
  	if [[ $line == *"Exit status 0"* ]];
  		then
			echo "Hardware layer installed"
			# Remove output.log
			rm output.log
			exit 0
	fi
done < "$input"
# Remove output.log
rm output.log
echo "Installation failed"