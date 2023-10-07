#include <chrono>
#include <cstring>  // std::memcpy
#include <thread>   // std::this_thread::sleep_for

#include <gtest/gtest.h>

/*

int clock_gettime(clockid_t clockid, struct timespec *tp);
int clock_settime(clockid_t clockid, const struct timespec *tp);

Link with -lrt (only for glibc versions before 2.17).

       CLOCK_REALTIME
              A  settable  system-wide  clock  that measures real (i.e., wall-clock) time.  Setting this clock requires
  appropriate privileges.  This clock is affected by discontinuous jumps in the system time (e.g., if the system
  administrator manually changes the clock), and by the incremental adjustments performed by adjtime(3) and NTP.

       CLOCK_MONOTONIC
              A nonsettable system-wide clock that represents monotonic time since—as described by POSIX—"some
  unspecified point in the past".  On Linux, that point corresponds  to  the  number  of seconds that the system has
  been running since it was booted.

*/

void ros_steadytime(uint32_t& sec, uint32_t& nsec) {
#ifndef WIN32
    timespec start;
#if defined(__APPLE__)
    // On macOS use clock_get_time.
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    start.tv_sec = mts.tv_sec;
    start.tv_nsec = mts.tv_nsec;
#else   // defined(__APPLE__)
    // Otherwise use clock_gettime.
    clock_gettime(CLOCK_MONOTONIC, &start);
#endif  // defined(__APPLE__)
    sec = start.tv_sec;
    nsec = start.tv_nsec;
#else
    static LARGE_INTEGER cpu_frequency, performance_count;
    // These should not ever fail since XP is already end of life:
    // From https://msdn.microsoft.com/en-us/library/windows/desktop/ms644905(v=vs.85).aspx and
    //      https://msdn.microsoft.com/en-us/library/windows/desktop/ms644904(v=vs.85).aspx:
    // "On systems that run Windows XP or later, the function will always succeed and will
    //  thus never return zero."
    QueryPerformanceFrequency(&cpu_frequency);
    if (cpu_frequency.QuadPart == 0) {
        throw NoHighPerformanceTimersException();
    }
    QueryPerformanceCounter(&performance_count);
    double steady_time = performance_count.QuadPart / (double)cpu_frequency.QuadPart;
    int64_t steady_sec = floor(steady_time);
    int64_t steady_nsec = boost::math::round((steady_time - steady_sec) * 1e9);

    // Throws an exception if we go out of 32-bit range
    normalizeSecNSecUnsigned(steady_sec, steady_nsec);

    sec = steady_sec;
    nsec = steady_nsec;
#endif
}

class SteadyTime {
  public:
    uint32_t sec, nsec;

    SteadyTime() : sec(0), nsec(0) {}

    double toSec() const { return (double)sec + 1e-9 * (double)nsec; };

    uint64_t toNSec() const { return (uint64_t)sec * 1000000000ull + (uint64_t)nsec; }

    static SteadyTime now() {
        SteadyTime t;
        ros_steadytime(t.sec, t.nsec);
        return t;
    }
};

TEST(SteadyTime, now) {
    SteadyTime t0 = SteadyTime::now();
    std::cout << t0.sec << ", " << t0.nsec << std::endl;

    double t0_sec = t0.toSec();

    std::this_thread::sleep_for(std::chrono::seconds(1));
    SteadyTime t1 = SteadyTime::now();
    EXPECT_EQ(1 + t0.sec, t1.sec);
    EXPECT_NEAR(t0_sec + 1, t1.toSec(), 1e-2);
}

/*
NAME
   memcpy - copy memory area

SYNOPSIS
   #include <string.h>

   void *memcpy(void *dest, const void *src, size_t n);

DESCRIPTION
   The  memcpy()  function  copies n bytes from memory area src to memory area dest.  The memory areas must not
   overlap.  Use memmove(3) if the memory areas do overlap.
*/
TEST(SteadyTime, memcpy) {
    constexpr int FRAME_ID_MAX_SIZE = 24;

    SteadyTime t = SteadyTime::now();

    std::string buf;
    buf.resize(32);
    EXPECT_EQ(32, buf.size());

    auto time_ns = t.toNSec();
    std::stringstream ss;
    ss << time_ns;
    std::string time_ns_str = ss.str();
    std::cout << time_ns_str << std::endl;
    EXPECT_EQ(15, time_ns_str.size());  // 纳秒的单位是15位

    std::memcpy(&buf[FRAME_ID_MAX_SIZE], &time_ns, 8);
    EXPECT_EQ(32, buf.size());

    {
        uint64_t recovery_time_ns = *reinterpret_cast<const uint64_t*>(&buf[FRAME_ID_MAX_SIZE]);
        std::cout << recovery_time_ns << std::endl;
        EXPECT_EQ(time_ns, recovery_time_ns);
    }

    {
        std::string copy_buf = buf;
        EXPECT_EQ(32, copy_buf.size());
        copy_buf[0] = 'x';
        uint64_t recovery_time_ns = *reinterpret_cast<const uint64_t*>(&copy_buf[FRAME_ID_MAX_SIZE]);
        EXPECT_EQ(time_ns, recovery_time_ns);
    }
    // 这是因为 uint64_t 就是 8 bytes, memcpy 就把这个数值复制到数组中了。
    // memcpy copy 不是字符a, b, c或者数字1,2,3, 而是内存地址里的内容。 所以这种方式是正确的。
    // 原本 15 位的数字 以 bytes的方式放到 string 中，则只需要 8 个 bytes即可。
    // 即 15个(数字, byte)变成了 8个（字符, byte） 空间一下子节省了一倍。
}