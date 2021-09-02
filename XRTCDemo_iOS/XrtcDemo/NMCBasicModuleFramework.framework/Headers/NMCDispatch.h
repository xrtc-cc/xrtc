//
//  NMCDispatch.h
//  NMCBasicModule
//
//  Created by taojinliang on 2018/10/17.
//  Copyright © 2018年 taojinliang. All rights reserved.
//

#import <Foundation/Foundation.h>

//声明
#define NMC_INLINE static inline  //内联

#define NMCWeakify(weak, strong)  __weak __typeof(strong) weak = strong;//弱引用

#define NMCStrongify(strong, weak)  __strong __typeof(weak) strong = weak;//强引用


//线程
#ifdef __cplusplus
extern "C" {
#endif
    
#pragma mark - IO 队列
    
    void nmc_io_async(dispatch_block_t block);
    
    void nmc_io_sync_safe(dispatch_block_t block);
    
    
    
#pragma mark - 主线程
    void nmc_main_sync_safe(dispatch_block_t block);
    
    void nmc_main_async_safe(dispatch_block_t block);
    
    
#ifdef __cplusplus
}
#endif
