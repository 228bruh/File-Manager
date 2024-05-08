#pragma once
#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/class_Item.h"

void start_screen();

void disableCanonicInput(struct termios& oldt, struct termios& newt);

void enableCanonicInput(struct termios& oldt);

void displayFiles(const std::vector<Item>& items, int currentItem, std::string copiedItemPath, std::string movingItemPath, std::string start_dir);
