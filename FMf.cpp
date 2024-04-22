#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> 
#include <unistd.h>
#include <termios.h>
#include <iomanip>

class Item {
public:
    enum class Type { Folder, Document, Image, Audio, Video, Archive, Other };
    bool is_folder;
    std::string name;
    Type type;
    double size = 0;
};

Item::Type getFileType(const std::string& name) {
    std::size_t pos = name.find_last_of('.');
    if (pos != std::string::npos) {
        std::string extension = name.substr(pos + 1);

        if (extension == "jpg" || extension == "jpeg" || extension == "png" || extension == "gif" || extension == "bmp") {
            return Item::Type::Image;
        } else if (extension == "mp3" || extension == "wav" || extension == "flac" || extension == "aac") {
            return Item::Type::Audio;
        } else if (extension == "mp4" || extension == "avi" || extension == "mkv" || extension == "mov") {
            return Item::Type::Video;
        } else if (extension == "zip" || extension == "rar" || extension == "tar" || extension == "gz" || extension == "jar") {
            return Item::Type::Archive;
        } else if (extension == "txt" || extension == "pdf" || extension == "doc" || extension == "docx") {
            return Item::Type::Document;
        }
    }
    return Item::Type::Other;
}

bool compareItemsByName(const Item& a, const Item& b) {
    return a.name < b.name;
}

std::vector<Item> set_list_of_current_dir(std::string start_dir) {
    std::vector<Item> items;
    DIR *dir;
    struct dirent *ent;
    struct stat st;
    if ((dir = opendir(start_dir.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            const char* name = ent->d_name;
            if (name[0] != '.') { 
                Item item;
                item.name = name;
                std::string full_path = start_dir + "/" + name;
                if (stat(full_path.c_str(), &st) == 0) {
                    item.is_folder = S_ISDIR(st.st_mode);
                    item.size = st.st_size / (1024.0 * 1024.0);
                    if (item.is_folder) {
                        item.type = Item::Type::Folder;
                    } else {
                        item.type = getFileType(name);
                    }
                    items.push_back(item);
                }
            }
        }
        closedir(dir);

        std::sort(items.begin(), items.end(), compareItemsByName);
    } else {
        std::cerr << "Error opening directory" << std::endl;
    }
    return items;
}

void start_screen();

void displayFiles(const std::vector<Item>& items, int currentItem, std::string copiedItemPath, std::string movingItemPath) {
    std::cout << "\033c";

    for (size_t i = 0; i < items.size(); ++i) {
        if (i == currentItem) {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "Folder";
                    break;
                case Item::Type::Document:
                    type_str = "Document";
                    break;
                case Item::Type::Image:
                    type_str = "Image";
                    break;
                case Item::Type::Audio:
                    type_str = "Audio";
                    break;
                case Item::Type::Video:
                    type_str = "Video";
                    break;
                case Item::Type::Archive:
                    type_str = "Archive";
                    break;
                case Item::Type::Other:
                    type_str = "Other";
                    break;
            }
            std::cout << "\033[32m";
            std::cout << "-> ";
            std::cout << std::setw(35) << std::left << items[i].name;
            std::cout << std::setw(10) << std::left << type_str;
            std::cout << std::setw(10) << std::left << items[i].size << " MB\n" << "\033[0m";
        } else {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "Folder";
                    break;
                case Item::Type::Document:
                    type_str = "Document";
                    break;
                case Item::Type::Image:
                    type_str = "Image";
                    break;
                case Item::Type::Audio:
                    type_str = "Audio";
                    break;
                case Item::Type::Video:
                    type_str = "Video";
                    break;
                case Item::Type::Archive:
                    type_str = "Archive";
                    break;
                case Item::Type::Other:
                    type_str = "Other";
                    break;
            }
            std::cout << "   ";
            std::cout << std::setw(35) << std::left << items[i].name;
            std::cout << std::setw(10) << std::left << type_str;
            std::cout << std::setw(10) << std::left << items[i].size << " MB\n";
        }
    }

    if (copiedItemPath != "") {
        std::cout << "\n\n------------------------------------------------------------------------\n";
        std::cout << "Clipboard: " << copiedItemPath << "\n";
    }

    if (movingItemPath != "") {
        std::cout << "\n\n------------------------------------------------------------------------\n";
        std::cout << "Moving: " << movingItemPath << "\n";
    }
}

bool removeFileOrDirectory(const std::string& path, bool is_folder) {
    if (is_folder) {
        if (rmdir(path.c_str()) != 0) {
            std::cerr << "Error removing directory: " << path << std::endl;
            return false;
        }
    } else {
        if (unlink(path.c_str()) != 0) {
            std::cerr << "Error removing file: " << path << std::endl;
            return false;
        }
    }
    return true;
}

void confirmAndDelete(const std::string& path, bool is_folder) {
    std::cout << "\nAre you sure you want to delete this item: " << path << "? (y/n)\n";
    char response;
    std::cin >> response;
    if (response == 'y' || response == 'Y') {
        if (removeFileOrDirectory(path, is_folder)) {
            std::cout << "Item successfully deleted.\n";
        } else {
            std::cout << "Failed to delete item.\n";
        }
    } else {
        std::cout << "Deletion canceled.\n";
    }
    std::cout << "\nPress any key to continue: ";
    std::cin.ignore();
}

void createItem(const std::string& path, bool isFolder) {
    if (isFolder) {
        mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        std::ofstream file(path);
    }
}

int main() {
    start_screen();

    std::string start_dir = "/home";
    std::vector<Item> items = set_list_of_current_dir(start_dir);

    int currentSelection = 0;
    
    std::string copiedItemPath;
    std::string movingItemPath;
    bool isMoving = false;

    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    displayFiles(items, currentSelection, copiedItemPath, movingItemPath);

    char input;
    while (read(STDIN_FILENO, &input, 1) == 1) {

        if (!items.empty() && input == '\033') {  // arrows
            read(STDIN_FILENO, &input, 1); 
            read(STDIN_FILENO, &input, 1);
            switch (input) {
                case 'A': 
                    currentSelection = (currentSelection == 0) ? items.size() - 1 : currentSelection - 1;
                    break;
                case 'B': 
                    currentSelection = (currentSelection + 1) % items.size();
                    break;
            }
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
        }

        else if (input == 'q') { // quit
            break;
        }

        else if (input == '\n') { // Enter
            if (!items.empty() && items[currentSelection].is_folder) { 
                start_dir += "/" + items[currentSelection].name;
                items = set_list_of_current_dir(start_dir); 
                if (items.empty()) {
                    std::cout << "\033c";
                    std::cout << "This folder is empty\n";
                } else {
                currentSelection = 0; 
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
                }
            }
        }

        else if (input == 127) { // Backspace
            if (start_dir != "/" && start_dir != "/home") {
                size_t pos = start_dir.find_last_of('/');
                if (pos != std::string::npos) {
                    start_dir = start_dir.substr(0, pos);
                    items = set_list_of_current_dir(start_dir); 
                    currentSelection = 0; 
                    displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
                }
            } else {
                start_dir = "/";
                items = set_list_of_current_dir(start_dir); 
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
            }

        }

        else if (input == '+') { // create item in curr dir 
            std::cout << "\033c";
            std::cout << "Create item in " << start_dir << "\n";
            std::cout << "Enter name: ";
            std::string itemName;
            std::cin >> itemName;
            std::string itemPath = start_dir + "/" + itemName;
        
            struct stat buffer;
            while (stat(itemPath.c_str(), &buffer) == 0) {
                std::cout << "\n\nItem with the same name '" << itemName << "' already exists in this directory\n";
                std::cout << "Enter name: ";
                std::cin >> itemName;
                itemPath = start_dir + "/" + itemName;
                if (stat(itemPath.c_str(), &buffer) != 0) {
                    break;
                }
            }

            bool isFolder = (itemName.find('.') == std::string::npos); 
            createItem(itemPath, isFolder); 
            items = set_list_of_current_dir(start_dir);
            currentSelection = 0;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
        }

        else if (!items.empty() && input == '-') { // delete folder or file
            std::string path = start_dir + "/" + items[currentSelection].name;

            if (items[currentSelection].is_folder) {
                confirmAndDelete(path, true);
            } else {
                confirmAndDelete(path, false);
            }

            items = set_list_of_current_dir(start_dir); 
            if (items.empty()) {
                    std::cout << "\033c";
                    std::cout << "This folder is empty\n";
            } else {
                currentSelection = 0; 
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
            }
        }

        else if (!items.empty() && input == 'r') { // rename
            std::cout << "\033c";
            std::cout << "Rename this item: '" << items[currentSelection].name << "'\n";
            std::cout << "Enter new name: ";
            std::string newName;
            std::cin >> newName;
            std::string newPath = start_dir + "/" + newName;
            
            struct stat buffer;
            while (stat(newPath.c_str(), &buffer) == 0) {
                std::cout << "\n\nItem with the name '" << newName << "' already exists in this directory\n";
                std::cout << "Enter new name: ";
                std::cin >> newName;
                newPath = start_dir + "/" + newName;
                if (stat(newPath.c_str(), &buffer) != 0) {
                    break;
                }
            }

            Item& currentItem = items[currentSelection];
            std::string oldPath = start_dir + "/" + currentItem.name;
            rename(oldPath.c_str(), newPath.c_str());
            items = set_list_of_current_dir(start_dir);
            currentSelection = 0;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
            
        }

        else if (!items.empty() && input == 'c') { //copy
            copiedItemPath = start_dir + "/" + items[currentSelection].name;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
        }

        else if (input == 'v' && copiedItemPath != "") { // paste
            std::string command = "cp -r '" + copiedItemPath + "' '" + start_dir + "'";
            system(command.c_str());

            copiedItemPath = "";
            items = set_list_of_current_dir(start_dir);
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
        }

        else if (input == 'm') { // move
            if (isMoving) {
                std::string command = "mv '" + movingItemPath + "' '" + start_dir + "'";
                system(command.c_str());

                movingItemPath = "";
                items = set_list_of_current_dir(start_dir);
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
                isMoving = false;
            } else {
                movingItemPath = start_dir + "/" + items[currentSelection].name;
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath);
                isMoving = true;
            }
        }

        else if (input == 's') { // search

        }

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}

void start_screen() {
    std::cout << "Navigation\n";
    std::cout << "--------------------------------------\n";
    std::cout << "↓ ↑       |navigate through the items|\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "Enter     |enter dir                 |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "Backspace |back to previous dir      |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "q         |quit                      |\n";
    std::cout << "--------------------------------------\n\n";
    std::cout << "Edit\n";
    std::cout << "--------------------------------------\n";
    std::cout << "+         |create item in current dir|\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "-         |delete selected item      |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "r         |rename selected item      |\n";
    std::cout << "--------------------------------------\n\n";
    std::cout << "Manipulations\n";
    std::cout << "--------------------------------------\n";
    std::cout << "c         |copy selected item        |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "v         |paste copied item         |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "m         |move selected item        |\n";
    std::cout << "--------------------------------------\n\n";
    std::cout << "Press Enter to continue: ";
    std::cin.ignore();
}

// search



