//
//  __EEJsonUtils.hpp
//  roll-eat
//
//  Created by Zinge on 5/16/16.
//
//

#import <Foundation/Foundation.h>

@interface EEJsonUtils : NSObject;

/// Converts @c NSDictionary to JSON formatted @c NSString.
+ (NSString*) convertDictionaryToString:(NSDictionary*) dict;

/// Converts JSON formatted @c NSString to @c NSDictionary.
+ (NSMutableDictionary*) convertStringToDictionary:(NSString*) str;

@end