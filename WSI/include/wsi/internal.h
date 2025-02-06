#pragma once
#include "wsi/common.h"

int isRunningAsAdmin(bool& has_elevated_privileges);
bool isSystemUEFI();