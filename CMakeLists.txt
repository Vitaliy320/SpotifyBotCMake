cmake_minimum_required(VERSION 3.10.2)
project(SpotifyBotCMake CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

file(GLOB_RECURSE headerFiles "/${CMAKE_PROJECT_DIR}SpotifyBot/*.h")
file(GLOB_RECURSE sourceFiles "/${CMAKE_PROJECT_DIR}SpotifyBot/*.cpp")

set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/dependencies/boost_1_81_0")
set(Boost_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost_1_81_0")

find_package(Boost)

set(BUILD_TESTING OFF)
set(BUILD_TESTING_STATIC OFF)
set(BUILD_TESTING_SHARED OFF)

file (GLOB_RECURSE sources CONFIGURE_DEPENDS "SpotifyBot/*.cpp")
file (GLOB_RECURSE headers CONFIGURE_DEPENDS "SpotifyBot/*.h")

set (includeDir "SpotifyBot")

foreach (_headerFile ${headers})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND includeDir ${_dir})
endforeach()
list (REMOVE_DUPLICATES includeDir)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(jsoncppIncludeDir "${CMAKE_SOURCE_DIR}/dependencies/jsoncpp/include")
include_directories(${jsoncppIncludeDir})

set(jsoncppSources "${CMAKE_SOURCE_DIR}/dependencies/jsoncpp/src")
set(jsoncppSourcesTestRunner "${CMAKE_SOURCE_DIR}/dependencies/jsoncpp/jsontestrunner")
set(jsoncppSourcesLibJson "${CMAKE_SOURCE_DIR}/dependencies/jsoncpp/lib_json")
set(jsoncppSourcesTestLibJson "${CMAKE_SOURCE_DIR}/dependencies/jsoncpp/test_lib_json")

include_directories(${jsoncppSources} ${jsoncppSourcesTestRunner} ${jsoncppSourcesLibJson} ${jsoncppSourcesTestLibJson})

file(GLOB jsoncppFiles
    "${jsoncppSources}/lib_json/*.cpp"
)

set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/lib/libcurl.lib")
set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include")
find_package( CURL REQUIRED )

file(GLOB_RECURSE curlHeaders "${CMAKE_SOURCE_DIR}/dependencies/curl/include/curl/*.h")
file(GLOB_RECURSE curlsrcHeaders "${CMAKE_SOURCE_DIR}/dependencies/curl/src/*")

include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

set(VCPKG_INSTALLATION_DIR "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows")


#set(TGBOT_LIBRARY "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/lib/TgBot.lib")

file(GLOB_RECURSE headerFiles "/${CMAKE_SOURCE_DIR}SpotifyBot/*.h")
file(GLOB_RECURSE sourceFiles "/${CMAKE_SOURCE_DIR}SpotifyBot/*.cpp")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE) # these are to use the prefered -pthread flag
set(THREADS_PREFER_PTHREAD_FLAG TRUE) # over the -lpthread option
find_package(Threads REQUIRED)


#set(tgbotHeaders "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include/tgbot")
#set(tgbotNetHeaders "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include/tgbot/net")
#set(tgbotToolsHeaders "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include/tgbot/tools")
#set(tgbotTypesHeaders "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include/tgbot/types")

#file(GLOB_RECURSE tgbotLib "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/lib/TgBot.lib")
#add_library(TgBot SHARED IMPORTED)
#set_target_properties(TgBot PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/bin/TgBot.dll")

include_directories("${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include" "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/include/crow" )

add_executable(SpotifyBotExecutable ${jsoncppFiles} SpotifyBot/main.cpp SpotifyBot/server.cpp SpotifyBot/jsonHandler.cpp SpotifyBot/SpotifyAPIHandler.cpp SpotifyBot/config.cpp)

target_include_directories(
    SpotifyBotExecutable PUBLIC 
    "dependencies/vcpkg/installed/x64-windows/include"
    "${CMAKE_PROJECT_DIR}/dependencies/curl/include"
    ${includeDir} 
    ${headerFiles} 
    ${jsoncppIncludeDir}
    #${tgbotHeaders}
    #${tgbotNetHeaders}
    #${tgbotToolsHeaders}
    #${tgbotTypesHeaders}
)

target_link_libraries(SpotifyBotExecutable 
    #${TGBOT_LIBRARY}
    ${headerFiles}
    ${sourceFiles} 
    Threads::Threads
    ${CMAKE_THREAD_LIBS_INIT}
    ${OPENSSL_LIBRARIES}
    "${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-windows/lib/TgBot.lib"
    #${TgBot}
    CURL::libcurl
)
