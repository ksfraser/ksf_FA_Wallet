# UAT Plan - ksf_FA_Wallet

## Document Information
| Field | Value |
|-------|-------|
| **Module** | ksf_FA_Wallet |
| **Version** | 1.0.0 |
| **Author** | KSFII Development Team |
| **Date** | 2026-05-12 |

---

## 1. UAT Overview

### 1.1 Purpose

User Acceptance Testing (UAT) validates that the ksf_FA_Wallet module meets business requirements and is ready for production deployment.

### 1.2 UAT Objectives

1. Verify wallet creation and management works
2. Validate credit/debit operations
3. Confirm transaction recording and history
4. Ensure cashback program functions correctly
5. Sign-off on module readiness

### 1.3 Success Criteria

| Criterion | Definition |
|-----------|------------|
| **Wallet Operations** | Create, credit, debit all work correctly |
| **Transaction Recording** | All transactions recorded with details |
| **Payment Integration** | Wallet used for payments successfully |
| **Cashback Program** | Cashback credited automatically |
| **UI** | Pages functional and data accurate |

---

## 2. UAT Scope

### 2.1 In Scope

- Wallet creation and management
- Credit/debit operations
- Transaction history
- Balance adjustments
- Cashback rules and credits
- Payment integration

### 2.2 Out of Scope

- Payment gateway integration
- Withdrawal to external accounts
- Automated top-up scheduling

---

## 3. UAT Scenarios

### 3.1 Wallet Operations Scenarios

#### UAT-WAL-001: View Customer Wallet
**Scenario:** Admin views customer's wallet balance

**Preconditions:**
- Admin logged into FrontAccounting
- Customer has wallet with balance

**Test Steps:**
1. Navigate to Sales > Wallet Management
2. Search for customer "Test Customer A"
3. Click on customer to view wallet
4. Verify displayed:
   - Current balance: $150.00
   - Status: Active
   - Created date

**Expected Results:**
- Balance displayed correctly
- Status shown
- Transaction history visible

**Pass/Fail Criteria:**
- [PASS] Data accurate
- [FAIL] Balance mismatch or missing data

---

#### UAT-WAL-002: Top-up Customer Wallet
**Scenario:** Admin adds funds to customer wallet

**Preconditions:**
- Customer has existing wallet
- Wallet not locked

**Test Steps:**
1. Navigate to customer wallet detail
2. Click "Adjust Balance"
3. Select "Credit"
4. Enter amount: $100.00
5. Enter reason: "Customer top-up request"
6. Click "Submit"
7. Verify balance increased
8. Verify transaction recorded

**Expected Results:**
- Balance increased by $100.00
- Transaction recorded with details
- Transaction shows type "credit"

**Pass Criteria:** Balance updated, transaction complete

---

#### UAT-WAL-003: Deduct from Wallet
**Scenario:** Admin deducts funds from customer wallet

**Test Steps:**
1. Navigate to customer wallet (balance $250.00)
2. Click "Adjust Balance"
3. Select "Debit"
4. Enter amount: $50.00
5. Enter reason: "Refund processing"
6. Submit
7. Verify balance = $200.00
8. Verify transaction recorded

**Expected Results:**
- Balance decreased by $50.00
- Transaction recorded
- Reason logged

**Pass Criteria:** Correct deduction, transaction complete

---

#### UAT-WAL-004: Insufficient Balance
**Scenario:** Attempt to debit more than available balance

**Preconditions:** Wallet balance = $50.00

**Test Steps:**
1. Attempt to debit $100.00
2. Observe error message

**Expected Results:** Error: "Insufficient balance"

**Pass Criteria:** Error displayed, balance unchanged

---

#### UAT-WAL-005: Lock Wallet
**Scenario:** Admin locks customer wallet

**Test Steps:**
1. Navigate to customer wallet
2. Click "Lock Wallet"
3. Confirm action
4. Verify status changes to "Locked"
5. Attempt to credit wallet
6. Verify blocked with error

**Expected Results:**
- Status = "Locked"
- Credit operation rejected

**Pass Criteria:** Lock works, operations blocked

---

#### UAT-WAL-006: Unlock Wallet
**Scenario:** Admin unlocks previously locked wallet

**Preconditions:** Wallet is locked

**Test Steps:**
1. Navigate to customer wallet
2. Click "Unlock Wallet"
3. Confirm action
4. Verify status = "Active"
5. Verify operations work

**Expected Results:**
- Status = "Active"
- Operations allowed

**Pass Criteria:** Unlock works, operations restored

---

### 3.2 Transaction Scenarios

#### UAT-TXN-001: View Transaction History
**Scenario:** Admin reviews customer transaction history

**Preconditions:** Customer has multiple transactions

**Test Steps:**
1. Navigate to customer wallet detail
2. Click "Transaction History"
3. Verify transactions listed with:
   - Date and time
   - Type (credit/debit)
   - Amount
   - Balance after
   - Reference
   - Details

**Expected Results:** Complete transaction history displayed

**Pass Criteria:** All transactions visible with correct data

---

#### UAT-TXN-002: Filter Transactions by Type
**Scenario:** Admin filters to view only credit transactions

**Test Steps:**
1. On Transaction History page
2. Select filter: Type = "Credit"
3. Click Apply
4. Verify only credit transactions shown

**Expected Results:** Filter works, only credits shown

**Pass Criteria:** Filter accurate, count correct

---

#### UAT-TXN-003: Search Transactions by Date
**Scenario:** Admin searches transactions within date range

**Test Steps:**
1. On Transaction History page
2. Set date range: Last 30 days
3. Click Search
4. Verify only transactions in range

**Expected Results:** Date filter works correctly

**Pass Criteria:** Accurate date filtering

---

### 3.3 Payment Integration Scenarios

#### UAT-PAY-001: Use Wallet for Order Payment
**Scenario:** Customer uses wallet for partial payment

**Preconditions:**
- Customer wallet balance = $75.00
- Order total = $150.00

**Test Steps:**
1. Customer at checkout
2. Customer selects "Pay with Wallet"
3. System shows available balance
4. Customer confirms use of $75.00
5. System processes payment
6. Verify wallet debited $75.00
7. Verify remaining $75.00 for other payment

**Expected Results:**
- $75 deducted from wallet
- $75 remaining shown
- Transaction recorded

**Pass Criteria:** Correct amounts, remaining calculated

---

#### UAT-PAY-002: Refund to Wallet
**Scenario:** Admin processes refund to customer wallet

**Preconditions:** Order had wallet payment of $50.00

**Test Steps:**
1. Admin initiates refund for order
2. Admin selects "Refund to Wallet"
3. Admin confirms amount $50.00
4. System credits wallet
5. Verify balance increased
6. Verify transaction type = "credit" with refund details

**Expected Results:** Refund credited to wallet

**Pass Criteria:** Correct amount, transaction recorded

---

#### UAT-PAY-003: Full Wallet Payment
**Scenario:** Customer pays entirely from wallet

**Preconditions:**
- Customer wallet balance = $200.00
- Order total = $150.00

**Test Steps:**
1. Customer selects "Pay with Wallet"
2. System confirms sufficient balance
3. Customer confirms payment
4. Order paid in full
5. Verify wallet debited $150.00

**Expected Results:**
- Order paid in full
- Wallet debited correct amount

**Pass Criteria:** Full payment processed, correct amount

---

### 3.4 Cashback Scenarios

#### UAT-CB-001: Configure Cashback Rule
**Scenario:** Admin creates new cashback rule

**Test Steps:**
1. Navigate to Cashback Rules page
2. Click "Add New Rule"
3. Enter:
   - Name: "5% Loyalty Cashback"
   - Percentage: 5%
   - Minimum order: $50.00
   - Maximum cashback: $100.00
   - Validity: 90 days
4. Save rule
5. Verify rule listed as active

**Expected Results:**
- Rule created with all settings
- Rule active

**Pass Criteria:** Rule created correctly

---

#### UAT-CB-002: Automatic Cashback on Order
**Scenario:** Customer receives cashback on qualifying order

**Preconditions:**
- Cashback rule: 5%, min $50
- Customer has wallet
- Order total = $200.00

**Test Steps:**
1. Customer completes order for $200.00
2. System detects qualifying order
3. System calculates cashback: $10.00 (5% of $200)
4. System credits wallet
5. Verify balance increased by $10.00
6. Verify transaction type = "cashback"

**Expected Results:**
- $10.00 credited to wallet
- Transaction recorded as cashback

**Pass Criteria:** Cashback calculated and credited

---

#### UAT-CB-003: Cashback Below Threshold
**Scenario:** Order does not qualify for cashback (below minimum)

**Preconditions:**
- Cashback rule: 5%, min $50
- Customer wallet exists
- Order total = $30.00

**Test Steps:**
1. Customer completes order for $30.00
2. System checks cashback eligibility
3. Verify NO cashback credited
4. Verify no transaction created

**Expected Results:** No cashback (below threshold)

**Pass Criteria:** Threshold checked, no credit

---

#### UAT-CB-004: Cashback with Cap
**Scenario:** Cashback reaches maximum cap

**Preconditions:**
- Cashback rule: 10%, max $50
- Customer wallet exists
- Order total = $1000.00

**Test Steps:**
1. Customer completes order for $1000.00
2. System calculates: 10% = $100.00
3. System applies cap: $50.00
4. System credits $50.00
5. Verify balance increased by $50.00

**Expected Results:** Cashback capped at $50.00

**Pass Criteria:** Cap applied, $50 credited

---

### 3.5 UI Scenarios

#### UAT-UI-001: Wallet List Navigation
**Scenario:** Admin navigates wallet list

**Test Steps:**
1. Navigate to Wallet Management
2. Verify wallet table displayed
3. Sort by balance
4. Verify sort works
5. Use pagination if many wallets

**Expected Results:** List navigable with sorting

**Pass Criteria:** Sorting and pagination work

---

#### UAT-UI-002: Filter Wallets by Status
**Scenario:** Admin filters to show locked wallets only

**Test Steps:**
1. On Wallet List page
2. Select filter: Status = "Locked"
3. Apply filter
4. Verify only locked wallets shown

**Expected Results:** Filter works correctly

**Pass Criteria:** Only locked wallets displayed

---

## 4. Test Environment

### 4.1 Environment Requirements

| Component | Specification |
|-----------|---------------|
| FrontAccounting | Version 2.4 or higher |
| PHP | 8.1+ |
| Database | MySQL with test company |
| Browser | Chrome/Edge latest |
| User Accounts | Admin, Customer |

### 4.2 Test Data Setup

| Data Type | Quantity | Notes |
|-----------|----------|-------|
| Users | 10 | For wallet testing |
| Wallets | 8 | Various balances |
| Transactions | 50 | Mixed types |
| Cashback Rules | 3 | Active rules |

---

## 5. UAT Schedule

### 5.1 Timeline

| Phase | Duration | Activities |
|-------|----------|------------|
| Setup | 0.5 day | Environment, test data |
| Execution | 2 days | Run all UAT scenarios |
| Defect Resolution | 0.5 day | Fix critical defects |
| Sign-off | 0.5 day | Review, sign-off |

### 5.2 Roles

| Role | Responsibilities |
|------|------------------|
| UAT Lead | Coordinate testing |
| Business Users | Execute scenarios |
| Developer | Support, fix defects |

---

## 6. Sign-off Requirements

### 6.1 Sign-off Criteria

| Category | Requirement | Status |
|----------|-------------|--------|
| Wallet Operations | Create, credit, debit all work | ☐ |
| Transactions | Recording, history work | ☐ |
| Payment Integration | Apply, refund work | ☐ |
| Cashback | Rules, credits work | ☐ |
| UI | Pages functional | ☐ |

### 6.2 Sign-off Template

```
Module: ksf_FA_Wallet
Version: 1.0.0
UAT Date: [Date]

Test Results Summary:
- Total Scenarios: [X]
- Passed: [X]
- Failed: [X]

Sign-off Decision: [APPROVED / NOT APPROVED]

Signatories:
Business Owner: _________________ Date: _____
Technical Lead: _________________ Date: _____
```

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*