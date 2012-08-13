#ifndef FRIENDSMASHER_FILE
#define FRIENDSMASHER_FILE

#include <aio.h>
#include <stdio.h>
#include "type_defs.h"
#include "File_iPhoneCore.h"

namespace FriendSmasher
{
	namespace System
	{
            typedef u32 FileHandle;

            class File
            {
            public:

                    static File* Instance();
                    virtual ~File();

                    bool Initialise();
                    void Update();
                    bool Uninitialise();
                    
                    virtual const char* GetName() { return "File"; }

                    u32 GetFileSizeBytes(const char* filename, bool bCompressed=false);
                    u32 GetFileSizeBytes(FILE* fptr, bool bCompressed=false);
                    u32 GetFileSizeBytes(FileHandle handle);

                    // Synchronous file handling
                    unsigned char* LoadFile(const char* filename);

            private:
                
                    File();
                    static File* ms_Instance;
                
                    FILE* OpenFile(char* filename);
                
            };
	}
}

#endif // FRIENDSMASHER_FILE