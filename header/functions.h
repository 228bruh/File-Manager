#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <unistd.h>
#include <termios.h>
#include <iomanip>

void start_screen();

void disableCanonicInput(struct termios& oldt, struct termios& newt);

void enableCanonicInput(struct termios& oldt);

class Item {
public:
    enum class Type { Folder, Document, Image, Audio, Video, Archive, Other };
    bool is_folder;
    std::string name;
    Type type;
    double size;

    Item();
    Item(const std::string& name, Type type, bool is_folder = false, double size = 0);

    static Type getFileType(const std::string& name);
};

bool compareItemsByName(const Item& a, const Item& b);

std::vector<Item> set_list_of_current_dir(std::string start_dir);

void displayFiles(const std::vector<Item>& items, int currentItem, std::string copiedItemPath, std::string movingItemPath, std::string start_dir);

void createItem(std::string start_dir);

bool removeItem(const std::string& path);

void confirmAndDelete(const std::string& path);

void renameItem(std::string start_dir, std::vector<Item>& items, int currentSelection);

void searchFile(const char* dir_name, const std::string& item_name, std::vector<std::string>& results);

void searchMenu();