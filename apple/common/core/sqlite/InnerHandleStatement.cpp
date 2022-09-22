//
// Created by sanhuazhang on 2019/05/19
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

#include <WCDB/AbstractHandle.hpp>
#include <WCDB/Assertion.hpp>
#include <WCDB/InnerHandleStatement.hpp>
#include <WCDB/SQLite.h>
#include <WCDB/WINQ.h>

namespace WCDB {

InnerHandleStatement::InnerHandleStatement(InnerHandleStatement &&other)
: HandleRelated(other.getHandle())
, m_stmt(other.m_stmt)
, m_done(other.m_done)
, m_newTable(other.m_newTable)
, m_modifiedTable(other.m_modifiedTable)
{
    other.m_done = false;
    other.m_stmt = nullptr;
}

InnerHandleStatement::InnerHandleStatement(AbstractHandle *handle)
: HandleRelated(handle), m_stmt(nullptr), m_done(false)
{
}

InnerHandleStatement::~InnerHandleStatement()
{
    finalize();
}

bool InnerHandleStatement::prepare(const Statement &statement)
{
    if (getHandle()->needMonitorTable()) {
        analysisStatement(statement);
    }
    return prepare(statement.getDescription());
}

void InnerHandleStatement::analysisStatement(const Statement &statement)
{
    m_modifiedTable.clear();
    m_newTable.clear();
    switch (statement.getType()) {
    case Syntax::Identifier::Type::InsertSTMT: {
        const Syntax::InsertSTMT &insertSTMT
        = static_cast<const Syntax::InsertSTMT &>(statement.syntax());
        m_modifiedTable = insertSTMT.table;
    } break;
    case Syntax::Identifier::Type::UpdateSTMT: {
        const Syntax::UpdateSTMT &updateSTMT
        = static_cast<const Syntax::UpdateSTMT &>(statement.syntax());
        m_modifiedTable = updateSTMT.table.table;
    } break;
    case Syntax::Identifier::Type::DeleteSTMT: {
        const Syntax::DeleteSTMT &deleteSTMT
        = static_cast<const Syntax::DeleteSTMT &>(statement.syntax());
        m_modifiedTable = deleteSTMT.table.table;
    } break;
    case Syntax::Identifier::Type::AlterTableSTMT: {
        const Syntax::AlterTableSTMT &alterSTMT
        = static_cast<const Syntax::AlterTableSTMT &>(statement.syntax());
        if (alterSTMT.switcher == Syntax::AlterTableSTMT::Switch::RenameTable) {
            m_modifiedTable = alterSTMT.newTable;
        }
    } break;
    case Syntax::Identifier::Type::CreateTableSTMT: {
        const Syntax::CreateTableSTMT &createSTMT
        = static_cast<const Syntax::CreateTableSTMT &>(statement.syntax());
        m_newTable = createSTMT.table;
    } break;
    case Syntax::Identifier::Type::CreateVirtualTableSTMT: {
        const Syntax::CreateVirtualTableSTMT &createSTMT
        = static_cast<const Syntax::CreateVirtualTableSTMT &>(statement.syntax());
        m_newTable = createSTMT.table;
    } break;
    default:
        break;
    }
}

bool InnerHandleStatement::prepare(const UnsafeStringView &sql)
{
    WCTRemedialAssert(!isPrepared(), "Last statement is not finalized.", finalize(););
    WCTAssert(sql.length() > 0);

    bool result = APIExit(
    sqlite3_prepare_v2(getRawHandle(), sql.data(), -1, &m_stmt, nullptr), sql);
    m_done = false;
    if (!result) {
        m_stmt = nullptr;
    }
    return result;
}

void InnerHandleStatement::reset()
{
    WCTAssert(isPrepared());
    APIExit(sqlite3_reset(m_stmt));
}

bool InnerHandleStatement::done()
{
    return m_done;
}

bool InnerHandleStatement::step()
{
    WCTAssert(isPrepared());

    int rc = sqlite3_step(m_stmt);
    m_done = rc == SQLITE_DONE;

    if (done() && getHandle()->needMonitorTable()
        && (!m_newTable.empty() || !m_modifiedTable.empty())) {
        getHandle()->postTableNotification(m_newTable, m_modifiedTable);
    }

    const char *sql = nullptr;
    if (isPrepared()) {
        // There will be privacy issues if use sqlite3_expanded_sql
        sql = sqlite3_sql(m_stmt);
    }
    return APIExit(rc, sql);
}

void InnerHandleStatement::finalize()
{
    if (m_stmt != nullptr) {
        // no need to call APIExit since it returns old code only.
        sqlite3_finalize(m_stmt);
        m_stmt = nullptr;
    }
}

int InnerHandleStatement::getNumberOfColumns()
{
    WCTAssert(isPrepared());
    return sqlite3_column_count(m_stmt);
}

const UnsafeStringView InnerHandleStatement::getOriginColumnName(int index)
{
    WCTAssert(isPrepared());
    return sqlite3_column_origin_name(m_stmt, index);
}

const UnsafeStringView InnerHandleStatement::getColumnName(int index)
{
    WCTAssert(isPrepared());
    return sqlite3_column_name(m_stmt, index);
}

const UnsafeStringView InnerHandleStatement::getColumnTableName(int index)
{
    WCTAssert(isPrepared());
    return sqlite3_column_table_name(m_stmt, index);
}

ColumnType InnerHandleStatement::getType(int index)
{
    WCTAssert(isPrepared());
    switch (sqlite3_column_type(m_stmt, index)) {
    case SQLITE_INTEGER:
        return ColumnType::Integer;
    case SQLITE_FLOAT:
        return ColumnType::Float;
    case SQLITE_BLOB:
        return ColumnType::BLOB;
    case SQLITE_TEXT:
        return ColumnType::Text;
    default:
        return ColumnType::Null;
    }
}

void InnerHandleStatement::bindInteger(const Integer &value, int index)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    bool succeed = APIExit(sqlite3_bind_int64(m_stmt, index, value));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

void InnerHandleStatement::bindDouble(const Float &value, int index)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    bool succeed = APIExit(sqlite3_bind_double(m_stmt, index, value));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

void InnerHandleStatement::bindText(const Text &value, int index)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    // use SQLITE_STATIC if auto_commit?
    bool succeed = APIExit(sqlite3_bind_text(
    m_stmt, index, value.data(), (int) value.length(), SQLITE_TRANSIENT));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

void InnerHandleStatement::bindBLOB(const BLOB &value, int index)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    // TODO: use SQLITE_STATIC to get better performance?
    bool succeed = APIExit(sqlite3_bind_blob(
    m_stmt, index, value.buffer(), (int) value.size(), SQLITE_TRANSIENT));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

void InnerHandleStatement::bindNull(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    bool succeed = APIExit(sqlite3_bind_null(m_stmt, index));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

void InnerHandleStatement::bindPointer(void *ptr,
                                       int index,
                                       const Text &type,
                                       void (*destructor)(void *))
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    bool succeed
    = APIExit(sqlite3_bind_pointer(m_stmt, index, ptr, type.data(), destructor));
    WCTAssert(succeed);
    WCDB_UNUSED(succeed);
}

int InnerHandleStatement::bindParameterIndex(const Text &parameterName)
{
    WCTAssert(isPrepared());
    WCTAssert(!isBusy());
    int index = sqlite3_bind_parameter_index(m_stmt, parameterName.data());
    return index;
}

InnerHandleStatement::Integer InnerHandleStatement::getInteger(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(isBusy());
    return static_cast<Integer>(sqlite3_column_int64(m_stmt, index));
}

InnerHandleStatement::Float InnerHandleStatement::getDouble(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(isBusy());
    return static_cast<Float>(sqlite3_column_double(m_stmt, index));
}

InnerHandleStatement::Text InnerHandleStatement::getText(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(isBusy());
    return UnsafeStringView(
    reinterpret_cast<const char *>(sqlite3_column_text(m_stmt, index)),
    sqlite3_column_bytes(m_stmt, index));
}

const InnerHandleStatement::BLOB InnerHandleStatement::getBLOB(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(isBusy());
    return BLOB::immutable(
    reinterpret_cast<const unsigned char *>(sqlite3_column_blob(m_stmt, index)),
    sqlite3_column_bytes(m_stmt, index));
}

void InnerHandleStatement::bindValue(const Value &value, int index)
{
    switch (value.getType()) {
    case ColumnType::Null:
        return bindNull(index);
    case ColumnType::Integer:
        return bindInteger(value, index);
    case ColumnType::Float:
        return bindDouble(value, index);
    case ColumnType::Text:
        return bindText(value, index);
    case ColumnType::BLOB:
        return bindBLOB(value, index);
    }
}

void InnerHandleStatement::bindRow(const OneRowValue &row)
{
    for (int i = 1; i <= row.size(); i++) {
        bindValue(row[i - 1], i);
    }
}

Value InnerHandleStatement::getValue(int index)
{
    switch (getType(index)) {
    case ColumnType::Null:
        return Value(nullptr);
    case ColumnType::Integer:
        return Value(getInteger(index));
    case ColumnType::Float:
        return Value(getDouble(index));
    case ColumnType::Text:
        return Value(getText(index));
    case ColumnType::BLOB:
        return Value(getBLOB(index));
    }
}

OptionalOneColumn InnerHandleStatement::getOneColumn(int index)
{
    OneColumnValue result;
    bool succeed = false;
    while ((succeed = step()) && !done()) {
        result.push_back(getValue(index));
    }
    return succeed ? result : OptionalOneColumn();
}

OneRowValue InnerHandleStatement::getOneRow()
{
    OneRowValue result;
    int count = getNumberOfColumns();
    for (int i = 0; i < count; i++) {
        result.push_back(getValue(i));
    }
    return result;
}

OptionalMultiRows InnerHandleStatement::getAllRows()
{
    MultiRowsValue result;
    bool succeed = false;
    while ((succeed = step()) && !done()) {
        result.push_back(getOneRow());
    }
    return succeed ? result : OptionalMultiRows();
}

signed long long InnerHandleStatement::getColumnSize(int index)
{
    WCTAssert(isPrepared());
    WCTAssert(isBusy());
    return sqlite3_column_bytes(m_stmt, index);
}

bool InnerHandleStatement::isBusy()
{
    WCTAssert(isPrepared());
    return sqlite3_stmt_busy(m_stmt) != 0;
}

bool InnerHandleStatement::isReadOnly()
{
    WCTAssert(isPrepared());
    return sqlite3_stmt_readonly(m_stmt) != 0;
}

bool InnerHandleStatement::isPrepared()
{
    return m_stmt != nullptr;
}

} //namespace WCDB