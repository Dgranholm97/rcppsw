################################################################################
# External Projects                                                            #
################################################################################
ExternalProject_Add(
project_rcsw
  SOURCE_DIR "$ENV{rcsw}"
  BINARY_DIR "$ENV{rcsw}/build"
  STEP_TARGETS build
  EXCLUDE_FROM_ALL TRUE
)
ExternalProject_Get_Property(project_rcsw binary_dir)
ExternalProject_Get_Property(project_rcsw source_dir)
add_library(rcsw STATIC IMPORTED)
set_property(TARGET rcsw PROPERTY IMPORTED_LOCATION ${binary_dir}/lib/librcsw.a)
message("${rcsw_LIBS}")
set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost 1.58.0 COMPONENTS system filesystem thread)

################################################################################
# Includes                                                                     #
################################################################################
include_directories(${source_dir}/include)

################################################################################
# Submodules                                                                   #
################################################################################
get_filename_component(target ${CMAKE_CURRENT_LIST_DIR} NAME)

list(APPEND ${target}_SUBDIRS src/common)
list(APPEND ${target}_SUBDIRS src/multithread)
list(APPEND ${target}_SUBDIRS src/utils)
list(APPEND ${target}_SUBDIRS src/patterns/state_machine)
list(APPEND ${target}_SUBDIRS src/control)
if (WITH_MPI)
  list(APPEND ${target}_SUBDIRS src/multiprocess)
endif()

################################################################################
# Libraries                                                                    #
################################################################################
add_library(${target}
  $<TARGET_OBJECTS:common>
  $<TARGET_OBJECTS:multithread>
  $<TARGET_OBJECTS:utils>
  $<TARGET_OBJECTS:state_machine>
  $<TARGET_OBJECTS:control>)

foreach(d ${${target}_SUBDIRS})
  add_subdirectory(${d})
endforeach()

target_link_libraries(${target}
  rcsw
  ${Boost_LIBRARIES}
  )
