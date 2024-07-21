#pragma once

#include <vector>
#include <string>

namespace models {

struct Content {
    struct Image {
        size_t size;
        size_t width;
        size_t heigth;

        std::string name;
        std::string content;

        bool is_statement_image;
        bool is_solution_image;
    };

    std::string statement_text;
    std::string solution_text;

    std::vector<Image> images;
};

}