#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/options.h"

void createItem(std::string start_dir) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";
    }
    std::cout << "|\n\n";

    std::cout << "Create item\n";
    std::cout << "Enter name: ";
    std::string itemName;

    std::cin >> itemName;
            
    std::string itemPath = start_dir + "/" + itemName;
        
    struct stat buffer;
    while (stat(itemPath.c_str(), &buffer) == 0) {
        std::cout << "\n\nItem with the same name '\033[0;1m" << itemName << "\033[0m' already exists in this directory\n";
        std::cout << "Enter name: ";
                
        std::cin >> itemName;

        itemPath = start_dir + "/" + itemName;
        if (stat(itemPath.c_str(), &buffer) != 0) {
            break;
        }
    }

    bool isFolder = (itemName.find('.') == std::string::npos); 

    if (isFolder) {
        mkdir(itemPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    } else {
        std::ofstream file(itemPath);
    }
}

bool removeItem(const std::string& path) {
    std::string rmCommand = "rm -r '" + path + "'";

    if (system(rmCommand.c_str()) != 0) {
        std::cerr << "Error removing directory: " << path << "\n";
        return false;
    }

    return true;
}

void confirmAndDelete(const std::string& path) { 
    std::cout << "\n\nAre you sure you want to delete this item: \033[0;1m" << path << "\033[0m? [Y/n]";
    char response;
    std::cin >> response;

    if (response == 'y' || response == 'Y') {
   
        if (removeItem(path)) {
            std::cout << "\nItem successfully deleted\n";
        } else {
            std::cout << "\nFailed to delete item\n";
        }

    } else {
        std::cout << "\nDeletion canceled\n";
    }

    std::cout << "\nPress \033[0;1mEnter\033[0m to continue: ";
    std::cin.ignore();
}

void renameItem(std::string start_dir, std::vector<Item>& items, int currentSelection) {
    std::cout << "\033c";
    std::cout << " \033[0;1m" << start_dir << "  \033[0m|\n";
    for(int i = 0; i < start_dir.size() + 3; i++) {
        std::cout << "_";            
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

void searchMenu() {
    std::cout << "\033c";
            
    std::string item_name;
    std::cout << "Search files |\n";
    std::cout << "_____________|\n\n";
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

    std::cout << "\nPress \033[0;1mEnter\033[0m to continue: ";
    std::cin.ignore();
    std::cin.ignore();
}
