# Functional Requirements - ksf_FA_Wallet

## Document Information
| Field | Value |
|-------|-------|
| **Module** | ksf_FA_Wallet |
| **Version** | 1.0.0 |
| **Author** | KSFII Development Team |
| **Date** | 2026-05-12 |

---

## 1. Introduction

### 1.1 Purpose

This document specifies the detailed functional requirements for the ksf_FA_Wallet module.

### 1.2 Scope

The functional requirements cover:
- Wallet creation and management
- Credit/debit operations
- Transaction recording
- Cashback program
- Admin UI functionality

---

## 2. Functional Requirements

### 2.1 Wallet Operations

#### FR-WAL-001: Create Wallet
**Priority:** High  
**Description:** System shall create wallet account for users/customers.

| Field | Type | Required | Validation |
|-------|------|----------|------------|
| user_id | INT | Yes | Must exist in users table |
| initial_balance | DECIMAL | No | Default 0, >= 0 |

**Business Rules:**
- One wallet per user
- Initial balance can be set for initial funding
- If wallet exists, return existing

**Acceptance Criteria:**
- Wallet created with balance = 0 (or initial)
- Unique per user
- is_locked = false initially

---

#### FR-WAL-002: Get Wallet Balance
**Priority:** High  
**Description:** System shall return current wallet balance for user.

**Input:** user_id (INT)

**Output:** 
- balance (DECIMAL)
- is_locked (BOOLEAN)
- last_transaction (DATETIME)

**Acceptance Criteria:**
- Returns current balance
- Returns lock status
- Returns null if no wallet

---

#### FR-WAL-003: Credit Wallet
**Priority:** High  
**Description:** System shall add funds to wallet balance.

| Field | Type | Required | Validation |
|-------|------|----------|------------|
| user_id | INT | Yes | Must have wallet |
| amount | DECIMAL | Yes | > 0 |
| details | TEXT | No | Transaction description |
| reference_type | VARCHAR | No | 'manual', 'refund', etc. |
| reference_id | INT | No | Related record ID |

**Business Rules:**
- Amount must be > 0
- Wallet must not be locked
- Record transaction with type='credit'

**Acceptance Criteria:**
- Balance increased by amount
- Transaction recorded
- balance_after calculated correctly

---

#### FR-WAL-004: Debit Wallet
**Priority:** High  
**Description:** System shall remove funds from wallet balance.

| Field | Type | Required | Validation |
|-------|------|----------|------------|
| user_id | INT | Yes | Must have wallet |
| amount | DECIMAL | Yes | > 0, <= balance |
| details | TEXT | No | Transaction description |
| reference_type | VARCHAR | No | 'payment', 'withdrawal', etc. |

**Business Rules:**
- Amount must be > 0
- Amount must be <= current balance
- Wallet must not be locked
- Record transaction with type='debit'

**Acceptance Criteria:**
- Balance decreased by amount
- Transaction recorded
- Cannot go negative

---

#### FR-WAL-005: Lock Wallet
**Priority:** Medium  
**Description:** System shall temporarily disable wallet.

**Input:** user_id (INT)

**Business Rules:**
- Set is_locked = true
- Block all credit/debit operations while locked

**Acceptance Criteria:**
- is_locked = true
- Operations blocked with error

---

#### FR-WAL-006: Unlock Wallet
**Priority:** Medium  
**Description:** System shall re-enable locked wallet.

**Input:** user_id (INT)

**Business Rules:**
- Set is_locked = false
- Restore normal operations

**Acceptance Criteria:**
- is_locked = false
- Operations allowed again

---

#### FR-WAL-007: List All Wallets
**Priority:** Medium  
**Description:** System shall list all wallets with filtering.

**Input Parameters:**
| Filter | Type | Description |
|--------|------|-------------|
| is_locked | BOOLEAN | Filter by lock status |
| min_balance | DECIMAL | Filter by minimum balance |

**Output:** Array of wallet objects with user info

**Acceptance Criteria:**
- All wallets returned if no filter
- Includes user_name, customer_name
- Pagination supported

---

### 2.2 Transaction Management

#### FR-TXN-001: Record Transaction
**Priority:** High  
**Description:** System shall log all wallet balance changes.

| Field | Type | Required | Validation |
|-------|------|----------|------------|
| user_id | INT | Yes | Valid user |
| type | ENUM | Yes | credit, debit, cashback, transfer_in, transfer_out |
| amount | DECIMAL | Yes | > 0 |
| balance_after | DECIMAL | Yes | Post-transaction balance |
| reference_type | VARCHAR | No | Source of transaction |
| reference_id | INT | No | Related record |
| details | TEXT | No | Description |
| created_by | INT | No | User performing action |

**Auto-calculated:**
- balance_after = current_balance + amount (credit) or - amount (debit)

**Acceptance Criteria:**
- Transaction record created
- All fields stored correctly
- Audit trail complete

---

#### FR-TXN-002: Get Transaction History
**Priority:** High  
**Description:** System shall retrieve transaction history for wallet.

**Input:** user_id (INT)

**Output:** Array of transaction objects ordered by date DESC

**Filters Available:**
- type (credit/debit/cashback)
- date_from, date_to
- reference_type

**Acceptance Criteria:**
- All transactions returned
- Ordered by date (newest first)
- Pagination for large histories

---

#### FR-TXN-003: Search Transactions
**Priority:** Medium  
**Description:** System shall allow searching transactions.

**Input:**
| Parameter | Type |
|-----------|------|
| user_id | INT |
| type | ENUM |
| date_from | DATETIME |
| date_to | DATETIME |
| reference_type | VARCHAR |
| reference_id | INT |

**Output:** Array of matching transactions

**Acceptance Criteria:**
- All filters work
- Combined filters supported

---

#### FR-TXN-004: Void Transaction
**Priority:** Low  
**Description:** System shall allow voiding transactions (soft delete).

**Input:** transaction_id (INT)

**Business Rules:**
- Set deleted = 1
- Do not reverse balance (create reversal transaction)
- Retain original record

**Acceptance Criteria:**
- Transaction marked deleted
- Original record preserved
- Reversal transaction created if needed

---

### 2.3 Payment Integration

#### FR-PAY-001: Apply Wallet to Payment
**Priority:** High  
**Description:** System shall apply wallet balance to order/invoice payment.

**Input:**
| Parameter | Type | Description |
|-----------|------|-------------|
| user_id | INT | Wallet owner |
| amount | DECIMAL | Amount to apply |
| reference_type | VARCHAR | 'order', 'invoice' |
| reference_id | INT | Related record |

**Business Rules:**
- Amount <= balance
- Wallet not locked
- Record as debit transaction

**Acceptance Criteria:**
- Balance reduced
- Transaction recorded
- Reference linked

---

#### FR-PAY-002: Process Refund to Wallet
**Priority:** High  
**Description:** System shall credit refund amount to wallet.

**Input:**
| Parameter | Type | Description |
|-----------|------|-------------|
| user_id | INT | Wallet owner |
| amount | DECIMAL | Refund amount |
| original_reference | VARCHAR | Original order/invoice |

**Business Rules:**
- Credit transaction
- reference_type = 'refund'
- Details include original reference

**Acceptance Criteria:**
- Balance increased
- Transaction recorded with refund details

---

#### FR-PAY-003: Partial Payment
**Priority:** High  
**Description:** System shall support partial payment from wallet.

**Input:**
| Parameter | Type | Description |
|-----------|------|-------------|
| user_id | INT | Wallet owner |
| total_amount | DECIMAL | Total order amount |
| wallet_amount | DECIMAL | Amount from wallet |
| remaining | DECIMAL | Amount from other sources |

**Acceptance Criteria:**
- Wallet portion debited
- Remaining amount returned for other payment methods

---

### 2.4 Cashback Program

#### FR-CB-001: Configure Cashback Rules
**Priority:** High  
**Description:** System shall allow configuration of cashback rules.

| Field | Type | Required | Validation |
|-------|------|----------|------------|
| name | VARCHAR(100) | Yes | Rule name |
| percentage | DECIMAL(5,2) | Yes | 0-100 |
| min_order_amount | DECIMAL | No | Default 0 |
| max_cashback | DECIMAL | No | Optional cap |
| validity_days | INT | No | Expiration days |

**Acceptance Criteria:**
- Rule created with all settings
- Multiple rules can exist
- is_active flag

---

#### FR-CB-002: Calculate Cashback
**Priority:** High  
**Description:** System shall calculate cashback for order.

**Input:**
| Parameter | Type | Description |
|-----------|------|-------------|
| order_amount | DECIMAL | Order total |
| customer_id | INT | Customer |

**Business Rules:**
- Find applicable rule (highest percentage for amount)
- Check min_order_amount threshold
- Apply max_cashback cap if set

**Output:**
- cashback_amount (DECIMAL)
- rule_applied (object)

**Acceptance Criteria:**
- Correct percentage applied
- Threshold checked
- Cap applied if reached

---

#### FR-CB-003: Credit Cashback
**Priority:** High  
**Description:** System shall automatically credit cashback to wallet.

**Input:**
| Parameter | Type | Description |
|-----------|------|-------------|
| user_id | INT | Wallet owner |
| amount | DECIMAL | Cashback amount |
| order_id | INT | Related order |

**Business Rules:**
- Create transaction with type='cashback'
- Set reference_type='cashback', reference_id=order_id
- Track for reporting

**Acceptance Criteria:**
- Wallet credited
- Transaction recorded as cashback
- Linked to order

---

#### FR-CB-004: Check Cashback Expiry
**Priority:** Medium  
**Description:** System shall check and expire old cashback.

**Input:** user_id (INT)

**Business Rules:**
- Find cashback transactions
- Check validity_days against created_at
- Mark expired (log for reporting)

**Acceptance Criteria:**
- Expired cashback identified
- Logged for reporting
- Wallet balance not affected

---

### 2.5 Admin UI

#### FR-UI-001: Wallet List Page
**Priority:** High  
**Description:** Admin page displaying all wallets.

**Features:**
- Table with user, balance, status
- Filter by status (active/locked)
- Filter by balance range
- Search by user name
- Actions: View, Lock/Unlock, Adjust

**Acceptance Criteria:**
- All wallets displayed
- Filters work correctly
- Actions functional

---

#### FR-UI-002: Wallet Detail Page
**Priority:** High  
**Description:** Page showing wallet details and transactions.

**Sections:**
- Wallet info (balance, status, created)
- Recent transactions
- Full transaction history
- Action buttons (adjust, lock/unlock)

**Acceptance Criteria:**
- All wallet data displayed
- Transaction history complete
- Actions available

---

#### FR-UI-003: Balance Adjustment
**Priority:** High  
**Description:** Admin page for manual balance adjustments.

**Features:**
- Select user
- Choose credit or debit
- Enter amount
- Enter reason/details
- Confirmation

**Acceptance Criteria:**
- Adjustment works
- Transaction recorded with details
- Balance updated correctly

---

#### FR-UI-004: Cashback Rules Configuration
**Priority:** Medium  
**Description:** Admin page for cashback rule management.

**Features:**
- List all rules
- Create new rule
- Edit existing rule
- Activate/deactivate rule

**Acceptance Criteria:**
- CRUD operations work
- Rules apply to new orders

---

## 3. Data Validation Rules

### 3.1 Input Validation

| Field | Rule | Error Message |
|-------|------|---------------|
| amount | > 0 | "Amount must be greater than zero" |
| amount | <= balance (debit) | "Insufficient balance" |
| user_id | Must have wallet | "Wallet not found" |
| percentage | 0-100 | "Percentage must be between 0 and 100" |

### 3.2 Business Rules

| Rule | Condition | Action |
|------|-----------|--------|
| Wallet locked | is_locked = true | Reject credit/debit |
| Negative balance | balance - amount < 0 | Reject debit |
| Duplicate wallet | Wallet exists for user | Return existing |

---

## 4. Traceability Matrix

| Requirement ID | Source | Test Cases |
|---------------|--------|------------|
| FR-WAL-001 | BR-001 | TC-WAL-001 |
| FR-WAL-002 | BR-001 | TC-WAL-002 |
| FR-WAL-003 | BR-001 | TC-WAL-003 |
| FR-WAL-004 | BR-001 | TC-WAL-004 |
| FR-TXN-001 | BR-002 | TC-TXN-001 |
| FR-PAY-001 | BR-003 | TC-PAY-001 |
| FR-CB-001 | BR-004 | TC-CB-001 |
| FR-UI-001 | BR-005 | TC-UI-001 |

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*