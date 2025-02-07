#pragma once
#include "wsi/common.h"

// Queries the system's Secure Boot state
int WSI_API querySecureBootState(bool& state);