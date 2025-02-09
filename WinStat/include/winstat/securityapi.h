#pragma once
#include "winstat/common.h"

// Queries the system's Secure Boot state
int WINSTAT_API querySecureBootState(bool& state);