# CMake generated Testfile for 
# Source directory: F:/quicNew/libevent-2.1.12-stable
# Build directory: F:/quicNew/libevent-2.1.12-stable/vs2019
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(test-changelist__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-changelist")
set_tests_properties(test-changelist__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-eof__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-eof")
set_tests_properties(test-eof__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-closed__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-closed")
set_tests_properties(test-closed__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-fdleak__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-fdleak")
set_tests_properties(test-fdleak__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-init__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-init")
set_tests_properties(test-init__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-time__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-time")
set_tests_properties(test-time__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-weof__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-weof")
set_tests_properties(test-weof__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1254;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-dumpevents__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-dumpevents" "|" "C:/Program Files/Python/Python39/python.exe" "F:/quicNew/libevent-2.1.12-stable/test/check-dumpevents.py")
set_tests_properties(test-dumpevents__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1267;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(regress__WIN32 "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/regress" "--quiet")
set_tests_properties(regress__WIN32 PROPERTIES  ENVIRONMENT "" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1289;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(regress__WIN32_debug "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/regress" "--quiet")
set_tests_properties(regress__WIN32_debug PROPERTIES  ENVIRONMENT ";EVENT_DEBUG_MODE=1" _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1295;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1320;add_backend_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-ratelim__group_lim "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-ratelim" "-g" "30000" "-n" "30" "-t" "100" "--check-grouplimit" "1000" "--check-stddev" "100")
set_tests_properties(test-ratelim__group_lim PROPERTIES  _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1331;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-ratelim__con_lim "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-ratelim" "-c" "1000" "-n" "30" "-t" "100" "--check-connlimit" "50" "--check-stddev" "50")
set_tests_properties(test-ratelim__con_lim PROPERTIES  _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1340;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-ratelim__group_con_lim "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-ratelim" "-c" "1000" "-g" "30000" "-n" "30" "-t" "100" "--check-grouplimit" "1000" "--check-connlimit" "50" "--check-stddev" "50")
set_tests_properties(test-ratelim__group_con_lim PROPERTIES  _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1349;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
add_test(test-ratelim__group_con_lim_drain "F:/quicNew/libevent-2.1.12-stable/vs2019/bin/test-ratelim" "-c" "1000" "-g" "35000" "-n" "30" "-t" "100" "-G" "500" "--check-grouplimit" "1000" "--check-connlimit" "50" "--check-stddev" "50")
set_tests_properties(test-ratelim__group_con_lim_drain PROPERTIES  _BACKTRACE_TRIPLES "F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;1360;add_test;F:/quicNew/libevent-2.1.12-stable/CMakeLists.txt;0;")
