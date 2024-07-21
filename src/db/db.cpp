#include "db.hpp"

#include <algorithm>
#include <exception>
#include <functional>
#include <iterator>
#include <limits>
#include <optional>
#include <string>
#include <unordered_map>
#include <ranges>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/utils/trivial_map.hpp>

#include <utility>
#include <vector>

#include "impl/sql.hpp"
#include "../utils/ranges.hpp"
#include "../exceptions/exceptions.hpp"

namespace storage {

namespace {

// Do not change it. Else you have to migrate db. You mast only add new cases.
constexpr userver::utils::TrivialBiMap kOlimpiadMap = [](auto selector) {
  return selector()
      .Case("ALL_RUSSIAN_MUNICIPAL", models::Task::Olimpiad::ALL_RUSSIAN_MUNICIPAL)
      .Case("ALL_RUSSIAN_REGION", models::Task::Olimpiad::ALL_RUSSIAN_REGION)
      .Case("ALL_RUSSIAN_FINAL", models::Task::Olimpiad::ALL_RUSSIAN_FINAL)
      .Case("MAO_FIRST", models::Task::Olimpiad::MAO_FIRST)
      .Case("MAO_SECOND", models::Task::Olimpiad::MAO_SECOND)
      .Case("MAO_FINAL", models::Task::Olimpiad::MAO_FINAL)
      .Case("SPB_QUALIFUING", models::Task::Olimpiad::SPB_QUALIFUING)
      .Case("SPB_FINAL", models::Task::Olimpiad::SPB_FINAL)
      .Case("IAO", models::Task::Olimpiad::IAO)
      .Case("IOAA", models::Task::Olimpiad::IOAA);
};

// Do not change it. Else you have to migrate db. You mast only add new cases.
constexpr userver::utils::TrivialBiMap kTourMap = [](auto selector) {
  return selector()
      .Case("THEORETICAL", models::Task::Tour::THEORETICAL)
      .Case("PRACTICAL", models::Task::Tour::PRACTICAL)
      .Case("BLITZ", models::Task::Tour::BLITZ)
      .Case("OBSERVATION", models::Task::Tour::OBSERVATION);
};

// Do not change it. Else you have to migrate db. You mat only add new cases.
constexpr userver::utils::TrivialBiMap kTopicMap = [](auto selector) {
  return selector()
      .Case("TELESCOPE", models::Task::Topic::TELESCOPE)
      .Case("POGSON", models::Task::Topic::POGSON)
      .Case("SPECTRUM", models::Task::Topic::SPECTRUM)
      .Case("EMITTING", models::Task::Topic::EMITTING)
      .Case("ABSORPTION", models::Task::Topic::ABSORPTION)
      .Case("COSMOLOGY", models::Task::Topic::COSMOLOGY)
      .Case("ATMOSPHERE", models::Task::Topic::ATMOSPHERE)
      .Case("SPACE_FLIGHTS", models::Task::Topic::SPACE_FLIGHTS)
      .Case("ORBIT_PARAMETERS", models::Task::Topic::ORBIT_PARAMETERS)
      .Case("CONSERVATION_LAWS", models::Task::Topic::CONSERVATION_LAWS)
      .Case("CELESTIAL_SPHERE", models::Task::Topic::CELESTIAL_SPHERE)
      .Case("GEOMETRY_AND_TIME", models::Task::Topic::GEOMETRY_AND_TIME)
      .Case("ECLIPS", models::Task::Topic::ECLIPS)
      .Case("PLANET_CONFIGURATIONS", models::Task::Topic::PLANET_CONFIGURATIONS)
      .Case("CALENDARS", models::Task::Topic::CALENDARS);
};

std::vector<std::string> CreateTopicsVector(const std::vector<models::Task::Topic>& topics) {
  std::vector<std::string> res;
  res.reserve(topics.size());
  std::transform(topics.begin(), topics.end(), std::back_inserter(res), [](models::Task::Topic topic){
    return std::string{kTopicMap.TryFind(topic).value()};
  });
  return res;
}

std::vector<models::Task::Topic> CreateTopicsVector(const std::vector<std::string>& topics) {
  std::vector<models::Task::Topic> res;
  res.reserve(topics.size());
  std::transform(topics.begin(), topics.end(), std::back_inserter(res), [](const std::string& topic){
    return kTopicMap.TryFind(topic).value_or(models::Task::Topic::UNDEFINED);
  });
  return res;
}

std::optional<std::vector<std::string>> CreateOlimpiadsVectorOpt(const std::optional<std::vector<models::Task::Olimpiad>>& olimpiads) {
  LOG_INFO() << "CREATE OLIMPIADS VECTOR OPT";
  if (!olimpiads.has_value()) {
    return std::nullopt;
  }
  std::vector<std::string> res;
  res.reserve(olimpiads->size());
  std::transform(olimpiads->begin(), olimpiads->end(), std::back_inserter(res), [](models::Task::Olimpiad olimpiad){
    return std::string{kOlimpiadMap.TryFind(olimpiad).value()};
  });
  LOG_INFO() << "CREATE OLIMPIADS VECTOR OPT FINISH";
  return res;
}

std::optional<std::vector<std::string>> CreateTopicsVectorOpt(const std::optional<std::vector<models::Task::Topic>>& topics) {
  LOG_INFO() << "CREATE TOPICS VECTOR OPT";
  if (!topics.has_value()) {
    return std::nullopt;
  }
  std::vector<std::string> res;
  res.reserve(topics->size());
  std::transform(topics->begin(), topics->end(), std::back_inserter(res), [](models::Task::Topic topic){
    return std::string{kTopicMap.TryFind(topic).value()};
  });
  LOG_INFO() << "CREATE TOPICS VECTOR OPT FINISH";
  return res;
}

int GetTaskHash(const std::string& olimpiad, int year, int grade, int number, const std::string& tour) {
  size_t res = 0;
  std::hash<std::string> string_hasher;
  boost::hash_combine(res, string_hasher(olimpiad));
  boost::hash_combine(res, year);
  boost::hash_combine(res, grade);
  boost::hash_combine(res, number);
  boost::hash_combine(res, string_hasher(tour));
  return res % std::numeric_limits<int>::max();
}

void AddImage(std::vector<models::Content::Image>& images, const userver::storages::postgres::Row& row) {
  images.push_back(models::Content::Image{
    .size=static_cast<size_t>(row["img_size"].As<int>()),
    .width=static_cast<size_t>(row["img_width"].As<int>()),
    .heigth=static_cast<size_t>(row["img_height"].As<int>()),
    .name=row["img_name"].As<std::string>(),
    .content=row["img_content"].As<std::string>(),
    .is_statement_image=row["is_statement_img"].As<bool>(),
    .is_solution_image=row["is_solution_img"].As<bool>()
  });
}

}

TasksStorage::TasksStorage(userver::storages::postgres::ClusterPtr pg_cluster)
      : pg_cluster_(pg_cluster) {}

void TasksStorage::InsertTask(const models::Task& task) {
  auto tx = pg_cluster_->Begin(userver::storages::postgres::ClusterHostType::kMaster, {});
  try {
    auto grades = task.grades_and_numbers | std::views::transform([](
      const models::Task::GradeAndNumber& grade_and_nubmer){
      return grade_and_nubmer.grade;}) | utils::ranges::To<std::vector<int>>();
    std::vector<std::string> grades_and_numbers = task.grades_and_numbers | std::views::transform([](const auto& grade_and_number){return fmt::format("{}:{}", grade_and_number.grade, grade_and_number.number);}) | utils::ranges::To<std::vector<std::string>>();
    tx.Execute( //
      sql::query::kInsertTask, //
      task.id,
      kOlimpiadMap.TryFind(task.olimpiad).value(), //
      task.year, //
      kTourMap.TryFind(task.tour).value(), //
      grades, //
      grades_and_numbers, //
      CreateTopicsVector(task.topics), //
      task.author, //
      task.title, //
      task.content.statement_text,
      task.content.solution_text
    );

    for (const auto& [grade, number] : task.grades_and_numbers) {
      int hash = GetTaskHash(
        std::string{kOlimpiadMap.TryFind(task.olimpiad).value()},
        task.year, grade, number,
        std::string{kTourMap.TryFind(task.tour).value()});
      tx.Execute(
        sql::query::kInsertTaskHash,
        hash,
        task.id
      );
    }

    for (const auto& image : task.content.images) {
      tx.Execute(
        sql::query::kInsertImage,
        task.id,
        static_cast<int>(image.size),
        static_cast<int>(image.width),
        static_cast<int>(image.heigth),
        image.name,
        image.content,
        image.is_statement_image,
        image.is_solution_image
      );
    }

    tx.Commit();
  } catch(const userver::storages::postgres::UniqueViolation& ex) {
    tx.Rollback();
    throw ArleadyExists("This task already exists");
  } catch (std::exception& ex) {
    tx.Rollback();
    throw;
  }
}

std::vector<models::Task> TasksStorage::FilterTasks(const models::Filter& filter, std::string_view offset, int limit) const {
  auto response = pg_cluster_->Execute(userver::storages::postgres::ClusterHostType::kSlaveOrMaster,
    sql::query::kFilterTasks,
    CreateOlimpiadsVectorOpt(filter.olimpiads),
    filter.start_year,
    filter.end_year,
    filter.tour.has_value() ? kTourMap.TryFind(filter.tour.value()) : std::nullopt,
    filter.grades,
    CreateTopicsVectorOpt(filter.included_topics),
    CreateTopicsVectorOpt(filter.excluded_topics),
    offset,
    limit
  );

  std::unordered_map<std::string, models::Task> map_id_task;
  for (auto row : response) {
    auto task_id = row["id"].As<std::string>();
    if (auto it = map_id_task.find(task_id); it == map_id_task.end()) {
      std::vector<std::string> raw_grade_and_number = row["grades_and_nubmers"].As<std::vector<std::string>>();
      auto grades_and_numbers = raw_grade_and_number | std::views::transform([](const std::string& grade_and_number_str) -> models::Task::GradeAndNumber {
            std::vector<std::string> grade_and_number;
            boost::split(grade_and_number, grade_and_number_str, boost::is_any_of(":"));
            return models::Task::GradeAndNumber{std::stoi(grade_and_number[0]), std::stoi(grade_and_number[1])};
          }) | utils::ranges::To<std::vector<models::Task::GradeAndNumber>>();

      std::vector<models::Content::Image> images;
      if (!row["img_name"].IsNull()) {
        AddImage(images, row);
      }

      map_id_task.insert(std::pair{task_id, models::Task{
        .id=task_id,
        .olimpiad=kOlimpiadMap.TryFind(row["olimpiad"].As<std::string>()).value_or(models::Task::Olimpiad::UNDEFINED),
        .year=row["year"].As<int>(),
        .grades_and_numbers=grades_and_numbers,
        .tour=kTourMap.TryFind(row["tour"].As<std::string>()).value_or(models::Task::Tour::UNDEFINED),
        .author=row["author"].As<std::optional<std::string>>(),
        .title=row["title"].As<std::optional<std::string>>(),
        .topics=CreateTopicsVector(row["topics"].As<std::vector<std::string>>()),
        .content=models::Content{
          .statement_text=row["task_statement"].As<std::string>(),
          .solution_text=row["task_solution"].As<std::string>(),
          .images = images,
        },
      }});
    } else {
      AddImage(it->second.content.images, row);
    }
  }

  std::vector<models::Task> tasks;
  for (auto&& [id, task] : map_id_task) {
    tasks.push_back(std::move(task));
  } 
  return tasks;
}

}