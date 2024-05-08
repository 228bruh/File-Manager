#pragma once
#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/class_Item.h"

void createItem(std::string start_dir);

bool removeItem(const std::string& path);

void confirmAndDelete(const std::string& path);

void renameItem(std::string start_dir, std::vector<Item>& items, int currentSelection);

void searchFile(const char* dir_name, const std::string& item_name, std::vector<std::string>& results);

void searchMenu();