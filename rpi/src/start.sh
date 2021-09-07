WORKON_HOME=$HOME/.virtualenvs
VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3
source /usr/local/bin/virtualenvwrapper.sh
export DISPLAY=:0

echo "running cv script by default"

trap 'echo "kill python by default" && killall python' SIGINT SIGTERM SIGHUP SIGQUIT

workon cv
python /home/pi/slowburning/main.py

