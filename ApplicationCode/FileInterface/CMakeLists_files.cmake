
# Use this workaround until we're on 2.8.3 on all platforms and can use CMAKE_CURRENT_LIST_DIR directly 
if (${CMAKE_VERSION} VERSION_GREATER "2.8.2")
    set(CEE_CURRENT_LIST_DIR  ${CMAKE_CURRENT_LIST_DIR}/)
endif()


list(APPEND CODE_HEADER_FILES
${CEE_CURRENT_LIST_DIR}RifEclipseInputFileTools.h
${CEE_CURRENT_LIST_DIR}RifEclipseOutputFileTools.h
${CEE_CURRENT_LIST_DIR}RifEclipseRestartDataAccess.h
${CEE_CURRENT_LIST_DIR}RifEclipseRestartFilesetAccess.h
${CEE_CURRENT_LIST_DIR}RifEclipseUnifiedRestartFileAccess.h
${CEE_CURRENT_LIST_DIR}RifReaderEclipseInput.h
${CEE_CURRENT_LIST_DIR}RifReaderEclipseOutput.h
${CEE_CURRENT_LIST_DIR}RifReaderInterface.h
${CEE_CURRENT_LIST_DIR}RifReaderMockModel.h
${CEE_CURRENT_LIST_DIR}RifReaderStatisticalCalculation.h
)

list(APPEND CODE_SOURCE_FILES
${CEE_CURRENT_LIST_DIR}RifEclipseInputFileTools.cpp
${CEE_CURRENT_LIST_DIR}RifEclipseOutputFileTools.cpp
${CEE_CURRENT_LIST_DIR}RifEclipseRestartDataAccess.cpp
${CEE_CURRENT_LIST_DIR}RifEclipseRestartFilesetAccess.cpp
${CEE_CURRENT_LIST_DIR}RifEclipseUnifiedRestartFileAccess.cpp
${CEE_CURRENT_LIST_DIR}RifReaderEclipseInput.cpp
${CEE_CURRENT_LIST_DIR}RifReaderEclipseOutput.cpp
${CEE_CURRENT_LIST_DIR}RifReaderMockModel.cpp
${CEE_CURRENT_LIST_DIR}RifReaderStatisticalCalculation.cpp
)

