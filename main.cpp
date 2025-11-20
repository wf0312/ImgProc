#include "types.hpp"
#include <cstddef>
#include <cvt_color.hpp>

#include "test/test.hpp"

using namespace zycs;

int main(int argc, char* argv[])
{
    int test_type = std::stoi(argv[1]);
    argc -= 2;
    argv += 2;
    if (0 == test_type) {
        return cvtTest(argc, argv);
    }
    if (1 == test_type) {
        return resizeTest(argc, argv);
    }

    return 0;
}
