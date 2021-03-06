//
//  EEPluginDictionary.cpp
//  roll-eat
//
//  Created by Zinge on 5/18/16.
//
//

#import "ee/core/internal/EEDictionaryUtils.h"

#import "ee/core/internal/EEJsonUtils.h"

@implementation EEDictionaryUtils

+ (NSString* _Nonnull)emptyResult {
    return [EEJsonUtils convertDictionaryToString:@{}];
}
+ (NSDictionary* _Nonnull)createDictWithError:(NSError* _Nullable)error {
    NSMutableDictionary* dict = [NSMutableDictionary dictionary];
    [self setError:error forDict:dict];
    return dict;
}

+ (void)setError:(NSError* _Nullable)error
         forDict:(NSMutableDictionary* _Nonnull)dict {
    if (error == nil) {
        [dict setObject:@([error code]) forKey:@"errorCode"];

        NSString* description = [error localizedDescription];
        if (description != nil) {
            [dict setObject:description forKey:@"errorDescription"];
        }
    }
}

+ (void)setResult:(id _Nullable)result
          forDict:(NSMutableDictionary* _Nonnull)dict {
    if (result != nil) {
        [dict setObject:(id _Nonnull)result forKey:@"result"];
    }
}
@end
