//
// Created by sanhuazhang on 2018/06/05
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

#pragma once

#include <WCDB/Backup.hpp>
#include <WCDB/FactoryRelated.hpp>

namespace WCDB {

namespace Repair {

class FactoryBackup final : public FactoryRelated, public ErrorProne, public BackupDelegateHolder {
public:
    using FactoryRelated::FactoryRelated;
    ~FactoryBackup() override final;
    bool work(const UnsafeStringView& database);

    void notifiyBackupBegin(StringView& materialPath);
    void notifiyBackupEnd(StringView& materialPath, Backup& backup);
};

} //namespace Repair

} //namespace WCDB