#include "fileio.h"

void openFile(std::ofstream& file_obj, const std::string& filename) {
    file_obj.open(filename);
}
