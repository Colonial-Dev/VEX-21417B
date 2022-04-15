#!/bin/bash
NC='\e[0m'
YELLOW='\e[1;33m'
BLUE='\e[36m'
GREEN='\e[1;32m'

pros make

echo -e "${BLUE}RSyncing hot package to Pi...${NC}"
sshpass -p "robokauz" rsync -P ~/Documents/21417/bin/hot.package.* eagle@cubesat.local:/home/eagle/program_binary
echo -e "${GREEN}Done.${NC}"
echo -e "${BLUE}Uploading hot package to robot...${YELLOW}" 
sshpass -p "robokauz" ssh eagle@cubesat.local 'cd ~/program_binary && ~/.local/bin/pros upload hot.package.* --slot 1 --target v5'
echo -e "${GREEN}Done.${NC}"
