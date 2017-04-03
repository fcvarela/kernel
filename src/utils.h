#ifndef __UTILS_H__
#define __UTILS_H__

#define ASSERT_EFI_STATUS(x, n) {if(EFI_ERROR((x))) { Print(n": %r\n", x); return x; }}

#endif
