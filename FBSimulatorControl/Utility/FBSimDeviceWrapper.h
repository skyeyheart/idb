/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <Foundation/Foundation.h>

@class FBProcessInfo;
@class FBProcessQuery;
@class FBSimulator;
@class FBSimulatorControlConfiguration;
@class SimDevice;

/**
 Mirrors the Method Signatures in SimDevice. Augmenting with:
 - More informative return values.
 - Implementations that are more resiliant to failure in CoreSimulator.
 */
@interface FBSimDeviceWrapper : NSObject

/**
 Creates a SimDevice Wrapper.

 @param simulator the Simulator to wrap
 @param configuration the Simulator Control Configuration.
 @param processQuery the Process Query to obtain process information.
 @return a new SimDevice wrapper.
 */
+ (instancetype)withSimulator:(FBSimulator *)simulator configuration:(FBSimulatorControlConfiguration *)configuration processQuery:(FBProcessQuery *)processQuery;

/**
 'Shutting Down' a Simulator can be a little hairier than just calling '-[SimDevice shutdownWithError:]'.
 This method of shutting down takes into account a variety of error states and attempts to recover from them.

 Note that 'Shutting Down' a Simulator is different to 'terminating' or 'killing':
 - Killing a Simulator will kill the Simulator.app process.
 - Killing the Simulator.app process will soon-after get the SimDevice into a 'Shutdown' state in CoreSimulator.
 - This will take a number of seconds and represents an inconsistent state for the Simulator.
 - Calling Shutdown on a Simulator without terminating the Simulator.app process first will result in a 'Zombie' Simulator.
 - A 'Zombie' Simulator.app is a Simulator that isn't backed by a running SimDevice in CoreSimulator.

 Therefore this method should be called if:
 - A Simulator has no corresponding 'Simulator.app'. This is the case if `-[SimDevice bootWithOptions:error]` has been called directly.
 - After Simulator's corresponding 'Simulator.app' has been killed.

 @param error a descriptive error for any error that occurred.
 @return YES if successful, NO otherwise.
 */
- (BOOL)shutdownWithError:(NSError **)error;

/**
 Boots an Application on the Simulator.
 Will time out with an error if CoreSimulator gets stuck in a semaphore and timeout resiliance is enabled.

 @param appID the Application ID to use.
 @param options the Options to use in the launch.
 @param error an error out for any error that occured.
 @return the Process Identifier of the launched process, -1 otherwise.
 */
- (FBProcessInfo *)launchApplicationWithID:(NSString *)appID options:(NSDictionary *)options error:(NSError **)error;

/**
 Installs an Application on the Simulator.
 Will time out with an error if CoreSimulator gets stuck in a semaphore and timeout resiliance is enabled.

 @param appURL the Application URL to use.
 @param options the Options to use in the launch.
 @param error an error out for any error that occured.
 @return YES if the Application was installed successfully, NO otherwise.
 */
- (BOOL)installApplication:(NSURL *)appURL withOptions:(NSDictionary *)options error:(NSError **)error;

/**
 Spawns a binary on the Simulator.
 Will time out with an error if CoreSimulator gets stuck in a semaphore and timeout resiliance is enabled.

 @param launchPath the path to the binary.
 @param options the Options to use in the launch.
 @param terminationHandler ?????
 @param error an error out for any error that occured.
 @return the Process Identifier of the launched process, -1 otherwise.
 */
- (FBProcessInfo *)spawnWithPath:(NSString *)launchPath options:(NSDictionary *)options terminationHandler:(id)terminationHandler error:(NSError **)error;

/**
 Adds a Video to the Camera Roll.
 Will polyfill to the 'Camera App Upload' hack.

 @param paths an Array of paths of videos to upload.
 @return YES if the upload was successful, NO otherwise.
 */
- (BOOL)addVideos:(NSArray *)paths error:(NSError **)error;

@end
