cmake_minimum_required(VERSION 3.10.2)
project(SpotifyBot CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(Boost_USE_MULTITHREADED ON)

set(BUILD_TESTING OFF)
set(BUILD_TESTING_STATIC OFF)
set(BUILD_TESTING_SHARED OFF)
#add_subdirectory("SpotifyBot/dependencies/vcpkg/installed/x64-linux/share/vcpkg-cmake")

file (GLOB_RECURSE sources CONFIGURE_DEPENDS "SpotifyBot/*.cpp")
file (GLOB_RECURSE headers CONFIGURE_DEPENDS "SpotifyBot/*.h")

set (includeDir "SpotifyBot")
set (jsoncppIncludeDir "SpotifyBot/dependencies/vcpkg/installed/x64-linux/include")

foreach (_headerFile ${headers})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND includeDir ${_dir})
endforeach()
list (REMOVE_DUPLICATES includeDir)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

include_directories(${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR}) 
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

file(GLOB_RECURSE tgbotHeaders "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/include/tgbot/*.h")
file(GLOB_RECURSE tgbotNetHeaders "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/include/tgbot/net/*.h")
file(GLOB_RECURSE tgbotToolsHeaders "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/include/tgbot/tools/*.h")
file(GLOB_RECURSE tgbotTypesHeaders "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/include/tgbot/types/*.h")

file(GLOB_RECURSE tgbotSrc "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/src/*.cpp")
file(GLOB_RECURSE tgbotNetSrc "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/src/net/*.cpp")
file(GLOB_RECURSE tgbotToolsSrc "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/src/tools/*.cpp")
file(GLOB_RECURSE tgbotTypesSrc "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/tgbot-cpp/src/types/*.cpp")

add_library(
	tgbot
	SHARED
	${tgbotHeaders}
	${tgbotNetHeaders}
	${tgbotToolsHeaders}
	${tgbotTypesHeaders}
	${tgbotSrc}
	${tgbotNetSrc}
	${tgbotToolsSrc}
	${tgbotTypesSrc}
)

file(GLOB_RECURSE jsoncppLib "${CMAKE_PROJECT_DIR}SpotifyBot/dependencies/vcpkg/installed/x64-linux/lib/libjsoncpp.a")

file(GLOB_RECURSE headerFiles "/${CMAKE_PROJECT_DIR}SpotifyBot/*.h")
file(GLOB_RECURSE sourceFiles "/${CMAKE_PROJECT_DIR}SpotifyBot/*.cpp")

find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE) # these are to use the prefered -pthread flag
set(THREADS_PREFER_PTHREAD_FLAG TRUE) # over the -lpthread option
find_package(Threads REQUIRED)

add_executable(SpotifyBotExecutable SpotifyBot/main.cpp SpotifyBot/server.cpp SpotifyBot/jsonHandler.cpp SpotifyBot/SpotifyAPIHandler.cpp SpotifyBot/config.cpp)

target_include_directories(
	SpotifyBotExecutable 
	PRIVATE 
	${includeDir} 
	${headerFiles} 
	${jsoncppIncludeDir}
)

target_link_libraries(
	SpotifyBotExecutable 
	tgbot 
	${headerFiles} 
	${sourceFiles} 
	${jsoncppLib} 
	Threads::Threads 
	${JSONCPP_LIBRARIES} 
	${CMAKE_THREAD_LIBS_INIT} 
	${OPENSSL_LIBRARIES} 
	${Boost_LIBRARIES} 
	${CURL_LIBRARIES} 
	${jsoncpp_LIBRARIES}
)