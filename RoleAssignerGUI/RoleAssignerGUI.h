#pragma once

#include "resource.h"
#include <commdlg.h>	// For OPENFILENAME and GetSaveFileName
#include <shlwapi.h>    // For PathRemoveFileSpecW
#pragma comment(lib, "shlwapi.lib")  // Link with Shlwapi.lib
#pragma comment(lib, "comdlg32.lib")
#include <fstream>      // For file input operations
#include <string>       // For std::string manipulation