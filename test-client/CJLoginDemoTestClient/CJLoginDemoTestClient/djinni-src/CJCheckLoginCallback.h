// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import <Foundation/Foundation.h>


@protocol CJCheckLoginCallback

- (void)complete:(int32_t)errcode
          errmsg:(nonnull NSString *)errmsg
      sessionKey:(nonnull NSString *)sessionKey;

@end
