#!/bin/bash
NC='\e[0m'
YELLOW='\e[1;33m'
BLUE='\e[36m'
GREEN='\e[1;32m'

pros make

echo -e "${BLUE}RSyncing hot package to Pi...${NC}"
sshpass -p "sudoking" rsync ~/Documents/21417/bin/hot.* eagle@cubesat:/home/eagle/hot_package
echo -e "${GREEN}Done.${NC}"
echo -e "${BLUE}Uploading hot package to robot...${YELLOW}" 
sshpass -p "sudoking" ssh eagle@cubesat 'bash bot_upload.sh'
echo -e "${GREEN}Done.${NC}"