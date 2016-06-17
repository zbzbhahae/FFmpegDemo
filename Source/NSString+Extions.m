//
//  NSString+Extions.m
//  SJLiveVideo
//
//  Created by yuanmaochao on 16/6/16.
//  Copyright © 2016年 king. All rights reserved.
//

#import "NSString+Extions.h"

@implementation NSString (Extions)

+(NSString *)bundlePath:(NSString *)fileName {
    return [[[NSBundle mainBundle] bundlePath] stringByAppendingPathComponent:fileName];
}

+(NSString *)documentsPath:(NSString *)fileName {
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    return [documentsDirectory stringByAppendingPathComponent:fileName];
}
@end
