// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from cj-login.djinni

#import <Foundation/Foundation.h>
@class CJCjLogin;
@class CJRegisterUserCallback;


@interface CJCjLogin : NSObject

+ (nullable CJCjLogin *)create;

- (void)registerUser:(nonnull NSString *)userName
            password:(nonnull NSString *)password
                  cb:(nullable CJRegisterUserCallback *)cb;

@end
