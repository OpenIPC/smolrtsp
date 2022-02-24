#include <smolrtsp/writer.h>

#include <greatest.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <slice99.h>

TEST fd_writer(void) {
    int fds[2];
    const bool socketpair_ok = socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0;
    ASSERT(socketpair_ok);

    SmolRTSP_Writer w = smolrtsp_fd_writer(&fds[0]);

    char buffer[32] = {0};

    {
        ssize_t ret = VCALL(w, write, CharSlice99_from_str("abc"));
        ASSERT_EQ(3, ret);
        read(fds[1], buffer, ret);
        ASSERT_STRN_EQ("abc", buffer, ret);
    }

    {
        ssize_t ret = VCALL(w, writef, "%d abc %s", 123, "!@#");
        ASSERT_EQ(11, ret);
        read(fds[1], buffer, ret);
        ASSERT_STRN_EQ("123 abc !@#", buffer, ret);
    }

    close(fds[0]);
    close(fds[1]);

    PASS();
}

TEST file_writer(void) {
    FILE *tmp = tmpfile();
    assert(tmp);

    SmolRTSP_Writer w = smolrtsp_file_writer(tmp);

    char buffer[32] = {0};

    {
        ssize_t ret = VCALL(w, write, CharSlice99_from_str("abc"));
        ASSERT_EQ(3, ret);
        fseek(tmp, 0, SEEK_SET);
        fread(buffer, sizeof(char), ret, tmp);
        ASSERT_STRN_EQ("abc", buffer, ret);
    }

    {
        ssize_t ret = VCALL(w, writef, "%d abc %s", 123, "!@#");
        ASSERT_EQ(11, ret);
        fseek(tmp, 0, SEEK_SET);
        fread(buffer, sizeof(char), ret, tmp);
        ASSERT_STRN_EQ("abc123 abc !@#", buffer, ret);
    }

    fclose(tmp);

    PASS();
}

TEST string_writer(void) {
    char buffer[32] = {0};

    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);

    {
        ssize_t ret = VCALL(w, write, CharSlice99_from_str("abc"));
        ASSERT_EQ(3, ret);
        ASSERT_STRN_EQ("abc", buffer, ret);
    }

    {
        ssize_t ret = VCALL(w, writef, "%d abc %s", 123, "!@#");
        ASSERT_EQ(11, ret);
        ASSERT_STR_EQ("abc123 abc !@#", buffer);
    }

    PASS();
}

TEST write_slices(void) {
    char buffer[128] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);

    const CharSlice99 slices[] = {
        CharSlice99_from_str("abc"),
        CharSlice99_from_str("~"),
        CharSlice99_from_str("&* 123"),
    };

    const ssize_t bytes_written =
        smolrtsp_write_slices(w, SLICE99_ARRAY_LEN(slices), slices);
    ASSERT_EQ(10, bytes_written);
    ASSERT_STR_EQ("abc~&* 123", buffer);

    PASS();
}

TEST write_slices_macro(void) {
    char buffer[128] = {0};
    SmolRTSP_Writer w = smolrtsp_string_writer(buffer);

    const ssize_t bytes_written = SMOLRTSP_WRITE_SLICES(
        w, {
               CharSlice99_from_str("abc"),
               CharSlice99_from_str("~"),
               CharSlice99_from_str("&* 123"),
           });
    ASSERT_EQ(10, bytes_written);
    ASSERT_STR_EQ("abc~&* 123", buffer);

    PASS();
}

SUITE(writer) {
    RUN_TEST(fd_writer);
    RUN_TEST(file_writer);
    RUN_TEST(string_writer);

    RUN_TEST(write_slices);
    RUN_TEST(write_slices_macro);
}
