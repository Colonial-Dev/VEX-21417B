#!/bin/bash
NC='\e[0m'
YELLOW='\e[1;33m'
BLUE='\e[36m'
GREEN='\e[1;32m'

#sshpass -p "robokauz" ssh eagle@cubesat.local 'cd ~ && sudo bash link_forward.sh'

rm -f /dev/vex0 > /dev/null 2>&1
rm -f /dev/vex1 > /dev/null 2>&1

nohup socat PTY,raw,echo=0,link=/dev/vex0 tcp:cubesat.local:27780 > /dev/null 2>&1 &
nohup socat PTY,raw,echo=0,link=/dev/vex1 tcp:cubesat.local:27781 > /dev/null 2>&1 &
disown -a
echo -e "${GREEN}Socat pipes opened.${NC}"

echo -e "${YELLOW}Waiting 5s to allow TCP link to be established...${NC}"
sleep 5
chmod o+rw /dev/vex0
chmod o+rw /dev/vex1
echo -e "${GREEN}Done - pipes are now read/write accessible!${NC}"