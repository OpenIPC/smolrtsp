#include <smolrtsp/io_vec.h>

#include <greatest.h>

TEST io_vec_len(void) {
    struct iovec bufs[] = {
        {.iov_base = NULL, .iov_len = 5},
        {.iov_base = NULL, .iov_len = 1},
        {.iov_base = NULL, .iov_len = 0},
    };

    ASSERT_EQ(
        SmolRTSP_IoVecSlice_len(
            (SmolRTSP_IoVecSlice)Slice99_typed_from_array(bufs)),
        5 + 1 + 0);

    PASS();
}

TEST slice_to_iovec(void) {
    uint8_t data[] = {1, 2, 3};
    const struct iovec bufs =
        smolrtsp_slice_to_iovec((U8Slice99)Slice99_typed_from_array(data));

    ASSERT_EQ(bufs.iov_base, data);
    ASSERT_EQ(bufs.iov_len, sizeof data);

    PASS();
}

SUITE(io_vec) {
    RUN_TEST(io_vec_len);
    RUN_TEST(slice_to_iovec);
}
