#pragma once

#include <optional>
#include <vector>
#include "task.hpp"

namespace models {

struct Filter {
    std::optional<std::vector<Task::Olimpiad>> olimpiads;
    std::optional<int32_t> start_year;
    std::optional<int32_t> end_year;
    std::optional<Task::Tour> tour;
    std::optional<std::vector<int32_t>> grades;
    std::optional<std::vector<Task::Topic>> included_topics;
    std::optional<std::vector<Task::Topic>> excluded_topics;
};

}
