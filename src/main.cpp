#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/ugrpc/client/client_factory_component.hpp>
#include <userver/ugrpc/server/server_component.hpp>
#include <userver/utils/daemon_run.hpp>

#include "components/service/hello.hpp"
#include "userver/storages/postgres/component.hpp"

int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::ugrpc::server::ServerComponent>()
          .Append<userver::ugrpc::client::ClientFactoryComponent>()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::clients::dns::Component>()
          .Append<service::TasksBank>()
          .Append<userver::components::Postgres>("postgres-db")
          .Append<userver::server::handlers::TestsControl>();

  return userver::utils::DaemonMain(argc, argv, component_list);
}
