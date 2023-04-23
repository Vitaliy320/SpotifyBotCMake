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

#boost_link: https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(Boost_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost_1_82_0/include/")
set(ASIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/boost_1_82_0/include/")
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost COMPONENTS system REQUIRED)

#curl_link: https://curl.se/download/curl-8.0.1.tar.gz
#set(CURL_LIBRARY "${CMAKE_SOURCE_DIR}/dependencies/curl-8.0.1/lib/")
#set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/dependencies/curl-8.0.1/include/")
find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

include_directories(
	${OPENSSL_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
	"${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/include"
	"${CMAKE_SOURCE_DIR}/dependencies/boost_1_82_0/include/"
	"${CMAKE_SOURCE_DIR}/dependencies/boost_1_82_0/include/boost"
	"${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-linux/include/"
	"${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-linux/include/crow/"
	"${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-linux/include/"
	"${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-linux/include/json/"
)
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

file(GLOB_RECURSE tgbotHeaders "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/include/tgbot/*.h")
file(GLOB_RECURSE tgbotNetHeaders "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/include/tgbot/net/*.h")
file(GLOB_RECURSE tgbotToolsHeaders "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/include/tgbot/tools/*.h")
file(GLOB_RECURSE tgbotTypesHeaders "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/include/tgbot/types/*.h")

file(GLOB_RECURSE tgbotSrc "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/src/*.cpp")
file(GLOB_RECURSE tgbotNetSrc "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/src/net/*.cpp")
file(GLOB_RECURSE tgbotToolsSrc "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/src/tools/*.cpp")
file(GLOB_RECURSE tgbotTypesSrc "${CMAKE_SOURCE_DIR}/dependencies/tgbot-cpp/src/types/*.cpp")

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

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(JSONCPP jsoncpp)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE) # these are to use the prefered -pthread flag
set(THREADS_PREFER_PTHREAD_FLAG TRUE) # over the -lpthread option
find_package(Threads REQUIRED)

#get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
#include_directories(${JSON_INC_PATH})


#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

add_executable(SpotifyBotExecutable SpotifyBot/main.cpp SpotifyBot/server.cpp SpotifyBot/jsonHandler.cpp SpotifyBot/SpotifyAPIHandler.cpp SpotifyBot/config.cpp)

target_include_directories(
	SpotifyBotExecutable 
	PRIVATE 
	${includeDir} 
	${headerFiles} 
	#${jsoncppIncludeDir}
)

target_link_libraries(
	SpotifyBotExecutable 
	tgbot 
	${headerFiles} 
	${sourceFiles} 
	"${CMAKE_SOURCE_DIR}/dependencies/vcpkg/installed/x64-linux/lib/libjsoncpp.a"
	Threads::Threads 
	#${JSONCPP_LIBRARIES} 
	${CMAKE_THREAD_LIBS_INIT} 
	${OPENSSL_LIBRARIES} 
	${Boost_LIBRARIES}
	#Curl::Curl
	#Crow::Crow
	${CURL_LIBRARIES} 
	#${jsoncpp_LIBRARIES}
)