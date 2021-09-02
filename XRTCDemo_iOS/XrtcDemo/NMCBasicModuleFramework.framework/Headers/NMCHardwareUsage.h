//
//  NMCHardwareUsage.h
//  NMC_TCP
//
//  Created by Sampson on 2019/3/22.
//  Copyright © 2019 Netease. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef struct nmcMemUsageInfo {
    double used; // in MBs
    double total; // in MBs
} nmcMemUsageInfo;

#ifdef __cplusplus
extern "C" {
#endif
    
    float nmc_cpu_usage(void);
    float nmc_device_cpu_usage(void);
    int nmc_mem_usage(nmcMemUsageInfo *retInfo);
    
#ifdef __cplusplus
}
#endif
