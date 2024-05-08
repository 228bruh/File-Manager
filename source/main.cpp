#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/class_Item.h"
#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/graphic_Functions.h"
#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/options.h" 

struct termios oldt, newt;

int main() {

    std::string start_dir = "//home";
    std::vector<Item> items = set_list_of_current_dir(start_dir);

    int currentSelection = 0;

    std::string copiedItemPath;
    std::string movingItemPath;
    bool isMoving = false;

    disableCanonicInput(oldt, newt);

    start_screen();

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

        else if (input == '+') { // create item
            enableCanonicInput(oldt);
            createItem(start_dir);
            disableCanonicInput(oldt, newt);

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

        else if (!items.empty() && input == 'r') { // rename item
            enableCanonicInput(oldt);
            renameItem(start_dir, items, currentSelection);
            disableCanonicInput(oldt, newt);

            items = set_list_of_current_dir(start_dir);
            currentSelection = 0;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);   
        }

        else if (!items.empty() && input == 'c') { //copy
            copiedItemPath = start_dir + "/" + items[currentSelection].name;
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (copiedItemPath != "" && input == 'v') { // paste
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
            } else if (!items.empty()){
                movingItemPath = start_dir + "/" + items[currentSelection].name;
                displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
                isMoving = true;
            }
        }

        else if (input == 's') { // search
            enableCanonicInput(oldt);
            searchMenu();
            disableCanonicInput(newt, oldt);

            char ch;
            do {
                ch = std::cin.get();
            } while (ch != 127);

            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

        else if (input == 9){ // Tab
            std::cout << "\033c";
            start_screen();
            displayFiles(items, currentSelection, copiedItemPath, movingItemPath, start_dir);
        }

    }

    enableCanonicInput(oldt);
    return 0;
}
