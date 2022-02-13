#include <smolrtsp/io_vec.h>

#include <greatest.h>

TEST io_vec_len(void) {
    struct iovec bufs[] = {
        {.iov_base = NULL, .iov_len = 5},
        {.iov_base = NULL, .iov_len = 1},
        {.iov_base = NULL, .iov_len = 0},
    };

    ASSERT_EQ(
        SmolRTSP_IoVecSlice_len((SmolRTSP_IoVecSlice)Slice99_typed_from_array(bufs)), 5 + 1 + 0);

    PASS();
}

SUITE(io_vec) {
    RUN_TEST(io_vec_len);
}
