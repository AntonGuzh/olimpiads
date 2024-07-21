#pragma once

#include <string>
#include <string_view>

#include <handlers/tasks_bank_service.usrv.pb.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/component_context.hpp>

#include "../../db/db.hpp"

namespace service {

namespace api = olimpiads::astronomy::v1;

class TasksBank final : public api::TasksBankServiceBase::Component {
 public:
  static constexpr std::string_view kName = "task-bank-service";

  TasksBank(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : api::TasksBankServiceBase::Component(config, component_context),
        storage_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db")
                .GetCluster()) {}

  void FilterTasks(api::TasksBankServiceBase::FilterTasksCall& call,
                   api::FilterTasksRequest&& request);

  void CreateTask(api::TasksBankServiceBase::CreateTaskCall& call,
                  api::Task&& request);

 private:
  storage::TasksStorage storage_;
};

}  // namespace olimpiads
