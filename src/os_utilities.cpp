#include <os_utilities.h>

#include <Windows.h>

void manta::createFolder(const char *folder) {
	if (CreateDirectory(folder, NULL) ||
		GetLastError() == ERROR_ALREADY_EXISTS) {
		// Folder was created successfully
	}
	else {
		// Failed to create folder
	}
}
