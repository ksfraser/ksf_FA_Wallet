# Use Case - ksf_FA_Wallet

## Document Information
| Field | Value |
|-------|-------|
| **Module** | ksf_FA_Wallet |
| **Version** | 1.0.0 |
| **Author** | KSFII Development Team |
| **Date** | 2026-05-12 |

---

## 1. Use Case Overview

### 1.1 Actor Definitions

| Actor | Description | Role |
|-------|-------------|------|
| **Customer** | End customer with wallet | Top-up, make payments, view balance |
| **Administrator** | System administrator | Manage wallets, adjust balances, configure |
| **Sales** | Sales team member | View customer wallets |
| **Finance** | Finance team member | Review transactions, reconcile |
| **System** | Automated process | Cashback, refunds |

### 1.2 Use Case Summary

| ID | Use Case | Primary Actor | Priority |
|----|----------|---------------|----------|
| UC-001 | View Wallet Balance | Customer | High |
| UC-002 | Top-up Wallet | Customer/Admin | High |
| UC-003 | Use Wallet for Payment | Customer | High |
| UC-004 | Process Refund to Wallet | System/Admin | High |
| UC-005 | Adjust Wallet Balance | Admin | High |
| UC-006 | View Transaction History | Customer/Admin | High |
| UC-007 | Lock/Unlock Wallet | Admin | Medium |
| UC-008 | Configure Cashback Rules | Admin | High |
| UC-009 | Cashback Credited | System | High |
| UC-010 | Search Transactions | Admin | Medium |
| UC-011 | Partial Payment | Customer | High |

---

## 2. Use Case Specifications

### UC-001: View Wallet Balance

**Primary Actor:** Customer  
**Priority:** High  
**Description:** Customer views current wallet balance and status.

#### Preconditions
- Customer has wallet account
- Customer logged in

#### Basic Flow
1. Customer navigates to "My Wallet" page
2. System displays:
   - Current balance
   - Wallet status (active/locked)
   - Recent transactions (last 5)
3. Customer can view full transaction history

#### Postconditions
- Balance displayed accurately

#### Acceptance Criteria
- Balance matches actual
- Status shown correctly
- Recent transactions visible

---

### UC-002: Top-up Wallet

**Primary Actor:** Customer / Administrator  
**Priority:** High  
**Description:** Add funds to wallet via manual credit.

#### Preconditions
- Wallet exists for user
- Wallet not locked
- Amount to credit is known

#### Basic Flow
1. Admin navigates to Wallet Management
2. Admin searches/selects user
3. Admin clicks "Adjust Balance"
4. Admin selects "Credit" type
5. Admin enters amount
6. Admin enters details/reason
7. Admin clicks "Submit"
8. System validates:
   - Amount > 0
   - Wallet not locked
9. System credits wallet:
   - balance += amount
   - Record transaction
10. System displays success notification

#### Alternative Flows

**A1: Insufficient Input**
- At step 8, amount not provided or invalid
- System displays error
- Admin corrects and resubmits

**A2: Wallet Locked**
- At step 8, wallet is locked
- System displays error
- Admin must unlock first

#### Postconditions
- Balance increased
- Transaction recorded

#### Acceptance Criteria
- Balance updated correctly
- Transaction recorded with details
- Audit trail complete

---

### UC-003: Use Wallet for Payment

**Primary Actor:** Customer  
**Priority:** High  
**Description:** Customer applies wallet balance to order payment.

#### Preconditions
- Customer has wallet with sufficient balance
- Wallet not locked
- Order/invoice exists

#### Basic Flow
1. Customer at checkout
2. Customer selects "Pay with Wallet"
3. Customer enters amount to use (up to balance)
4. System validates:
   - Amount <= balance
   - Wallet not locked
5. System deducts from wallet:
   - balance -= amount
   - Record transaction type='debit', reference to order
6. System returns remaining amount for other payment methods
7. Customer completes remaining payment

#### Postconditions
- Wallet balance reduced
- Transaction recorded
- Order linked to payment

#### Acceptance Criteria
- Correct amount deducted
- Transaction linked to order
- Remaining balance calculated

---

### UC-004: Process Refund to Wallet

**Primary Actor:** System / Administrator  
**Priority:** High  
**Description:** Refund processed as wallet credit.

#### Preconditions
- Original order had wallet payment
- Refund approved

#### Basic Flow
1. System/Admin initiates refund
2. System determines:
   - Original payment amount from wallet
   - User wallet ID
3. System credits wallet:
   - balance += refund_amount
   - Record transaction type='credit', reference='refund'
4. System notifies customer of refund

#### Postconditions
- Wallet credited
- Transaction recorded with refund reference

#### Acceptance Criteria
- Correct amount credited
- Refund linked to original order
- Customer notified

---

### UC-005: Adjust Wallet Balance

**Primary Actor:** Administrator  
**Priority:** High  
**Description:** Admin manually adjusts customer wallet balance.

#### Preconditions
- Admin has SA_WALLETADJUST permission
- Valid reason for adjustment

#### Basic Flow
1. Admin navigates to Wallet Management
2. Admin selects customer
3. Admin clicks "Adjust Balance"
4. Admin selects:
   - Type: Credit or Debit
   - Amount: [value]
   - Reason: [description]
5. Admin submits adjustment
6. System validates all inputs
7. System applies adjustment
8. System records transaction with reason
9. System displays updated balance

#### Alternative Flow

**A1: Debit Exceeds Balance**
- At step 6, debit amount > balance
- System displays error
- Admin adjusts amount or cancels

#### Postconditions
- Balance adjusted
- Transaction recorded with reason
- Audit trail complete

#### Acceptance Criteria
- Adjustment applied correctly
- Reason recorded
- Balance accurate

---

### UC-006: View Transaction History

**Primary Actor:** Customer / Administrator  
**Priority:** High  
**Description:** View complete transaction history for wallet.

#### Preconditions
- Wallet exists
- Transactions exist

#### Basic Flow
1. User navigates to wallet detail or transaction history page
2. System displays transactions:
   - Date/time
   - Type (credit/debit/cashback)
   - Amount
   - Balance after
   - Reference
   - Details
3. User can filter by:
   - Date range
   - Type
   - Reference type
4. User can export transactions

#### Postconditions
- Transaction history displayed

#### Acceptance Criteria
- All transactions shown
- Filters work correctly
- Pagination functional

---

### UC-007: Lock/Unlock Wallet

**Primary Actor:** Administrator  
**Priority:** Medium  
**Description:** Admin temporarily disables wallet.

#### Preconditions
- Admin has SA_WALLETADMIN permission
- Valid reason for lock/unlock

#### Basic Flow - Lock
1. Admin navigates to customer wallet
2. Admin clicks "Lock Wallet"
3. Admin confirms action
4. System updates:
   - is_locked = true
5. System logs action

#### Basic Flow - Unlock
1. Admin navigates to customer wallet
2. Admin clicks "Unlock Wallet"
3. Admin confirms action
4. System updates:
   - is_locked = false
5. System logs action

#### Postconditions
- Wallet status changed
- Operations blocked/enabled

#### Acceptance Criteria
- Status updated correctly
- Operations blocked when locked

---

### UC-008: Configure Cashback Rules

**Primary Actor:** Administrator  
**Priority:** High  
**Description:** Admin creates/edits cashback program rules.

#### Preconditions
- Admin has SA_WALLETADMIN permission

#### Basic Flow
1. Admin navigates to Cashback Rules settings
2. Admin clicks "Add New Rule"
3. Admin enters:
   - Rule name
   - Cashback percentage (e.g., 5%)
   - Minimum order amount
   - Maximum cashback cap (optional)
   - Validity period (days until expiry)
4. Admin saves rule
5. System validates inputs
6. System creates rule
7. Rule active for new orders

#### Alternative Flow - Edit
1. Admin selects existing rule
2. Admin modifies settings
3. Admin saves
4. System updates rule

#### Postconditions
- Cashback rule created/updated
- Applied to qualifying orders

#### Acceptance Criteria
- Rule configured correctly
- Multiple rules supported
- Activation/deactivation works

---

### UC-009: Cashback Credited

**Primary Actor:** System (Automated)  
**Priority:** High  
**Description:** System automatically credits cashback on qualifying orders.

#### Preconditions
- Order completed
- Customer has wallet
- Order meets cashback threshold

#### Basic Flow
1. Order marked as completed
2. System triggers cashback calculation
3. System finds applicable rule
4. System calculates cashback:
   - amount = order_total × percentage
   - Apply max cap if reached
5. System credits to wallet:
   - balance += cashback_amount
   - Record transaction type='cashback'
   - Link to order
6. System notifies customer

#### Postconditions
- Wallet credited with cashback
- Transaction recorded
- Customer notified

#### Acceptance Criteria
- Correct percentage applied
- Cap respected if set
- Transaction linked to order

---

### UC-010: Search Transactions

**Primary Actor:** Administrator  
**Priority:** Medium  
**Description:** Admin searches transactions by various criteria.

#### Preconditions
- Admin has access to transaction search

#### Basic Flow
1. Admin navigates to Transaction Search page
2. Admin enters search criteria:
   - User (optional)
   - Date range
   - Type (credit/debit/cashback)
   - Reference type/order ID
3. Admin clicks "Search"
4. System returns matching transactions
5. Admin can export results

#### Postconditions
- Search results displayed

#### Acceptance Criteria
- All filters work
- Results accurate
- Export functional

---

### UC-011: Partial Payment

**Primary Actor:** Customer  
**Priority:** High  
**Description:** Customer uses wallet for partial payment on order.

#### Preconditions
- Customer has wallet with balance
- Balance < order total
- Wallet not locked

#### Basic Flow
1. Customer at checkout with order total $150
2. Customer wallet balance = $75
3. Customer selects "Pay with Wallet"
4. System shows: "You have $75 available"
5. Customer confirms use of $75 from wallet
6. System debits $75 from wallet
7. System returns: "Remaining amount: $75"
8. Customer pays remaining via other method

#### Postconditions
- $75 debited from wallet
- $75 remaining for other payment
- Transaction recorded

#### Acceptance Criteria
- Correct amount debited
- Remaining calculated correctly
- Transaction linked to order

---

## 3. Use Case Relationships

### 3.1 Use Case Diagram

```
                    ┌─────────────────────┐
                    │     Customer        │
                    └──────────┬───────────┘
                               │
          ┌────────────────────┼────────────────────┐
          │                    │                    │
          ▼                    ▼                    ▼
    ┌───────────┐       ┌───────────┐       ┌───────────┐
    │ View      │       │ Top-up    │       │ Pay with  │
    │ Balance   │       │ Wallet    │       │ Wallet    │
    └───────────┘       └───────────┘       └───────────┘
          │                                      │
          │                                      │
          ▼                                      ▼
    ┌─────────────────────────────────────────────────┐
    │              Administrator                     │
    ├─────────────────────────────────────────────────┤
    │                                                 │
    │  ┌─────────────┐    ┌─────────────┐           │
    │  │ Adjust     │    │ Lock/Unlock  │           │
    │  │ Balance    │    │ Wallet      │           │
    │  └─────────────┘    └─────────────┘           │
    │         │                  │                   │
    │         └──────────────────┘                   │
    │                │                               │
    │                ▼                               │
    │         ┌─────────────┐                       │
    │         │ Configure   │                       │
    │         │ Cashback    │                       │
    │         └─────────────┘                       │
    └─────────────────────────────────────────────────┘
                              │
                              ▼
                    ┌─────────────────────┐
                    │       System        │
                    ├─────────────────────┤
                    │                     │
    ┌───────────────┼───────────┬─────────┼───────────────┐
    │               │           │         │               │
    ▼               ▼           ▼         ▼               ▼
┌───────────┐ ┌───────────┐ ┌───────────┐ ┌───────────┐ ┌───────────┐
│ Refund to │ │ Cashback  │ │ Check     │ │ Search    │ │ Partial   │
│ Wallet    │ │ Credit    │ │ Expiry    │ │ Trans     │ │ Payment   │
└───────────┘ └───────────┘ └───────────┘ └───────────┘ └───────────┘
```

### 3.2 Wallet State Diagram

```
                    ┌──────────────┐
                    │   ACTIVE     │
                    │  (balance)   │
                    └──────┬───────┘
                           │
            ┌──────────────┼──────────────┐
            │              │              │
      Credit│         Debit│         Lock │
            │              │              │
            ▼              ▼              ▼
      ┌──────────┐   ┌──────────┐   ┌──────────┐
      │ Balance  │   │ Balance  │   │  LOCKED  │
      │ Increases│   │ Decreases│   │ (blocked)│
      └──────────┘   └──────────┘   └─────┬────┘
                                           │
                                      Unlock
                                           │
                                           ▼
                                    ┌──────────────┐
                                    │   ACTIVE    │
                                    └──────────────┘
```

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*