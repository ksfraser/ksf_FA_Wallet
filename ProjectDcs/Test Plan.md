# Test Plan - ksf_FA_Wallet

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

This Test Plan defines the testing approach, scenarios, and criteria for the ksf_FA_Wallet module.

### 1.2 Scope

- Wallet creation and balance testing
- Credit/debit operation testing
- Transaction recording testing
- Cashback program testing
- Admin UI testing

### 1.3 Test Strategy

| Type | Approach |
|------|----------|
| **Unit Tests** | Test business logic and calculations |
| **Integration Tests** | Test FA database integration |
| **UI Tests** | Manual testing of admin pages |

---

## 2. Test Scenarios

### 2.1 Wallet Operations

#### TC-WAL-001: Create Wallet
**Test ID:** TC-WAL-001  
**Priority:** High  
**Requirement:** FR-WAL-001

**Test Steps:**
1. Invoke `create_wallet()` with user_id = 5
2. Verify wallet created with balance = 0
3. Verify is_locked = false

**Expected Result:** Wallet created with default values

**Pass Criteria:** Wallet exists, balance=0, not locked

---

#### TC-WAL-002: Get Balance
**Test ID:** TC-WAL-002  
**Priority:** High  
**Requirement:** FR-WAL-002

**Test Steps:**
1. Create wallet with balance = 100.00
2. Invoke `getBalance(5)`
3. Verify returns 100.00

**Expected Result:** Current balance returned

**Pass Criteria:** Balance matches stored value

---

#### TC-WAL-003: Credit Wallet
**Test ID:** TC-WAL-003  
**Priority:** High  
**Requirement:** FR-WAL-003

**Test Steps:**
1. Create wallet with balance = 50.00
2. Invoke `credit(5, 100.00, 'Top-up')`
3. Verify balance = 150.00
4. Verify transaction recorded

**Expected Result:** Balance increased, transaction created

**Pass Criteria:** Balance = 150, transaction exists

---

#### TC-WAL-004: Debit Wallet
**Test ID:** TC-WAL-004  
**Priority:** High  
**Requirement:** FR-WAL-004

**Test Steps:**
1. Create wallet with balance = 200.00
2. Invoke `debit(5, 75.00, 'Payment')`
3. Verify balance = 125.00
4. Verify transaction recorded

**Expected Result:** Balance decreased, transaction created

**Pass Criteria:** Balance = 125, transaction exists

---

#### TC-WAL-005: Debit Insufficient Balance
**Test ID:** TC-WAL-005  
**Priority:** High  
**Requirement:** FR-WAL-004

**Test Steps:**
1. Create wallet with balance = 50.00
2. Attempt `debit(5, 100.00, 'Payment')`
3. Verify error returned
4. Verify balance unchanged

**Expected Result:** Error: "Insufficient balance"

**Pass Criteria:** Error thrown, balance unchanged

---

#### TC-WAL-006: Lock Wallet
**Test ID:** TC-WAL-006  
**Priority:** Medium  
**Requirement:** FR-WAL-005

**Test Steps:**
1. Create wallet
2. Invoke `lockWallet(5)`
3. Verify is_locked = true
4. Attempt credit - verify blocked

**Expected Result:** Locked, operations blocked

**Pass Criteria:** Lock status true, operations rejected

---

#### TC-WAL-007: Unlock Wallet
**Test ID:** TC-WAL-007  
**Priority:** Medium  
**Requirement:** FR-WAL-006

**Test Steps:**
1. Create locked wallet
2. Invoke `unlockWallet(5)`
3. Verify is_locked = false
4. Verify operations allowed

**Expected Result:** Unlocked, operations allowed

**Pass Criteria:** Lock status false, operations work

---

#### TC-WAL-008: Credit Locked Wallet
**Test ID:** TC-WAL-008  
**Priority:** High  
**Requirement:** FR-WAL-003

**Test Steps:**
1. Create wallet and lock it
2. Attempt `credit(5, 50.00)`
3. Verify error returned
4. Verify balance unchanged

**Expected Result:** Error: "Wallet is locked"

**Pass Criteria:** Error thrown, balance unchanged

---

### 2.2 Transaction Management

#### TC-TXN-001: Record Transaction
**Test ID:** TC-TXN-001  
**Priority:** High  
**Requirement:** FR-TXN-001

**Test Steps:**
1. Create wallet with balance = 100.00
2. Invoke `credit(5, 50.00, 'Manual credit')`
3. Query fa_wallet_transactions for user_id = 5
4. Verify transaction exists with:
   - type = 'credit'
   - amount = 50.00
   - balance_after = 150.00

**Expected Result:** Transaction recorded with all fields

**Pass Criteria:** Transaction record complete

---

#### TC-TXN-002: Transaction History
**Test ID:** TC-TXN-002  
**Priority:** High  
**Requirement:** FR-TXN-002

**Test Steps:**
1. Create wallet
2. Record 10 transactions (5 credit, 5 debit)
3. Invoke `get_transactions(5)`
4. Verify 10 transactions returned
5. Verify ordered by date DESC

**Expected Result:** All transactions returned in order

**Pass Criteria:** 10 transactions, correct order

---

#### TC-TXN-003: Filter Transactions by Type
**Test ID:** TC-TXN-003  
**Priority:** Medium  
**Requirement:** FR-TXN-003

**Test Steps:**
1. Create wallet with transactions: 3 credit, 2 debit
2. Invoke `get_transactions(5, type='credit')`
3. Verify only 3 returned

**Expected Result:** Only matching type returned

**Pass Criteria:** Filter accurate, count correct

---

#### TC-TXN-004: Search by Date Range
**Test ID:** TC-TXN-004  
**Priority:** Medium  
**Requirement:** FR-TXN-003

**Test Steps:**
1. Create wallet with transactions at known dates
2. Invoke `get_transactions(5, date_from, date_to)`
3. Verify only transactions in range

**Expected Result:** Date filter works

**Pass Criteria:** Inclusive of boundaries

---

### 2.3 Payment Integration

#### TC-PAY-001: Apply Wallet to Payment
**Test ID:** TC-PAY-001  
**Priority:** High  
**Requirement:** FR-PAY-001

**Test Steps:**
1. Create wallet with balance = 200.00
2. Invoke `apply_to_payment(5, 150.00, 'order', 123)`
3. Verify balance = 50.00
4. Verify transaction reference

**Expected Result:** Payment applied, balance reduced

**Pass Criteria:** Balance = 50, reference stored

---

#### TC-PAY-002: Refund to Wallet
**Test ID:** TC-PAY-002  
**Priority:** High  
**Requirement:** FR-PAY-002

**Test Steps:**
1. Create wallet with balance = 100.00
2. Invoke `refund_to_wallet(5, 50.00, 'order', 123)`
3. Verify balance = 150.00
4. Verify transaction type = 'credit', reference = 'refund'

**Expected Result:** Refund credited to wallet

**Pass Criteria:** Balance increased, refund details

---

#### TC-PAY-003: Partial Payment
**Test ID:** TC-PAY-003  
**Priority:** High  
**Requirement:** FR-PAY-003

**Test Steps:**
1. Create wallet with balance = 75.00
2. Invoke `partial_payment(5, 200.00, 75.00)`
3. Verify wallet debited = 75.00
4. Verify remaining returned = 125.00

**Expected Result:** Partial payment processed

**Pass Criteria:** Correct amounts, remaining calculated

---

### 2.4 Cashback

#### TC-CB-001: Configure Cashback Rule
**Test ID:** TC-CB-001  
**Priority:** High  
**Requirement:** FR-CB-001

**Test Steps:**
1. Invoke `create_cashback_rule()` with:
   - name: '5% Cashback'
   - percentage: 5.00
   - min_order_amount: 50.00
2. Verify rule created

**Expected Result:** Rule created with settings

**Pass Criteria:** Rule exists with all values

---

#### TC-CB-002: Calculate Cashback
**Test ID:** TC-CB-002  
**Priority:** High  
**Requirement:** FR-CB-002

**Test Steps:**
1. Create rule: 5% cashback, min $50
2. Invoke `calculate_cashback(100.00)` (order amount)
3. Verify cashback = 5.00

**Test Steps (Threshold Not Met):**
1. Invoke `calculate_cashback(40.00)` (below min)
2. Verify cashback = 0

**Expected Result:** Correct percentage, threshold checked

**Pass Criteria:** 5% of 100 = 5, below threshold = 0

---

#### TC-CB-003: Apply Max Cashback Cap
**Test ID:** TC-CB-003  
**Priority:** Medium  
**Requirement:** FR-CB-002

**Test Steps:**
1. Create rule: 10% cashback, max $50
2. Invoke `calculate_cashback(1000.00)`
3. Verify cashback = 50.00 (capped)

**Expected Result:** Cap applied

**Pass Criteria:** 10% of 1000 = 100, capped to 50

---

#### TC-CB-004: Credit Cashback
**Test ID:** TC-CB-004  
**Priority:** High  
**Requirement:** FR-CB-003

**Test Steps:**
1. Create wallet with balance = 0
2. Create rule: 5% cashback
3. Invoke `credit_cashback(5, 5.00, 123)` (order 123)
4. Verify balance = 5.00
5. Verify transaction type = 'cashback'

**Expected Result:** Cashback credited

**Pass Criteria:** Balance = 5, type = cashback

---

### 2.5 Admin UI

#### TC-UI-001: Wallet List Display
**Test ID:** TC-UI-001  
**Priority:** High  
**Requirement:** FR-UI-001

**Test Steps:**
1. Create 10 wallets with various balances
2. Navigate to Wallet Management page
3. Verify all wallets displayed
4. Verify columns: User, Balance, Status, Actions

**Expected Result:** Full list with correct data

**Pass Criteria:** All wallets visible with correct values

---

#### TC-UI-002: Balance Adjustment
**Test ID:** TC-UI-002  
**Priority:** High  
**Requirement:** FR-UI-003

**Test Steps:**
1. Navigate to wallet detail
2. Click "Adjust Balance"
3. Select credit, enter 100.00
4. Enter reason: "Customer service adjustment"
5. Submit
6. Verify balance increased by 100.00

**Expected Result:** Balance adjusted, reason logged

**Pass Criteria:** Balance updated, transaction has reason

---

#### TC-UI-003: Transaction History Display
**Test ID:** TC-UI-003  
**Priority:** High  
**Requirement:** FR-UI-002

**Test Steps:**
1. Create wallet with 15 transactions
2. Navigate to wallet detail page
3. View Transaction History section
4. Verify transactions with all columns

**Expected Result:** All transactions displayed

**Pass Criteria:** 15 transactions with correct data

---

#### TC-UI-004: Cashback Rule CRUD
**Test ID:** TC-UI-004  
**Priority:** Medium  
**Requirement:** FR-UI-004

**Test Steps:**
1. Navigate to Cashback Rules page
2. Create new rule with all settings
3. Edit rule to change percentage
4. Deactivate rule
5. Verify all operations successful

**Expected Result:** Full CRUD functional

**Pass Criteria:** All operations successful

---

## 3. Test Data Requirements

### 3.1 Required Test Data

| Entity | Quantity | Purpose |
|--------|----------|---------|
| Users | 10 | Wallet testing |
| Wallets | 10 | Balance operations |
| Transactions | 100 | Transaction testing |
| Cashback Rules | 3 | Cashback testing |

### 3.2 Test Environment Setup

```sql
-- Insert test users
INSERT INTO users (user_id, user_name, real_name) VALUES 
(1, 'testuser1', 'Test User 1'),
(2, 'testuser2', 'Test User 2');

-- Insert test wallets
INSERT INTO fa_wallet_balances (user_id, balance) VALUES 
(1, 100.00),
(2, 0.00);
```

---

## 4. Pass Criteria Summary

| Category | Pass Criteria |
|----------|---------------|
| Wallet Operations | Create, credit, debit all work |
| Transactions | Recording, history, filters work |
| Payment Integration | Apply, refund, partial work |
| Cashback | Rules, calculation, credit work |
| UI | Pages display data, actions work |

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*