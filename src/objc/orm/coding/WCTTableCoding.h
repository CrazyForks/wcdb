//
// Created by sanhuazhang on 2019/05/02
//

/*
 * Tencent is pleased to support the open source community by making
 * WCDB available.
 *
 * Copyright (C) 2017 THL A29 Limited, a Tencent company.
 * All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License"); you may not use
 * this file except in compliance with the License. You may obtain a copy of
 * the License at
 *
 *       https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef WCDB_SWIFT

#import "WCTBridgeProperty.h"

#elif __cplusplus

#import "WCTBridgeProperty+CPP.h"
#import "WCTCommon.h"
#import "WCTProperty.h"

#endif

NS_ASSUME_NONNULL_BEGIN

@protocol WCTTableCoding <NSObject>

#ifdef WCDB_SWIFT

@required
+ (WCTBridgeProperties *)allBridgeProperties NS_SWIFT_NAME(allProperties());

#elif __cplusplus

@required
+ (const WCTBinding &)objectRelationalMapping;
+ (const WCTProperties &)allProperties;
@optional
+ (void)additionalObjectRelationalMapping:(WCTBinding &)binding;

#endif

@optional
@property (nonatomic, assign) long long lastInsertedRowID;
@property (nonatomic, assign) BOOL isAutoIncrement;

@end

NS_ASSUME_NONNULL_END
