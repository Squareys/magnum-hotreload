/*
    Copyright © 2018 Jonathan Hale <squareys@googlemail.com>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "AbstractModule.h"

#include <Corrade/Utility/Debug.h>
#include <Corrade/Utility/Directory.h>
#include <Corrade/Containers/Array.h>

using namespace Corrade;
using namespace Corrade::Utility;

void onFileChanged(const std::string& filename);

#ifdef CORRADE_TARGET_WINDOWS
#include <windows.h>
#include <Winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

void watchDirectory(const std::string& directory) {
    HANDLE directoryHandle;

    directoryHandle = CreateFile(directory.c_str(),
            GENERIC_READ|FILE_LIST_DIRECTORY,
            FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
            nullptr);

    if(directoryHandle == INVALID_HANDLE_VALUE) {
        Error{} << "Cannot open folder.";
        return;
    }

    OVERLAPPED pollingOverlap;
    pollingOverlap.OffsetHigh = 0;
    pollingOverlap.hEvent = CreateEvent(nullptr, true, false, nullptr);

    bool result = true;
    char filename[MAX_PATH]; // TODO: Use Corrade Array
    while(result) {
        char buf[2048];
        DWORD resultSize;
        result = ReadDirectoryChangesW(
                directoryHandle,
                &buf, sizeof(buf), /* Destination for change results */
                false, /* Don't monitor entire tree */
                FILE_NOTIFY_CHANGE_LAST_WRITE,
                &resultSize, /* Destination for returned size in bytes */
                &pollingOverlap, nullptr);
        /* Wait for buf to be fully filled */
        WaitForSingleObject(pollingOverlap.hEvent, INFINITE);

        int offset = 0;
        FILE_NOTIFY_INFORMATION* notifyInformation;
        do {
            notifyInformation = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
            strcpy(filename, "");

            const int filenameLength = WideCharToMultiByte(
                    CP_ACP, 0, notifyInformation->FileName, notifyInformation->FileNameLength/2,
                    filename, sizeof(filename),
                    nullptr, nullptr); // TODO: ???

            filename[filenameLength] = '\0'; // TODO: ???
            switch(notifyInformation->Action) {
                case FILE_ACTION_MODIFIED:
                    onFileChanged(filename);
                    break;
                default:
                    Error{} << "Default error.";
                    break;
            }

            offset += notifyInformation->NextEntryOffset;
        } while(notifyInformation->NextEntryOffset);
    }

    CloseHandle(directoryHandle);
}

#else
void watchDirectory(const std::string& filename) {
    Fatal{} << "Directory watching not implemented for your operating system.";
    return;
}
#endif

PluginManager::Manager<AbstractModule> manager{};
std::unique_ptr<AbstractModule> module;

void onFileChanged(const std::string& filename) {
    if(filename.size() < 4) return;

    if(filename.compare("TestModule.dll") == 0) {
        Debug() << "Hot reloading" << filename;

        /* Tell module that it is being unloaded and retrieve its state for
         * state transfer, if supported. */
        auto state{module->unload(false)};
        /* Remove references to the plugin, otherwise we cannot unload it */
        module.reset(nullptr);

        /* Unload the plugin, so that we can overwrite it with a new copy.
         * Note: You may want to keep it around if you are using any classes
         * of this plugin. Instead you can copy the plugin to a filename with
         * random suffix and leave this one loaded. */
        if(manager.unload("TestModule123") & PluginManager::LoadState::UnloadFailed) {
            Error{} << "Unload of" << filename << "failed.";
            return;
        }

        /* Overwrite temporary copy with new copy, TODO: Maybe only necessary on windows. */
        Directory::write("TestModule123.dll", Directory::read("TestModule.dll"));

        /* Load the new copy */
        if(!(manager.load("TestModule123") & PluginManager::LoadState::Loaded)) {
            Error{} << "Load of" << filename << "failed.";
            return;
        }

        module = manager.instance("TestModule123");
        /* Tell the new instance it has been loaded and pass the state from the
         * old plugin, which may be used or may not. */
        module->load(std::move(state));
    }
}

int main(int argc, char** argv) {
    /* Copy DLL so that the original can be overwritten with never version */
    Directory::write("TestModule123.dll", Directory::read("TestModule.dll"));

    /* Set the plugin manager to load plugins from same directory as this app */
    manager.setPluginDirectory(Directory::path(Directory::executableLocation()));

    /* First load of the copied plugin */
    if(!(manager.load("TestModule123") & PluginManager::LoadState::Loaded)) {
        Error{} << "TestModule can not be loaded.";
        return 2;
    }

    module = manager.instance("TestModule123");
    module->load();

    /* Watch the directory for changes to files and hotreload on them */
    watchDirectory(".");

    module->unload();
    return 0;
}
