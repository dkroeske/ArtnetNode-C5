# origin-pack: STMicroelectronics::lan8742_part_drivers@2.0.2
# file-format: 1.0.0
project(STMicroelectronics_lan8742_part_drivers_2_0_2)
cmake_minimum_required(VERSION 3.30)
add_library(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE)

# List of all CMSIS properties that influence conditions for this package


# Device specific defined symbols








# Enable all components in this package
if(CMSIS_ENTIRE_STMicroelectronics_lan8742_part_drivers_2_0_2)
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 Core#Cversion:1.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 IO#Cvariant:MDIO#Cversion:1.0.0")
  list(APPEND CMSIS_COMPONENTS_LIST "Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 IO#Cvariant:Custom#Cversion:1.0.0")
endif()

# All conditions used by this package

# condition: STMicroelectronics.lan8742_part_drivers.2.0.2:LAN8742 CFG Condition
# description: STMicroelectronics configuration for LAN8742 part driver
set(STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_CFG_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_CFG_Condition enabled")


# condition: STMicroelectronics.lan8742_part_drivers.2.0.2:LAN8742 Core Condition
# description: STMicroelectronics LAN8742 Core Driver
set(STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Core_Condition "$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Board Part#.*Cgroup:LAN8742#.*Csub:LAN8742 IO(#.*|$)>,>>")
message(DEBUG "CMSIS condition STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Core_Condition enabled")


# condition: STMicroelectronics.lan8742_part_drivers.2.0.2:LAN8742 Custom IO Condition
# description: 
set(STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition "1")
message(DEBUG "CMSIS condition STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition enabled")


# condition: STMicroelectronics.lan8742_part_drivers.2.0.2:LAN8742 IO MDIO Condition
# description: STMicroelectronics LAN8742 IO MDIO Driver
set(STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_IO_MDIO_Condition "$<AND:$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:ETH(#.*|$)>,>>,$<NOT:$<STREQUAL:$<FILTER:${CMSIS_COMPONENTS_LIST},INCLUDE,.*Cclass:Device#.*Cgroup:STM32 HAL#.*Csub:EXTI(#.*|$)>,>>>")
message(DEBUG "CMSIS condition STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_IO_MDIO_Condition enabled")

# Files and components in this package
if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 Core#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Core driver for LAN8742 Ethernet PHY
  message(DEBUG "Using component Board_Part_LAN8742_LAN8742_Core_1_0_0")
  target_compile_definitions(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Core_Condition}:-DCMSIS_USE_Board_Part_LAN8742_LAN8742_Core_1_0_0=1>")
  target_include_directories(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/.>")
  target_sources(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Core_Condition}:${CMAKE_CURRENT_LIST_DIR}/lan8742.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 IO#Cvariant:MDIO#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # IO for LAN8742 ETH PHY
  message(DEBUG "Using component Board_Part_LAN8742_LAN8742_IO_MDIO_1_0_0")
  target_compile_definitions(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_IO_MDIO_Condition}:-DCMSIS_USE_Board_Part_LAN8742_LAN8742_IO_MDIO_1_0_0=1>")
  target_include_directories(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_IO_MDIO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/eth>")
  target_sources(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_IO_MDIO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/eth/lan8742_io.c>")
endif()

if("Cvendor:STMicroelectronics#Cclass:Board Part#Cgroup:LAN8742#Csub:LAN8742 IO#Cvariant:Custom#Cversion:1.0.0" IN_LIST CMSIS_COMPONENTS_LIST)  # Custom I/O layer for LAN8742 part
  message(DEBUG "Using component Board_Part_LAN8742_LAN8742_IO_Custom_1_0_0")
  target_compile_definitions(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition}:-DCMSIS_USE_Board_Part_LAN8742_LAN8742_IO_Custom_1_0_0=1>")
  target_include_directories(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom>")
  if(EXISTS ${CMSIS_RTE_FOLDER}/Board Part/lan8742_io.c)
    target_sources(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition}:${CMSIS_RTE_FOLDER}/Board Part/lan8742_io.c>")
  else()
    target_sources(STMicroelectronics_lan8742_part_drivers_2_0_2 INTERFACE "$<${STMicroelectronics.lan8742_part_drivers.2.0.2_LAN8742_Custom_IO_Condition}:${CMAKE_CURRENT_LIST_DIR}/interfaces/custom/lan8742_io.c>")
  endif()
endif()

