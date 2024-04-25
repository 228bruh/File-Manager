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
    if (a.is_folder && !b.is_folder) {
        return true;
    } else if (!a.is_folder && b.is_folder) {
        return false;
    } else {
        return a.name < b.name;
    }
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
                    if (item.is_folder) {
                        item.type = Item::Type::Folder;
                    } else {
                        item.size = st.st_size / (1000.0 * 1000.0);
                        item.size = std::round(item.size * 1000.0) / 1000.0;
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

void displayFiles(const std::vector<Item>& items, int currentItem, std::string copiedItemPath, std::string movingItemPath, std::string start_dir) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";
    }
    std::cout << "|\n\n";

    if (items.empty()) {
        std::cout << "This folder is empty\n";
    } else {

    for (size_t i = 0; i < items.size(); ++i) {
        if (i == currentItem) {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "   \033[33m┌─┐___\n\033[32m->\033[33m │  _  │   \033[32m" + items[i].name + "\n\033[33m   └─┴─┴─┘";
                    break;
                case Item::Type::Document:
                    type_str = "   ┌───┐\n\033[32m->\033[0m │~~~│     \033[32m" + items[i].name + "\n\033[0m   └───┘";
                    break;
                case Item::Type::Image:
                    type_str = "   \033[34m┌───┐\n\033[32m->\033[34m │◠˚✧│     \033[32m" + items[i].name + "\n\033[34m   └───┘"; 
                    break;
                case Item::Type::Audio:
                    type_str = "   \033[90m┌───┐\n\033[32m->\033[90m │ ♫ │     \033[32m" + items[i].name + "\n\033[90m   └───┘";
                    break;
                case Item::Type::Video:
                    type_str = "   \033[31m┌───┐\n\033[32m->\033[31m │ ▶ │     \033[32m" + items[i].name + "\n\033[31m   └───┘";
                    break;
                case Item::Type::Archive:
                    type_str = "   \033[33;2m┌───┐\n\033[0m\033[32m->\033[33;2m │ ↓ │     \033[0m\033[32m" + items[i].name + "\n\033[33;2m   └───┘\033[0m";
                    break;
                case Item::Type::Other:
                    type_str = "   ┌───┐\n\033[32m->\033[0m │ ⚙ │     \033[32m" + items[i].name + "\n\033[0m   └───┘";
                    break;
            }

            std::cout << type_str;

            if (items[i].is_folder) {
                std::cout << "\033[0m\n";
            } else {
                std::cout << "     \033[32m" << items[i].size << " MB\033[0m\n";
            }    

        } else {
            std::string type_str;
            switch (items[i].type) {
                case Item::Type::Folder:
                    type_str = "\033[33m     ┌─┐___\n     │  _  │ \033[0m" + items[i].name +"\n\033[33m     └─┴─┴─┘";
                    break;
                case Item::Type::Document:
                    type_str = "     ┌───┐\n     │~~~│   " + items[i].name + "\n     └───┘"; 
                    break;
                case Item::Type::Image:
                    type_str = "\033[34m     ┌───┐\n     │◠˚✧│   \033[0m" + items[i].name +"\n\033[34m     └───┘";
                    break;
                case Item::Type::Audio:
                    type_str = "\033[90m     ┌───┐\n     │ ♫ │   \033[0m" + items[i].name +"\n\033[90m     └───┘";
                    break;
                case Item::Type::Video:
                    type_str = "\033[31m     ┌───┐\n     │ ▶ │   \033[0m" + items[i].name +"\n\033[31m     └───┘";
                    break;
                case Item::Type::Archive:
                    type_str = "\033[33;2m     ┌───┐\n     │ ↓ │   \033[0m" + items[i].name +"\n\033[33;2m     └───┘";
                    break;
                case Item::Type::Other:
                    type_str = "     ┌───┐\n     │ ⚙ │   " + items[i].name + "\n     └───┘";
                    break;
            }

            std::cout << type_str;

            if (items[i].is_folder) {
                std::cout << "\033[0m\n";
            } else {
                std::cout << "   \033[0m" << items[i].size << " MB\n";
            }  
        }
    }

    }

    if (copiedItemPath != "") {
        std::cout << "\n";
        for(int i = 0; i < copiedItemPath.size() + 13; i++) {
            std::cout << "_";
        }
        
        std::cout << "\nClipboard: \033[0;1m" << copiedItemPath << "\033[0m |\n";
    }

    if (movingItemPath != "") {
        std::cout << "\n";
        for(int i = 0; i < movingItemPath.size() + 10; i++) {
            std::cout << "_";
        }
        
        std::cout << "\nMoving: \033[0;1m" << movingItemPath << "\033[0m |\n";
    }
}

bool removeItem(const std::string& path) {
    std::string rmCommand = "rm -r '" + path + "'";

    if (system(rmCommand.c_str()) != 0) {
        std::cerr << "Error removing directory: " << path << std::endl;
        return false;
    }

    return true;
}

void confirmAndDelete(const std::string& path) {
    std::cout << "\n\nAre you sure you want to delete this item: \033[0;1m" << path << "\033[0m? (y/n)\n";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
        if (removeItem(path)) {
            std::cout << "Item successfully deleted.\n";
        } else {
            std::cout << "Failed to delete item.\n";
        }
    } else {
        std::cout << "Deletion canceled.\n";
    }

    std::cout << "\nPress " << "\033[0;1m" << "any key" << "\033[0m" << " to continue: ";
    std::cin.ignore();
}

void createItem(const std::string& path, bool isFolder) {
    if (isFolder) {
        mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        std::ofstream file(path);
    }
}

void searchFile(const char* dir_name, const std::string& item_name, std::vector<std::string>& results) {
    DIR* dir = opendir(dir_name);
    if (dir == nullptr) {
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] != '.' && entry->d_type == DT_DIR && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string subdir_name = std::string(dir_name) + "/" + entry->d_name;
            searchFile(subdir_name.c_str(), item_name, results);
        } else if (entry->d_name[0] != '.' && entry->d_type == DT_REG && strcmp(entry->d_name, item_name.c_str()) == 0) {
            std::string file_path = std::string(dir_name) + "/" + entry->d_name;
            results.push_back(file_path);
        }
    }

    closedir(dir);
}

int main() {
    start_screen();

    std::string start_dir = "//home";
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

    displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);

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
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (input == 'q') { // quit
            break;
        }

        else if (input == '\n') { // Enter
            if (!items.empty() && items[currentSelection].is_folder) { 
                start_dir += "/" + items[currentSelection].name;
                items = set_list_of_current_dir(start_dir); 
                currentSelection = 0; 
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
            }
        }

        else if (input == 127) { // Backspace
            if (start_dir != "/" && start_dir != "/home") {
                size_t pos = start_dir.find_last_of('/');
                if (pos != std::string::npos) {
                    start_dir = start_dir.substr(0, pos);
                    items = set_list_of_current_dir(start_dir); 
                    currentSelection = 0; 
                    displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
                }
            } else {
                start_dir = "/";
                items = set_list_of_current_dir(start_dir); 
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
            }

        }

        else if (input == '+') { // create item in curr dir 
            std::cout << "\033c";
            std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
            for(int i = 0; i < start_dir.size() + 3; i++) {
                std::cout << "_";            // —
            }
            std::cout << "|\n\n";

            std::cout << "Create item\n";
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
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (!items.empty() && input == '-') { // delete item
            std::string path = start_dir + "/" + items[currentSelection].name;

            confirmAndDelete(path);

            items = set_list_of_current_dir(start_dir); 
            currentSelection = 0; 
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (!items.empty() && input == 'r') { // rename
            std::cout << "\033c";
            std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
            for(int i = 0; i < start_dir.size() + 3; i++) {
                std::cout << "_";            // —
            }
            std::cout << "|\n\n";

            std::cout << "Rename this item: \033[0;1m'" << items[currentSelection].name << "'\033[0m\n";
            std::cout << "Enter new name: ";
            std::string newName;
            std::cin >> newName;
            std::string newPath = start_dir + "/" + newName;
            
            struct stat buffer;
            while (stat(newPath.c_str(), &buffer) == 0) {
                std::cout << "\n\nItem with the name '\033[0;1m" << newName << "\033[0m' already exists in this directory\n";
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
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
            
        }

        else if (!items.empty() && input == 'c') { //copy
            copiedItemPath = start_dir + "/" + items[currentSelection].name;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (input == 'v' && copiedItemPath != "") { // paste
            std::string command = "cp -r '" + copiedItemPath + "' '" + start_dir + "'";
            system(command.c_str());

            copiedItemPath = "";
            items = set_list_of_current_dir(start_dir);
            if (items.size() == 1) {
                currentSelection = 0;
            }
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (input == 'm') { // move
            if (isMoving) {
                std::string command = "mv '" + movingItemPath + "' '" + start_dir + "'";
                system(command.c_str());

                movingItemPath = "";
                items = set_list_of_current_dir(start_dir);
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
                isMoving = false;
            } else {
                movingItemPath = start_dir + "/" + items[currentSelection].name;
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
                isMoving = true;
            }
        }

        else if (input == 's') { // search
            std::cout << "\033c";
            
            std::string item_name;
            std::cout << "Search files |\n";
            std::cout << "-------------|\n";
            std::cout << "Enter file name: ";
            std::cin >> item_name;

            std::vector<std::string> results;

            searchFile("/home", item_name, results); 

            if (!results.empty()) {
                std::cout << "\n\nFile(s) found:\n";
                for (const auto& file : results) {
                    std::cout << "\033[0;1m" << file << "\033[0m\n"; 
                }
            } else {
                std::cout << "\n\nFile '\033[0;1m" << item_name << "\033[0m' not found :(\n";
            }

            std::cout << "\nPress " << "\033[0;1m" << "Backspace" << "\033[0m" << " to quit search: ";
            char tempInput;
            while (true) {
                std::cin >> tempInput;
                if (tempInput == 127) {
                    break;
                }
            }

            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (input == 9){ // Tab
            std::cout << "\033c";
            start_screen();
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}


void start_screen() {
    std::cout << "Navigation                            \033[32m            ________  ______  __        ________                                                 \033[0m\n";
    std::cout << "======================================\033[32m           |        \\|      \\|  \\      |        \\                                            \033[0m\n";
    std::cout << "\033[0;1mTab\033[0m       |show options              |\033[32m           | $$$$$$$$ \\$$$$$$| $$      | $$$$$$$$                                               \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$__      | $$  | $$      | $$__                                                    \033[0m\n";
    std::cout << "\033[0;1m↓ ↑\033[0m       |navigate through the items|\033[32m           | $$  \\     | $$  | $$      | $$  \\                                                 \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$$$$     | $$  | $$      | $$$$$                                                   \033[0m\n";
    std::cout << "\033[0;1mEnter\033[0m     |enter dir                 |\033[32m           | $$       _| $$_ | $$_____ | $$_____                                                 \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$      |   $$ \\| $$     \\| $$     \\                                             \033[0m\n";
    std::cout << "\033[0;1mBackspace\033[0m |back to previous dir      |\033[32m            \\$$       \\$$$$$$ \\$$$$$$$$ \\$$$$$$$$                                            \033[0m\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "\033[0;1mq\033[0m         |quit                      |\033[32m            __       __   ______   __    __   ______    ______   ________  _______               \033[0m\n";
    std::cout << "======================================\033[32m           |  \\     /  \\ /      \\ |  \\  |  \\ /      \\  /      \\ |        \\|       \\     \033[0m\n";
    std::cout << "                                      \033[32m           | $$\\   /  $$|  $$$$$$\\| $$\\ | $$|  $$$$$$\\|  $$$$$$\\| $$$$$$$$| $$$$$$$\\       \033[0m\n";
    std::cout << "Edit                                  \033[32m           | $$$\\ /  $$$| $$__| $$| $$$\\| $$| $$__| $$| $$ ____ | $$__    | $$__| $$           \033[0m\n";
    std::cout << "======================================\033[32m           | $$$$\\  $$$$| $$   \\$$| $$$$\\ $$| $$   \\$$| $$|    \\| $$  \\   | $$   \\$$      \033[0m\n";
    std::cout << "\033[0;1m+\033[0m         |create item in current dir|\033[32m           | $$\\$$ $$ $$| $$$$$$$$| $$\\$$ $$| $$$$$$$$| $$ \\$$$$| $$$$$   | $$$$$$$\\         \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m           | $$ \\$$$| $$| $$  | $$| $$ \\$$$$| $$  | $$| $$__| $$| $$_____ | $$  | $$           \033[0m\n";
    std::cout << "\033[0;1m-\033[0m         |delete selected item      |\033[32m           | $$  \\$ | $$| $$  | $$| $$  \\$$$| $$  | $$ \\$$    $$| $$     \\| $$  | $$         \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m            \\$$      \\$$ \\$$   \\$$ \\$$   \\$$ \\$$   \\$$  \\$$$$$$  \\$$$$$$$$ \\$$   \\$$ \033[0m\n";
    std::cout << "\033[0;1mr\033[0m         |rename selected item      |\n";
    std::cout << "======================================\n";
    std::cout << "\n";
    std::cout << "Manipulations\n";
    std::cout << "======================================           \033[33m┌─┐___\033[0m                  ┌───┐                 \033[33;2m┌───┐\033[0m\n";
    std::cout << "\033[0;1mc\033[0m         |copy selected item        |           \033[33m│  _  │\033[0m folder          │~~~│ document        \033[33;2m│ ↓ │\033[0m archive\n";
    std::cout << "----------|--------------------------|           \033[33m└─┴─┴─┘\033[0m                 └───┘                 \033[33;2m└───┘\n";
    std::cout << "\033[0;1mv\033[0m         |paste copied item         |\n";
    std::cout << "----------|--------------------------|           \033[34m┌───┐\033[31m                   ┌───┐                 \033[90m┌───┐\033[0m                 ┌───┐\n";
    std::cout << "\033[0;1mm\033[0m         |move selected item        |           \033[34m│◠˚✧│\033[0m image             \033[31m│ ▶ │ \033[0mvideo           \033[90m│ ♫ │\033[0m audio           │ ⚙ │ other\n";
    std::cout << "----------|--------------------------|           \033[34m└───┘\033[31m                   └───┘                 \033[90m└───┘\033[0m                 └───┘\n";
    std::cout << "\033[0;1ms\033[0m         |file search               |\n";
    std::cout << "======================================\n";

    std::cout << "\nPress \033[0;1mEnter\033[0m to continue: ";
    std::cin.ignore();
}



















/*        

     ________  ______  __        ________                                   
    |        \|      \|  \      |        \                                   
    | $$$$$$$$ \$$$$$$| $$      | $$$$$$$$                                   
    | $$__      | $$  | $$      | $$__                                       
    | $$  \     | $$  | $$      | $$  \                                         
    | $$$$$     | $$  | $$      | $$$$$                                      
    | $$       _| $$_ | $$_____ | $$_____                                    
    | $$      |   $$ \| $$     \| $$     \                                   
     \$$       \$$$$$$ \$$$$$$$$ \$$$$$$$$                                   
                                                                                                       
     __       __   ______   __    __   ______    ______   ________  _______
    |  \     /  \ /      \ |  \  |  \ /      \  /      \ |        \|       \ 
    | $$\   /  $$|  $$$$$$\| $$\ | $$|  $$$$$$\|  $$$$$$\| $$$$$$$$| $$$$$$$\
    | $$$\ /  $$$| $$__| $$| $$$\| $$| $$__| $$| $$ ____ | $$__    | $$__| $$
    | $$$$\  $$$$| $$   \$$| $$$$\ $$| $$   \$$| $$|    \| $$  \   | $$   \$$
    | $$\$$ $$ $$| $$$$$$$$| $$\$$ $$| $$$$$$$$| $$ \$$$$| $$$$$   | $$$$$$$\
    | $$ \$$$| $$| $$  | $$| $$ \$$$$| $$  | $$| $$__| $$| $$_____ | $$  | $$
    | $$  \$ | $$| $$  | $$| $$  \$$$| $$  | $$ \$$    $$| $$     \| $$  | $$
     \$$      \$$ \$$   \$$ \$$   \$$ \$$   \$$  \$$$$$$  \$$$$$$$$ \$$   \$$








 
|¯¯\___
|      |
|______|

|¯¯\___
│      │
└──────┘

┌─┐___
│     │
└─────┘



|¯¯¯¯|
|~~~~|
|____|

┌───┐
│~~~│
└───┘



 /¯¯¯¯/|
|¯¯¯¯| |
|____|/   






┘ ┌ └ ┐ ┴ ┬ ├ ─ ┼ ┤ │ ¯ 



folder
┌─┐___
│  _  │ 
└─┴─┴─┘

document
┌───┐
│~~~│
└───┘

archive
┌───┐
│↓↓↓│
└───┘

image
┌───┐
│◠˚✧|
└───┘

video
┌───┐
│ ▶ │
└───┘

audio
┌───┐
│ ♫ │
└───┘

other
┌───┐
│ ⚙ │
└───┘

*/



/*void start_screen() {
    std::cout << "Navigation\n";
    std::cout << "======================================\n";
    std::cout << "Tab       |show options              |\n";
    std::cout << "----------|--------------------------|\033[32m                [][][]  [][][]  []      [][][]                            \033[0m\n";
    std::cout << "↓ ↑       |navigate through the items|\033[32m                []        []    []      []                                \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m                [][]      []    []      [][][]                            \033[0m\n";
    std::cout << "Enter     |enter dir                 |\033[32m                []        []    []      []                                \033[0m\n";
    std::cout << "----------|--------------------------|\033[32m                []      [][][]  [][][]  [][][]                            \033[0m\n";
    std::cout << "Backspace |back to previous dir      |\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "q         |quit                      |\033[32m                []  []    []    []  []    []    [][][]  [][][]  [][][]    \033[0m\n";
    std::cout << "======================================\033[32m                [[][]]   [][]   []] []   [][]   []      []      []  []    \033[0m\n";
    std::cout << "                                      \033[32m                [][][]  []  []  [][][]  []  []  [] [[]  [][]    [][][]    \033[0m\n";
    std::cout << "Edit                                  \033[32m                []  []  [][][]  [] [[]  [][][]  []  []  []      [] []     \033[0m\n";
    std::cout << "======================================\033[32m                []  []  []  []  []  []  []  []  [][][]  [][][]  []  []    \033[0m\n";
    std::cout << "+         |create item in current dir|\n";
    std::cout << "----------|--------------------------|\n";
    std::cout << "-         |delete selected item      |\033[33;2m                 [[][][][][]                                            \033[0m\n";
    std::cout << "----------|--------------------------|\033[33;2m                []          [][]                                        \033[0m\n";
    std::cout << "r         |rename selected item      |\033[33;2m                []              [][][][][][][][][][][][]]               \033[0m\n";                
    std::cout << "======================================\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "                                      \033[33;2m                [][][][][][][][][][][][][][][][][][][][][]              \033[0m\n";
    std::cout << "Manipulations                         \033[33;2m                []                                      []              \033[0m\n";
    std::cout << "======================================\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "c         |copy selected item        |\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "----------|--------------------------|\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "v         |paste copied item         |\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "----------|--------------------------|\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "m         |move selected item        |\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "----------|--------------------------|\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "s         |file search               |\033[33;2m                []                                      []              \033[0m\n";
    std::cout << "======================================\033[33;2m                 [[][][][][][][][][][][][][][][][][][][]]               \033[0m\n";

    std::cout << "\nPress \033[0;1mEnter\033[0m to continue: ";
    std::cin.ignore();
}*/