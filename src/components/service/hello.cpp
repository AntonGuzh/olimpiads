#include "hello.hpp"
#include <grpcpp/support/status.h>
#include <handlers/tasks_bank.pb.h>
#include <algorithm>
#include <exception>
#include <iterator>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <userver/utils/uuid4.hpp>
#include <utility>
#include <vector>

#include "../../exceptions/exceptions.hpp"
#include "userver/logging/log.hpp"

namespace service {

namespace {

models::Task::Olimpiad ConvertOlimpiad(api::Olimpiad olimpiad) {
  switch (olimpiad) {
    case api::Olimpiad::ALL_RUSSIAN_MUNICIPAL:
      return models::Task::Olimpiad::ALL_RUSSIAN_MUNICIPAL;
    case api::Olimpiad::ALL_RUSSIAN_REGION:
      return models::Task::Olimpiad::ALL_RUSSIAN_REGION;
    case api::Olimpiad::ALL_RUSSIAN_FINAL:
      return models::Task::Olimpiad::ALL_RUSSIAN_FINAL;
    case api::Olimpiad::MAO_FIRST:
      return models::Task::Olimpiad::MAO_FIRST;
    case api::Olimpiad::MAO_SECOND:
      return models::Task::Olimpiad::MAO_SECOND;
    case api::Olimpiad::MAO_FINAL:
      return models::Task::Olimpiad::MAO_FINAL;
    case api::Olimpiad::SPB_QUALIFUING:
      return models::Task::Olimpiad::SPB_QUALIFUING;
    case api::Olimpiad::SPB_FINAL:
      return models::Task::Olimpiad::SPB_FINAL;
    case api::Olimpiad::IAO:
      return models::Task::Olimpiad::IAO;
    case api::Olimpiad::IOAA:
      return models::Task::Olimpiad::IOAA;
    default:
      throw InvalidArgument{"Olimpiad must be one of enumerated values"};
  }
}

api::Olimpiad ConvertOlimpiad(models::Task::Olimpiad olimpiad) {
  switch (olimpiad) {
    case models::Task::Olimpiad::ALL_RUSSIAN_MUNICIPAL:
      return api::Olimpiad::ALL_RUSSIAN_MUNICIPAL;
    case models::Task::Olimpiad::ALL_RUSSIAN_REGION:
      return api::Olimpiad::ALL_RUSSIAN_REGION;
    case models::Task::Olimpiad::ALL_RUSSIAN_FINAL:
      return api::Olimpiad::ALL_RUSSIAN_FINAL;
    case models::Task::Olimpiad::MAO_FIRST:
      return api::Olimpiad::MAO_FIRST;
    case models::Task::Olimpiad::MAO_SECOND:
      return api::Olimpiad::MAO_SECOND;
    case models::Task::Olimpiad::MAO_FINAL:
      return api::Olimpiad::MAO_FINAL;
    case models::Task::Olimpiad::SPB_QUALIFUING:
      return api::Olimpiad::SPB_QUALIFUING;
    case models::Task::Olimpiad::SPB_FINAL:
      return api::Olimpiad::SPB_FINAL;
    case models::Task::Olimpiad::IAO:
      return api::Olimpiad::IAO;
    case models::Task::Olimpiad::IOAA:
      return api::Olimpiad::IOAA;
    case models::Task::Olimpiad::UNDEFINED:
      throw InternalError{"Olimpiad is undefined"};
  }
}

models::Task::Topic ConvertTopic(api::Topic topic) {
  switch (topic) {
    case api::Topic::TELESCOPE:
      return models::Task::Topic::TELESCOPE;
    case api::Topic::POGSON:
      return models::Task::Topic::POGSON;
    case api::Topic::SPECTRUM:
      return models::Task::Topic::SPECTRUM;
    case api::Topic::EMITTING:
      return models::Task::Topic::EMITTING;
    case api::Topic::ABSORPTION:
      return models::Task::Topic::ABSORPTION;
    case api::Topic::COSMOLOGY:
      return models::Task::Topic::COSMOLOGY;
    case api::Topic::ATMOSPHERE:
      return models::Task::Topic::ATMOSPHERE;
    case api::Topic::SPACE_FLIGHTS:
      return models::Task::Topic::SPACE_FLIGHTS;
    case api::Topic::ORBIT_PARAMETERS:
      return models::Task::Topic::ORBIT_PARAMETERS;
    case api::Topic::CONSERVATION_LAWS:
      return models::Task::Topic::CONSERVATION_LAWS;
    case api::Topic::CELESTIAL_SPHERE:
      return models::Task::Topic::CELESTIAL_SPHERE;
    case api::Topic::GEOMETRY_AND_TIME:
      return models::Task::Topic::GEOMETRY_AND_TIME;
    case api::Topic::ECLIPS:
      return models::Task::Topic::ECLIPS;
    case api::Topic::PLANET_CONFIGURATIONS:
      return models::Task::Topic::PLANET_CONFIGURATIONS;
    case api::Topic::CALENDARS:
      return models::Task::Topic::CALENDARS;
    default:
      throw InvalidArgument("Topic must be one of enumerated values");
  }
}

api::Topic ConvertTopic(models::Task::Topic topic) {
  switch (topic) {
    case models::Task::Topic::TELESCOPE:
      return api::Topic::TELESCOPE;
    case models::Task::Topic::POGSON:
      return api::Topic::POGSON;
    case models::Task::Topic::SPECTRUM:
      return api::Topic::SPECTRUM;
    case models::Task::Topic::EMITTING:
      return api::Topic::EMITTING;
    case models::Task::Topic::ABSORPTION:
      return api::Topic::ABSORPTION;
    case models::Task::Topic::COSMOLOGY:
      return api::Topic::COSMOLOGY;
    case models::Task::Topic::ATMOSPHERE:
      return api::Topic::ATMOSPHERE;
    case models::Task::Topic::SPACE_FLIGHTS:
      return api::Topic::SPACE_FLIGHTS;
    case models::Task::Topic::ORBIT_PARAMETERS:
      return api::Topic::ORBIT_PARAMETERS;
    case models::Task::Topic::CONSERVATION_LAWS:
      return api::Topic::CONSERVATION_LAWS;
    case models::Task::Topic::CELESTIAL_SPHERE:
      return api::Topic::CELESTIAL_SPHERE;
    case models::Task::Topic::GEOMETRY_AND_TIME:
      return api::Topic::GEOMETRY_AND_TIME;
    case models::Task::Topic::ECLIPS:
      return api::Topic::ECLIPS;
    case models::Task::Topic::PLANET_CONFIGURATIONS:
      return api::Topic::PLANET_CONFIGURATIONS;
    case models::Task::Topic::CALENDARS:
      return api::Topic::CALENDARS;
    case models::Task::Topic::UNDEFINED:
      throw InternalError("Topic is undefined");
  }
}

models::Task::Tour ConvertTour(api::Tour topic) {
  switch (topic) {
    case api::Tour::THEORETICAL:
      return models::Task::Tour::THEORETICAL;
    case api::Tour::PRACTICAL:
      return models::Task::Tour::PRACTICAL;
    case api::Tour::BLITZ:
      return models::Task::Tour::BLITZ;
    case api::Tour::OBSERVATION:
      return models::Task::Tour::OBSERVATION;
    default:
      throw InvalidArgument("Tour must be one of enumerated values");
  }
}

api::Tour ConvertTour(models::Task::Tour topic) {
  switch (topic) {
    case models::Task::Tour::THEORETICAL:
      return api::Tour::THEORETICAL;
    case models::Task::Tour::PRACTICAL:
      return api::Tour::PRACTICAL;
    case models::Task::Tour::BLITZ:
      return api::Tour::BLITZ;
    case models::Task::Tour::OBSERVATION:
      return api::Tour::OBSERVATION;
    default:
      throw InternalError("Tour is undefined");
  }
}

void ValidateCreateTaskRequest(api::Task& request) {
  request.clear_name();

  if (!api::Olimpiad_IsValid(request.olimpiad()) || request.olimpiad() == api::Olimpiad::UNDEFINED_OLIMPIAD) {
    throw InvalidArgument("Olimpiad is required");
  }

  if (request.year() == 0) {
    throw InvalidArgument("Year is required");
  }

  if (!api::Tour_IsValid(request.tour()) || request.tour() == api::Tour::UNDEFINED_TOUR) {
    throw InvalidArgument("Tour is required");
  }

  if (request.grades_and_numbers_size() == 0) {
    throw InvalidArgument("Grades and numbers are required");
  }

  if (request.topics_size() == 0) {
    throw InvalidArgument("Topics are required");
  }
  for (auto& topic : request.topics()) {
    if (topic == api::Topic::UNDEFINED_TOPIC) {
      throw InvalidArgument("There is/are undefined topic(s)");
    }
  }

  if (!request.has_content()) {
    throw InternalError("Content is required");
  }

  auto& content = request.content();

  if (content.statement_text().empty()) {
    throw InternalError("Statement is required");
  }

  if (content.solution_text().empty()) {
    throw InvalidArgument("Solution is required");
  }

  for (auto& img : content.images()) {
    if (img.name().empty()) {
      throw InvalidArgument("Image name is required");
    }

    if (img.width() == 0 || img.heigth() == 0) {
      throw InvalidArgument("Image width and heigth are required");
    }

    if (img.content().empty()) {
      throw InvalidArgument("Image content is required");
    }

    if (!img.is_statement_image() && !img.is_solution_image()) {
      throw InvalidArgument("Image type is required");
    }

    if (static_cast<size_t>(img.size()) != img.content().size()) {
      throw InvalidArgument(fmt::format("Image size is incorect. Real size: {}, given size: {}", img.content().size(), img.size()));
    }
  }
}

void ValidateFilterTasksRequest(api::FilterTasksRequest& request) {
  if (request.page_size() <= 0) {
    request.set_page_size(10);
  }
  if (request.page_size() > 50) {
    request.set_page_size(50);
  }

  for (auto olimpiad : request.filter().olimpiads()) {
    if (!api::Olimpiad_IsValid(olimpiad) || olimpiad == api::Olimpiad::UNDEFINED_OLIMPIAD) {
      throw InvalidArgument("There is/are undefined olimpiad(s)");
    }
  }

  for (auto topic : request.filter().included_topics()) {
    if (!api::Topic_IsValid(topic) || topic == api::Topic::UNDEFINED_TOPIC) {
      throw InvalidArgument("There is/are undefined topic(s) in included topics");
    }
  }

  for (auto topic : request.filter().excluded_topics()) {
    if (!api::Topic_IsValid(topic) || topic == api::Topic::UNDEFINED_TOPIC) {
      throw InvalidArgument("There is/are undefined topic(s) in excluded topics");
    }
  }

  if (request.has_filter() && !request.filter().included_topics().empty() && !request.filter().excluded_topics().empty()) {
    std::set<int> included_topics{request.filter().included_topics().begin(), request.filter().included_topics().end()};
    std::set<int> exluded_topics{request.filter().excluded_topics().begin(), request.filter().excluded_topics().end()};
    std::vector<int> intersection;
    std::set_intersection(included_topics.begin(), included_topics.end(), exluded_topics.begin(), exluded_topics.end(), std::back_inserter(intersection));
    if (!intersection.empty()) {
      throw InvalidArgument("There are same topics in included and exluded topics");
    }
  }
}

models::Filter ConvertFilter(const api::Filter& filter) {
  models::Filter res;

  if (!filter.olimpiads().empty()) {
    res.olimpiads = std::vector<models::Task::Olimpiad>{};
    std::transform(filter.olimpiads().begin(), filter.olimpiads().end(), std::back_inserter(res.olimpiads.value()), [](int x){return ConvertOlimpiad(api::Olimpiad(x));});
  }
  if (filter.start_year() != 0) {
    res.start_year = filter.start_year();
  }
  if (filter.end_year() != 0) {
    res.end_year = filter.end_year();
  }
  if (api::Tour_IsValid(filter.tour()) && filter.tour() != api::Tour::UNDEFINED_TOUR) {
    res.tour = ConvertTour(filter.tour());
  }
  if (!filter.grades().empty()) {
    res.grades={filter.grades().begin(), filter.grades().end()};
  }
  if (!filter.included_topics().empty()) {
    res.included_topics = std::vector<models::Task::Topic>{};
    std::transform(filter.included_topics().begin(), filter.included_topics().end(), std::back_inserter(res.included_topics.value()), [](int x){return ConvertTopic(api::Topic(x));});
  }
  if (!filter.excluded_topics().empty()) {
    res.excluded_topics = std::vector<models::Task::Topic>{};
    std::transform(filter.excluded_topics().begin(), filter.excluded_topics().end(), std::back_inserter(res.excluded_topics.value()), [](int x){return ConvertTopic(api::Topic(x));});
  }

  return res;
}

}

void TasksBank::CreateTask(api::TasksBankServiceBase::CreateTaskCall& call,
                  api::Task&& request) try {
  ValidateCreateTaskRequest(request);

  models::Task task{
    .id=userver::utils::generators::GenerateUuid(),
    .olimpiad=ConvertOlimpiad(request.olimpiad()),
    .year=request.year(),
    .tour=ConvertTour(request.tour()),
    .author=request.author().empty() ? std::optional<std::string>{std::nullopt} : request.author(),
    .title=request.title().empty() ? std::optional<std::string>{std::nullopt}: request.title(),
    .content = models::Content{
      .statement_text=request.content().statement_text(),
      .solution_text=request.content().solution_text(),
    },
  };
  std::transform(request.topics().begin(), request.topics().end(), std::back_inserter(task.topics), [](int x){return ConvertTopic(api::Topic(x));});
  std::transform(request.grades_and_numbers().begin(), request.grades_and_numbers().end(), std::back_inserter(task.grades_and_numbers), [](const api::GradeNumber& grade_and_number){return models::Task::GradeAndNumber{grade_and_number.grade(), grade_and_number.number()};});
  std::transform(request.content().images().begin(), request.content().images().end(), std::back_inserter(task.content.images), [](const api::Image& image){return models::Content::Image{
    .size=static_cast<size_t>(image.size()),
    .width=static_cast<size_t>(image.width()),
    .heigth=static_cast<size_t>(image.heigth()),
    .name=image.name(),
    .content=image.content(),
    .is_statement_image=image.is_statement_image(),
    .is_solution_image=image.is_solution_image(),
  };});

  storage_.InsertTask(task);

  request.set_name(task.id);
  call.Finish(request);
} catch (const InvalidArgument& e) {
  call.FinishWithError({grpc::StatusCode::INVALID_ARGUMENT, e.what()});
} catch (const ArleadyExists& e) {
  call.FinishWithError({grpc::StatusCode::ALREADY_EXISTS, e.what()});
} catch (const std::exception& e) {
  call.FinishWithError({grpc::StatusCode::INTERNAL, e.what()});
}

void TasksBank::FilterTasks(api::TasksBankServiceBase::FilterTasksCall& call,
                   api::FilterTasksRequest&& request) try {
  LOG_INFO() << "START FILTER HANDLER";
  ValidateFilterTasksRequest(request);
  LOG_INFO() << "VALIDATE SUCCESFUL";

  auto filter = ConvertFilter(request.filter());

  LOG_INFO() << "CONVERT FILTER SUCCESFUL";

  auto tasks = storage_.FilterTasks(filter, "", request.page_size());
  LOG_INFO() << "DB QUERY SUCCESFUL";

  api::ListTasksResponse response;
  response.set_next_page_token(tasks.back().id);
  for (const auto& task : tasks) {
    auto api_task = response.add_tasks();

    api_task->set_name(task.id);
    api_task->set_olimpiad(ConvertOlimpiad(task.olimpiad));
    api_task->set_year(task.year);
    api_task->set_tour(ConvertTour(task.tour));

    for (const auto& [grade, number] : task.grades_and_numbers) {
      auto api_grade_and_number = api_task->add_grades_and_numbers();
      api_grade_and_number->set_grade(grade);
      api_grade_and_number->set_number(number);
    }

    for (const auto& topic : task.topics) {
      api_task->add_topics(ConvertTopic(topic));
    }

    if (task.author.has_value()) {
      api_task->set_author(task.author.value());
    }
    if (task.title.has_value()) {
      api_task->set_title(task.title.value());
    }

    auto api_content = api_task->mutable_content();

    api_content->set_statement_text(task.content.statement_text);
    api_content->set_solution_text(task.content.solution_text);

    for (const auto& img : task.content.images) {
      auto api_image = api_content->add_images();

      api_image->set_name(img.name);
      api_image->set_size(img.size);
      api_image->set_width(img.width);
      api_image->set_heigth(img.heigth);
      api_image->set_content(img.content);
      api_image->set_is_statement_image(img.is_statement_image);
      api_image->set_is_solution_image(img.is_solution_image);
    }

  }

  call.Finish(response);
} catch (const InvalidArgument& e) {
  call.FinishWithError({grpc::StatusCode::INVALID_ARGUMENT, e.what()});
} catch (const ArleadyExists& e) {
  call.FinishWithError({grpc::StatusCode::ALREADY_EXISTS, e.what()});
} catch (const std::exception& e) {
  call.FinishWithError({grpc::StatusCode::INTERNAL, e.what()});
}

}  // namespace olimpiads
