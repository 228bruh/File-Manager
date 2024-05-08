#include "/home/kirill/VS_Code_Files/Git/File-Manager/header/class_Item.h"

Item::Item() : is_folder(false), name(""), type(Type::Other), size(0) {}
Item::Item(const std::string& name, Type type, bool is_folder, double size)
    : name(name), type(type), is_folder(is_folder), size(size) {}

Item::Type Item::getFileType(const std::string& name) {
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
                        Item file;
                        item.type = file.getFileType(name);
                    }
                    items.push_back(item);
                }
            }
        }
        closedir(dir);

        std::sort(items.begin(), items.end(), compareItemsByName);
    } else {
        std::cerr << "Error opening directory\n";
    }
    return items;
}
