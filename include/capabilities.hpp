#pragma once
namespace nx {
struct CapabilityReport { bool sockets=false; bool nifm=false; unsigned long long totalMemory=0; unsigned long long freeMemory=0; };
CapabilityReport probeCapabilities();
}
