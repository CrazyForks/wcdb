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

#ifndef _WCDB_MASTERCRAWLER_HPP
#define _WCDB_MASTERCRAWLER_HPP

#include <WCDB/Crawlable.hpp>

namespace WCDB {

namespace Repair {

class Master;
class MasterCrawler;

class MasterCrawlerDelegate {
public:
    virtual ~MasterCrawlerDelegate();

protected:
    friend class MasterCrawler;

    virtual void onMasterPageCrawled(const Page &page);
    virtual void onMasterCellCrawled(const Cell &cell, const Master &master) = 0;
    virtual void onMasterCrawlerError() = 0;
};

class MasterCrawler final : public Crawlable {
#pragma mark - Initialize
public:
    MasterCrawler(Pager &pager);

#pragma mark - Master
public:
    bool work(MasterCrawlerDelegate *delegate);

protected:
    MasterCrawlerDelegate *m_delegate;

#pragma mark - Crawlable
protected:
    void onCellCrawled(const Cell &cell) override final;
    bool willCrawlPage(const Page &page, int height) override final;
    void onCrawlerError() override final;
};

} //namespace Repair

} //namespace WCDB

#endif /* _WCDB_MASTERCRAWLER_HPP */