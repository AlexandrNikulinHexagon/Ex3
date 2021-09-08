#include <iostream>

#include "async.h"
#include "logger.h"

int main(int, char *[]) {
    Logger::add_pool<ConsoleLoggerHandler>(1);
    Logger::add_pool<FileLoggerHandler>(2);

    std::size_t bulk = 5;
    auto h = async::connect(bulk);
    auto h2 = async::connect(bulk);

/*    async::receive(h, "1", 1);
    async::receive(h2, "1\n", 2);
    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
    async::receive(h, "b\nc\nd\n}\n89\n", 11);*/

    std::thread t1([&]() {
        for (int i = 0; i < 8; i++)
        {
            async::receive(h, "1", 1);
            async::receive(h, "1\n", 2);
            async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
            async::receive(h, "b\nc\nd\n}\n89\n", 11);
        }
    });

    std::thread t2([&]() {
        for (int i = 0; i < 8; i++)
        {
            async::receive(h2, "1", 1);
            async::receive(h2, "1\n", 2);
            async::receive(h2, "\n2\n3\n4\n5\n6\n{\na\n", 15);
            async::receive(h2, "b\nc\nd\n}\n89\n", 11);
        }
        });

    t1.join();
    t2.join();

    async::disconnect(h);
    async::disconnect(h2);

    return 0;
}
