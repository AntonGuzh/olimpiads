#pragma once

#include <ranges>

namespace utils::ranges {

template<class Container>
class To {};

template<class Container, class Range>
Container operator|(Range&& range, To<Container>&&) {
  return Container{std::begin(range), std::end(range)};
}

template<class Container, class Range>
Container operator|(Range&& range, const To<Container>&) {
  return Container{std::begin(range), std::end(range)};
}

}