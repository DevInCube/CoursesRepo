#include <windows.h>
#include <Winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include "thread.h"
#include "list.h"
#include "fswatcher.h"

#define MAX_DIRS 25
#define MAX_FILES 255
#define MAX_BUFFER 4096

WINBASEAPI BOOL WINAPI
ReadDirectoryChangesW(
    HANDLE hDirectory,
    LPVOID lpBuffer,
    DWORD nBufferLength,
    BOOL bWatchSubtree,
    DWORD dwNotifyFilter,
    LPDWORD lpBytesReturned,
    LPOVERLAPPED lpOverlapped,
    LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

// all purpose structure to contain directory information and provide
// the input buffer that is filled with file change data

typedef struct _DIRECTORY_INFO {
    HANDLE hDir;
    TCHAR lpszDirName[MAX_PATH];
    CHAR lpBuffer[MAX_BUFFER];
    DWORD dwBufLength;
    OVERLAPPED Overlapped;
} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO DirInfo[MAX_DIRS];   // Buffer for all of the directories
TCHAR FileList[MAX_FILES*MAX_PATH]; // Buffer for all of the files
DWORD numDirs;

//Method to start watching a directory. Call it on a separate thread so it wont block the main thread.

void WatchDirectory(LPCWSTR path) {
    char buf[2048];
    DWORD nRet;
    BOOL result = TRUE;
    char filename[MAX_PATH];
    DirInfo[0].hDir = CreateFile (
        (LPCSTR) path,
        GENERIC_READ | FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
        NULL
    );

    if(DirInfo[0].hDir == INVALID_HANDLE_VALUE)
    {
       return; //cannot open folder
    }

    lstrcpy(DirInfo[0].lpszDirName, (LPCSTR) path);
    OVERLAPPED PollingOverlap;

    FILE_NOTIFY_INFORMATION * pNotify;
    int offset;
    PollingOverlap.OffsetHigh = 0;
    PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    while(result)
    {
       result = ReadDirectoryChangesW(
                    DirInfo[0].hDir,    // handle to the directory to be watched
                    &buf,   // pointer to the buffer to receive the read results
                    sizeof(buf),    // length of lpBuffer
                    TRUE,   // flag for monitoring directory or directory tree
                    FILE_NOTIFY_CHANGE_FILE_NAME |
                    FILE_NOTIFY_CHANGE_DIR_NAME |
                    FILE_NOTIFY_CHANGE_SIZE,
                    // FILE_NOTIFY_CHANGE_LAST_WRITE |
                    // FILE_NOTIFY_CHANGE_LAST_ACCESS |
                    // FILE_NOTIFY_CHANGE_CREATION,
                    &nRet,  // number of bytes returned
                    &PollingOverlap,    // pointer to structure needed for overlapped I/O
                    NULL
                );

       WaitForSingleObject(PollingOverlap.hEvent, INFINITE);
       offset = 0;
       do
       {
           pNotify = (FILE_NOTIFY_INFORMATION *)((char *)buf + offset);
           strcpy((char *) filename, "");
           (void)WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength / 2, filename, sizeof(filename), NULL, NULL);
           filename[pNotify->FileNameLength / 2] = 0;
           switch(pNotify->Action)
           {
               case FILE_ACTION_ADDED:
                   printf("\nThe file is added to the directory: [%s] \n", filename);
                   break;
               case FILE_ACTION_REMOVED:
                   printf("\nThe file is removed from the directory: [%s] \n", filename);
                   break;
               case FILE_ACTION_MODIFIED:
                   printf("\nThe file is modified. This can be a change in the time stamp or attributes: [%s]\n", filename);
                   break;
               case FILE_ACTION_RENAMED_OLD_NAME:
                   printf("\nThe file was renamed and this is the old name: [%s]\n", filename);
                   break;
               case FILE_ACTION_RENAMED_NEW_NAME:
                   printf("\nThe file was renamed and this is the new name: [%s]\n", filename);
                   break;
               default:
                   printf("\nDefault error.\n");
                   break;
            }

           offset += pNotify->NextEntryOffset;

        } while(pNotify->NextEntryOffset); //(offset != 0);
      }

    CloseHandle( DirInfo[0].hDir );
}

int fileExists(TCHAR * file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file, &FindFileData) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found)
   {
       //FindClose(&handle); this will crash
       FindClose(handle);
   }
   return found;
}
