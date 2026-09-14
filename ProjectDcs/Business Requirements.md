# Business Requirements - ksf_FA_Wallet

## Document Information
| Field | Value |
|-------|-------|
| **Module** | ksf_FA_Wallet |
| **Version** | 1.0.0 |
| **Author** | KSFII Development Team |
| **Date** | 2026-05-12 |
| **Status** | Implemented |

---

## 1. Project Overview

### 1.1 Module Purpose

**ksf_FA_Wallet** is a FrontAccounting adapter module that provides a digital wallet system for customers. Extracted and adapted from TeraWallet/WooCommerce, this module enables customers to store funds, make partial payments, receive cashback, and transfer funds between accounts within the FrontAccounting ecosystem.

### 1.2 Business Context

Modern business models require flexible payment options:

1. **Stored Value**: Customers can preload funds for future purchases
2. **Partial Payments**: Flexible payment splitting across methods
3. **Cashback Programs**: Reward customers with store credits
4. **Peer Transfers**: Transfer funds between customer accounts

### 1.3 Target Users

| User Type | Role Description |
|-----------|------------------|
| **Customers** | Store funds, make payments, receive cashback |
| **Sales Team** | View customer wallet balances, process adjustments |
| **Administrators** | Configure wallet settings, manage cashback rules |
| **Finance** | Review transactions, reconcile wallet accounts |

---

## 2. Problem Statement

### 2.1 Business Problem

Organizations using FrontAccounting often need customer wallet functionality:

1. **Payment Flexibility**: Customers want to split payments between methods
2. **Loyalty Programs**: Need to award and track store credits
3. **Prepaid Accounts**: Customers want to fund accounts in advance
4. **Refunds**: Need efficient way to process store credits vs cash

### 2.2 Current State

Without this module, FrontAccounting users must:
- Process all refunds as cash
- Manually track customer credits
- Use external wallet systems disconnected from FA
- Lack visibility into customer prepaid balances

### 2.3 Desired State

With **ksf_FA_Wallet**, organizations can:
- Enable customer wallet accounts in FA
- Process partial payments from wallet
- Automatically award cashback on purchases
- Transfer funds between customer accounts

---

## 3. Project Scope

### 3.1 In-Scope Features

#### Wallet Management
- Create wallet account for customers
- View wallet balance
- Top-up (credit) wallet
- Withdraw (debit) wallet
- Lock/unlock wallet

#### Transaction Management
- Record all wallet transactions
- Transaction types: credit, debit, cashback, transfer
- Transaction history per customer
- Transaction details (date, amount, reference, notes)

#### Payment Integration
- Use wallet balance for partial payment
- Apply wallet to invoices
- Combined payment (wallet + other methods)

#### Cashback Program
- Configurable cashback rules
- Automatic cashback on purchases
- Cashback threshold settings
- Cashback expiration

#### Admin Interface
- View all customer wallets
- Adjust balances manually
- Transaction search
- Cashback rule configuration

### 3.2 Out-of-Scope Features

The following are explicitly not in scope for this version:

- Payment gateway integration
- Withdrawal to external accounts
- Multi-currency wallets
- Wallet-to-wallet messaging
- Automated top-up scheduling

### 3.3 Module Boundaries

```
┌─────────────────────────────────────────────────────────────────┐
│                        ksf_FA_Wallet                            │
│  ┌─────────────┐  ┌──────────────┐  ┌────────────────────────┐  │
│  │ Wallet      │  │ Transaction  │  │ Cashback              │  │
│  │ Management  │  │ Management    │  │ Program               │  │
│  └─────────────┘  └──────────────┘  └────────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
         │                   │                       │
         ▼                   ▼                       ▼
┌─────────────────┐ ┌─────────────────┐ ┌─────────────────────────┐
│  ksf_FA_CRM     │ │  ksf_FA_Payments│ │  ksf_FA_Subscriptions  │
│  (Customer Link)│ │  (AR Integration)│ │  (Payment Link)        │
└─────────────────┘ └─────────────────┘ └─────────────────────────┘
                              │
                              ▼
                    ┌─────────────────┐
                    │ FrontAccounting  │
                    │ Core Platform    │
                    └─────────────────┘
```

---

## 4. Feature Specifications

### 4.1 Wallet Operations

| Feature ID | Feature | Description |
|------------|---------|-------------|
| WAL-001 | Create Wallet | Create wallet for customer |
| WAL-002 | Get Balance | Retrieve current wallet balance |
| WAL-003 | Credit Wallet | Add funds to wallet (top-up) |
| WAL-004 | Debit Wallet | Remove funds from wallet (withdrawal) |
| WAL-005 | Lock Wallet | Temporarily disable wallet |
| WAL-006 | Unlock Wallet | Re-enable locked wallet |
| WAL-007 | List Wallets | View all customer wallets |

### 4.2 Transaction Management

| Feature ID | Feature | Description |
|------------|---------|-------------|
| TXN-001 | Record Transaction | Log all wallet balance changes |
| TXN-002 | Transaction Types | Support credit, debit, cashback, transfer |
| TXN-003 | Transaction Reference | Link to order, invoice, or other reference |
| TXN-004 | Transaction Notes | Add description/notes to transaction |
| TXN-005 | Transaction History | View all transactions for wallet |
| TXN-006 | Search Transactions | Filter by date, type, amount, reference |

### 4.3 Payment Integration

| Feature ID | Feature | Description |
|------------|---------|-------------|
| PAY-001 | Partial Payment | Use wallet for part of order payment |
| PAY-002 | Apply to Invoice | Apply wallet balance to FA invoice |
| PAY-003 | Payment Method | Integrate as payment option at checkout |
| PAY-004 | Refund to Wallet | Process refunds as wallet credit |

### 4.4 Cashback Program

| Feature ID | Feature | Description |
|------------|---------|-------------|
| CB-001 | Cashback Rules | Configure automatic cashback percentage |
| CB-002 | Minimum Threshold | Set minimum order amount for cashback |
| CB-003 | Cashback Credit | Automatically credit cashback to wallet |
| CB-004 | Cashback Expiry | Configure expiration period for cashback |
| CB-005 | Cashback History | Track cashback earned and used |

---

## 5. Integration Dependencies

### 5.1 Required Dependencies

| Module | Purpose | Integration Type |
|--------|---------|-------------------|
| **FrontAccounting 2.4+** | Core platform | Runtime dependency |
| **ksf_FA_CRM** | Customer integration | Database (debtor_no linking) |

### 5.2 Optional Dependencies

| Module | Purpose | Integration Type |
|--------|---------|-------------------|
| **ksf_FA_Subscriptions** | Subscription payments | Future integration |
| **ksf_FA_Orders** | Order payment integration | Future integration |

### 5.3 Database Dependencies

| Table | Type | Purpose |
|-------|------|---------|
| `debtors_master` | External (FA) | Customer information |
| `debtor_trans` | External (FA) | AR transactions |
| `fa_wallet_balances` | Local | Customer wallet balances |
| `fa_wallet_transactions` | Local | Transaction records |
| `fa_wallet_cashback_rules` | Local | Cashback configuration |

---

## 6. Non-Functional Requirements

### 6.1 Performance

- Balance lookup: < 100ms
- Transaction processing: < 200ms
- Batch processing: Handle 100+ transactions per batch

### 6.2 Security

- Wallet operations protected by FA security areas
- Authorization checks on all operations
- Audit trail for all balance changes
- Fraud detection (negative balance prevention)

### 6.3 Compatibility

- Compatible with FrontAccounting 2.4, 2.5, and 2.6
- PHP 8.1+ required
- UTF-8mb4 database encoding

---

## 7. Acceptance Criteria

| ID | Criterion | Validation Method |
|----|-----------|-------------------|
| AC-001 | Wallets created for customers | Manual test |
| AC-002 | Credit/debit operations work | Manual test |
| AC-003 | Transactions recorded with details | Manual test |
| AC-004 | Wallet used for partial payment | Manual test |
| AC-005 | Cashback awarded automatically | Manual test |
| AC-006 | Admin can view and adjust balances | Manual test |
| AC-007 | Lock/unlock functionality works | Manual test |

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*