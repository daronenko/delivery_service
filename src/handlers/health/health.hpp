#pragma once

#include <userver/components/component_list.hpp>


namespace delivery_service::handlers::health {

void AppendHealth(userver::components::ComponentList& component_list);

}  // namespace delivery_service::handlers::health
