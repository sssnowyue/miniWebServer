#pragma once

#include <functional>
#include <memory>

class Buffer;
class Connector;
class Timestamp;

using ConnectorPtr = std::shared_ptr<Connector>;
using MessageCallback =
    std::function<void(const ConnectorPtr &, Buffer *, Timestamp)>;
using WriteCompleteCallback = std::function<void(const ConnectorPtr &)>;