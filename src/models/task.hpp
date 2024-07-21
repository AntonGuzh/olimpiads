#pragma once

#include <string>
#include <vector>
#include <optional>

#include "content.hpp"

namespace models {

struct Task {
    enum class Olimpiad {
        ALL_RUSSIAN_MUNICIPAL,
        ALL_RUSSIAN_REGION,
        ALL_RUSSIAN_FINAL,
        MAO_FIRST,
        MAO_SECOND,
        MAO_FINAL,
        SPB_QUALIFUING,
        SPB_FINAL,
        IAO,
        IOAA,
        UNDEFINED
    };

    enum class Tour {
        THEORETICAL,
        PRACTICAL,
        BLITZ,
        OBSERVATION,
        UNDEFINED
    };

    enum class Topic {
        TELESCOPE,
        POGSON,
        SPECTRUM,
        EMITTING,
        ABSORPTION,
        COSMOLOGY,
        ATMOSPHERE,
        SPACE_FLIGHTS,
        ORBIT_PARAMETERS,
        CONSERVATION_LAWS,
        CELESTIAL_SPHERE,
        GEOMETRY_AND_TIME,
        ECLIPS,
        PLANET_CONFIGURATIONS,
        CALENDARS,
        UNDEFINED
    };

    struct GradeAndNumber {
        int grade;
        int number;
    };

    std::string id;
    
    Olimpiad olimpiad;
    int year;
    std::vector<GradeAndNumber> grades_and_numbers;
    Tour tour;
    std::optional<std::string> author;
    std::optional<std::string> title;
    std::vector<Topic> topics;
    Content content;
};

}
