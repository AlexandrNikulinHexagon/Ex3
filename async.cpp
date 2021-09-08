#include "async.h"
#include "command.h"

#include <strstream>
#include <boost/asio.hpp>

namespace async {

handle_t connect(std::size_t bulk)
{
    return new BulkCommandOperator(bulk);
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    BulkCommandOperator *bulkCommandOperator = reinterpret_cast<BulkCommandOperator*>(handle);

    std::istrstream ss(data, size);
    for (std::string line; std::getline(ss, line);)
    {
        if (line == "{")
            bulkCommandOperator->startBulk();
        else if (line == "}")
            bulkCommandOperator->endBulk();
        else if (!line.empty())
            bulkCommandOperator->addCommand(line);
    }

}

void disconnect(handle_t handle)
{
    delete reinterpret_cast<BulkCommandOperator*>(handle);
}

}
