# Copyright (C) 2006-2018 Istituto Italiano di Tecnologia (IIT)
# All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

if(CREATE_YARPROBOTINTERFACE)

  get_property(YARP_OS_INCLUDE_DIRS TARGET YARP_OS PROPERTY INCLUDE_DIRS)
  get_property(YARP_dev_INCLUDE_DIRS TARGET YARP_dev PROPERTY INCLUDE_DIRS)


  include_directories(${YARP_OS_INCLUDE_DIRS}
                      ${YARP_dev_INCLUDE_DIRS}
                      ${TinyXML_INCLUDE_DIRS})

  set(yarprobotinterface_SRCS
      main.cpp
      Action.cpp
      CalibratorThread.cpp
      Device.cpp
      Module.cpp
      Param.cpp
      Robot.cpp
      Types.cpp
      XMLReader.cpp)

  set(yarprobotinterface_HDRS
      Action.h
      CalibratorThread.h
      Device.h
      Module.h
      Param.h
      Robot.h
      Types.h
      XMLReader.h)

  set(yarprobotinterface_IDLS
      yarprobotinterface.thrift)


  include(YarpIDL)
  yarp_idl_to_dir(${yarprobotinterface_IDLS}
                  "${CMAKE_CURRENT_SOURCE_DIR}/idl_generated_code"
                  yarprobotinterface_IDLS_SRCS
                  yarprobotinterface_IDLS_HDRS
                  yarprobotinterface_IDLS_INCLUDE_DIRS)
  include_directories("${yarprobotinterface_IDLS_INCLUDE_DIRS}")
  
  yarp_prepare_plugin(yarprobotinterface
                      CATEGORY rfmodule
                      TYPE RobotInterface::Module
                      INCLUDE Module.h
                      EXTRA_CONFIG CODE=""
                      DEPENDS "CREATE_RFMODULE_LIBRARY_MODULES")
  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  yarp_add_plugin(robotinterface     ${yarprobotinterface_SRCS}
                                     ${yarprobotinterface_HDRS}
                                     ${yarprobotinterface_IDLS_SRCS}
                                     ${yarprobotinterface_IDLS_HDRS})
  
  target_link_libraries(robotinterface
                        YARP::YARP_OS
                        YARP::YARP_dev
                        YARP::YARP_init
                        ${TinyXML_LIBRARIES})

  yarp_install(TARGETS robotinterface
               EXPORT YARP
               COMPONENT runtime
               LIBRARY DESTINATION ${YARP_DYNAMIC_PLUGINS_INSTALL_DIR}
               ARCHIVE DESTINATION ${YARP_STATIC_PLUGINS_INSTALL_DIR})
  yarp_install(FILES yarprobotinterface.ini
               COMPONENT runtime
               DESTINATION ${YARP_PLUGIN_MANIFESTS_INSTALL_DIR})
  set_property(TARGET robotinterface PROPERTY FOLDER "Plugins/RFModules")

  add_executable(yarprobotinterface ${yarprobotinterface_SRCS}
                                    ${yarprobotinterface_HDRS}
                                    ${yarprobotinterface_IDLS_SRCS}
                                    ${yarprobotinterface_IDLS_HDRS})

  target_link_libraries(yarprobotinterface
                        YARP::YARP_OS
                        YARP::YARP_dev
                        YARP::YARP_init
                        ${TinyXML_LIBRARIES})

  install(TARGETS yarprobotinterface
          COMPONENT utilities
          DESTINATION ${CMAKE_INSTALL_BINDIR})

  set_property(TARGET yarprobotinterface PROPERTY FOLDER "Command Line Tools")
endif()
