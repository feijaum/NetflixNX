#include "capabilities.hpp"
#include <switch.h>
namespace nx {
CapabilityReport probeCapabilities(){
 CapabilityReport r{}; r.sockets=true;
 Result rc=nifmInitialize(NifmServiceType_User); r.nifm=R_SUCCEEDED(rc); if(r.nifm)nifmExit();
 u64 total=0,used=0; svcGetInfo(&total,InfoType_TotalMemorySize,INVALID_HANDLE,0); svcGetInfo(&used,InfoType_UsedMemorySize,INVALID_HANDLE,0);
 r.totalMemory=total;r.freeMemory=total>used?total-used:0;return r;
}
}
