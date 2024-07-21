#pragma once

#include <handlers/tasks_bank.pb.h>
#include <string>
#include <string_view>

#include <handlers/tasks_bank_service.usrv.pb.hpp>
#include <userver/components/loggable_component_base.hpp>
#include <userver/components/component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/components/component_context.hpp>

#include "../models/task.hpp"
#include "../models/filter.hpp"

namespace storage {

class TasksStorage {
 public:
  TasksStorage(userver::storages::postgres::ClusterPtr pg_cluster);

  void InsertTask(const models::Task& task);

  std::vector<models::Task> FilterTasks(const models::Filter& filter, std::string_view offset, int limit) const;

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace storage
