# Architecture - ksf_FA_Wallet

## Document Information
| Field | Value |
|-------|-------|
| **Module** | ksf_FA_Wallet |
| **Version** | 1.0.0 |
| **Author** | KSFII Development Team |
| **Date** | 2026-05-12 |

---

## 1. Architecture Overview

### 1.1 Module Classification

**ksf_FA_Wallet** is classified as a **FrontAccounting Thin Adapter** module with integration to **ksf_Wallet_Core** (extracted from TeraWallet/WooCommerce). It provides:
- FrontAccounting-specific database adapters
- FA hooks integration
- Admin UI pages in FA's application structure
- Database schema using FA's table prefix conventions

### 1.2 Architecture Pattern

The module follows the **Business Logic + Platform Adapter** pattern:

```
┌─────────────────────────────────────────────────────────────────┐
│                     Business Logic Layer                         │
│                       (ksf_Wallet_Core)                         │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │ WalletManager │ Transaction │ CashbackService │           ││
│  │ ValueObjects: Money, WalletId                              ││
│  │ Contracts: WalletServiceInterface                          ││
│  └─────────────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                    Platform Adapter Layer                        │
│                        ksf_FA_Wallet                            │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐  │
│  │ FA_Wallet       │  │ wallet_         │  │ wallet_         │  │
│  │ Repository      │  │ integration.php  │  │ transactions_db │  │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘  │
│  ┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐  │
│  │   hooks.php     │  │   pages/       │  │  sql/install.sql│  │
│  └─────────────────┘  └─────────────────┘  └─────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
                              │
                              ▼
┌─────────────────────────────────────────────────────────────────┐
│                   FrontAccounting Platform                       │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────────────┐  │
│  │ debtors_master│  │debtor_trans │  │  CRM Application     │  │
│  └──────────────┘  └──────────────┘  └──────────────────────┘  │
└─────────────────────────────────────────────────────────────────┘
```

### 1.3 Design Principles

| Principle | Application |
|-----------|-------------|
| **SOLID** | Each service handles single responsibility |
| **DRY** | Shared transaction logic |
| **TDD** | Tests cover core functionality |
| **DI** | Services injected for testability |
| **SRP** | Repository handles data access |

---

## 2. Component Architecture

### 2.1 Module Components

#### 2.1.1 Business Logic (ksf_Wallet_Core)

The core wallet functionality extracted from TeraWallet:

| Component | Responsibility | Public API |
|-----------|----------------|------------|
| `WalletManager` | Core wallet operations | `credit()`, `debit()`, `getBalance()`, `lockWallet()` |
| `Transaction` | Transaction entity | Properties: amount, type, reference, created_by |
| `CashbackService` | Cashback calculations | `calculateCashback()`, `creditCashback()` |
| `Money` | Value object for amounts | Immutable, currency-aware |

#### 2.1.2 Platform Adapter (ksf_FA_Wallet)

FA-specific integration layer:

| Component | Responsibility | Public API |
|-----------|----------------|------------|
| `FA_WalletRepository` | Database access | Implements WalletRepositoryInterface |
| `wallet_integration.php` | FA integration | `init_wallet_manager()` |
| `wallet_transactions_db.inc` | Transaction CRUD | `record_transaction()`, `get_transactions()` |

#### 2.1.3 Pages (pages/)

Admin UI pages for:
- Wallet management
- Transaction history
- Balance adjustments
- Cashback rule configuration

### 2.2 Class Diagram

```
┌─────────────────────────────────────────────────────────────────┐
│                    FA Wallet Module                             │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    ksf_Wallet_Core                          ││
│  ├─────────────────────────────────────────────────────────────┤│
│  │  ┌─────────────────┐    ┌─────────────────┐                ││
│  │  │  WalletManager  │    │     Cashback    │                ││
│  │  │                 │    │     Service     │                ││
│  │  │ + credit()      │    │                 │                ││
│  │  │ + debit()       │    │ + calculate()   │                ││
│  │  │ + getBalance()  │    │ + credit()      │                ││
│  │  │ + lockWallet()  │    │ + getRules()    │                ││
│  │  └────────┬────────┘    └─────────────────┘                ││
│  │           │                                                 ││
│  │           ▼                                                 ││
│  │  ┌─────────────────┐    ┌─────────────────┐                ││
│  │  │   Transaction    │    │   ValueObjects  │                ││
│  │  │   (Entity)      │    │   Money, WalletId│                ││
│  │  └─────────────────┘    └─────────────────┘                ││
│  └─────────────────────────────────────────────────────────────┘│
│                                                                 │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │                    ksf_FA_Wallet Adapter                   ││
│  ├─────────────────────────────────────────────────────────────┤│
│  │  ┌─────────────────┐    ┌─────────────────┐                ││
│  │  │ FA_Wallet       │    │ WalletService   │                ││
│  │  │ Repository      │    │ Interface       │                ││
│  │  │                 │    │                 │                ││
│  │  │ + getBalance()  │    │ + credit()      │                ││
│  │  │ + update()      │    │ + debit()       │                ││
│  │  │ + getTransactions│   │ + getBalance()  │                ││
│  │  └─────────────────┘    └─────────────────┘                ││
│  └─────────────────────────────────────────────────────────────┘│
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

### 2.3 Database Schema

#### fa_wallet_balances

```sql
CREATE TABLE fa_wallet_balances (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,                      -- Links to FA users or debtors
    balance DECIMAL(15,2) DEFAULT 0,
    is_locked TINYINT(1) DEFAULT 0,
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    UNIQUE KEY idx_user (user_id),
    INDEX idx_locked (is_locked)
);
```

#### fa_wallet_transactions

```sql
CREATE TABLE fa_wallet_transactions (
    id INT AUTO_INCREMENT PRIMARY KEY,
    user_id INT NOT NULL,
    type ENUM('credit', 'debit', 'cashback', 'transfer_in', 'transfer_out') NOT NULL,
    amount DECIMAL(15,2) NOT NULL,
    balance_after DECIMAL(15,2) NOT NULL,
    reference_type VARCHAR(50) DEFAULT NULL,   -- 'order', 'invoice', 'refund', etc.
    reference_id INT DEFAULT NULL,            -- ID of the related record
    details TEXT,
    created_by INT DEFAULT NULL,               -- User who created the transaction
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    deleted TINYINT(1) DEFAULT 0,
    INDEX idx_user (user_id),
    INDEX idx_type (type),
    INDEX idx_created (created_at),
    INDEX idx_reference (reference_type, reference_id)
);
```

#### fa_wallet_cashback_rules

```sql
CREATE TABLE fa_wallet_cashback_rules (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    percentage DECIMAL(5,2) NOT NULL,         -- Cashback percentage
    min_order_amount DECIMAL(15,2) DEFAULT 0,  -- Minimum order for cashback
    max_cashback DECIMAL(15,2) DEFAULT NULL,  -- Maximum cashback cap
    is_active TINYINT(1) DEFAULT 1,
    validity_days INT DEFAULT NULL,           -- Days until cashback expires
    created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
    updated_at DATETIME DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
```

---

## 3. Data Flow Architecture

### 3.1 Credit (Top-up) Flow

```
┌─────────────┐    ┌──────────────┐    ┌─────────────────────┐
│ Admin       │───▶│ wallet      │───▶│ WalletManager       │
│ (UI Form)   │    │ adjust_page  │    │ credit()            │
└─────────────┘    └──────────────┘    └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ Validate:           │
                                         │ - User exists       │
                                         │ - Amount > 0        │
                                         │ - Wallet not locked │
                                         └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ FA_WalletRepository │
                                         │ - Update balance    │
                                         │ - Record transaction│
                                         └─────────────────────┘
```

### 3.2 Debit (Payment) Flow

```
┌─────────────┐    ┌──────────────┐    ┌─────────────────────┐
│ Checkout    │───▶│ Apply wallet │───▶│ WalletManager       │
│             │    │ to order     │    │ debit()             │
└─────────────┘    └──────────────┘    └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ Validate:           │
                                         │ - Balance >= amount │
                                         │ - Wallet not locked │
                                         └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ Update FA:          │
                                         │ - Deduct from wallet│
                                         │ - Create order     │
                                         └─────────────────────┘
```

### 3.3 Cashback Flow

```
┌─────────────┐    ┌──────────────┐    ┌─────────────────────┐
│ Order       │───▶│ Order        │───▶│ CashbackService     │
│ Completed   │    │ Complete Hook │    │ calculateCashback()  │
└─────────────┘    └──────────────┘    └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ Check Rules:        │
                                         │ - Min order amount │
                                         │ - Percentage       │
                                         │ - Max cap          │
                                         └─────────────────────┘
                                                  │
                                                  ▼
                                         ┌─────────────────────┐
                                         │ Auto-credit:        │
                                         │ - Credit to wallet  │
                                         │ - Record as type   │
                                         │   'cashback'       │
                                         └─────────────────────┘
```

---

## 4. Integration Architecture

### 4.1 FrontAccounting Integration Points

#### Security Areas
```php
// Define security section and areas
define('SS_WALLET', 140 << 8);
$security_areas['SA_WALLETVIEW'] = array(SS_WALLET | 1, _("View Wallet"));
$security_areas['SA_WALLETADMIN'] = array(SS_WALLET | 2, _("Wallet Administration"));
$security_areas['SA_WALLETADJUST'] = array(SS_WALLET | 3, _("Adjust Wallet Balance"));
```

#### Menu Integration
```php
// Under appropriate application (Sales or CRM)
$app->add_lapp_function(0, _("Wallet Management"), ..., 'SA_WALLETVIEW', MENU_ENTRY);
$app->add_lapp_function(1, _("Wallet Transactions"), ..., 'SA_WALLETVIEW', MENU_INQUIRY);
$app->add_lapp_function(2, _("Cashback Rules"), ..., 'SA_WALLETADMIN', MENU_SETTINGS);
```

### 4.2 Customer Integration

Wallets link to FrontAccounting customers via user_id:

```sql
SELECT w.*, u.user_name, u.real_name, d.name as customer_name
FROM fa_wallet_balances w
JOIN users u ON w.user_id = u.user_id
LEFT JOIN debtors_master d ON u.debtor_no = d.debtor_no
WHERE w.user_id = ?
```

### 4.3 Payment Integration

Wallet as payment method at checkout:

```php
class PaymentGateway_Wallet implements PaymentGateway {
    public function process_payment($amount, $user_id) {
        $walletManager = init_wallet_manager();
        return $walletManager->debit($user_id, $amount, 'Order payment');
    }
}
```

---

## 5. Module Structure

```
ksf_FA_Wallet/
├── hooks.php                    # FA hooks class
├── includes/
│   ├── wallet_integration.php   # Core integration
│   ├── wallet_transactions_db.inc # Transaction CRUD
│   └── FA_WalletRepository.php  # Repository implementation
├── pages/
│   ├── wallet_ui.inc            # UI rendering functions
│   ├── wallet_admin.php         # Admin pages
│   └── wallet_adjust.php        # Balance adjustment
├── sql/
│   ├── install.sql              # Schema installation
│   ├── update.sql               # Version migrations
│   └── fa_wallet_transactions.sql # Transaction table
└── ProjectDcs/
    ├── Business Requirements.md
    ├── Architecture.md
    ├── Functional Requirements.md
    ├── Use Case.md
    ├── Test Plan.md
    └── UAT Plan.md
```

---

## 6. Error Handling

### 6.1 Insufficient Balance

```php
public function debit(int $userId, float $amount, string $reason): bool
{
    $balance = $this->getBalance($userId);
    if ($balance < $amount) {
        throw new InsufficientBalanceException(
            "Insufficient balance. Required: $amount, Available: $balance"
        );
    }
    // ... proceed with debit
}
```

### 6.2 Wallet Locked

```php
public function credit(int $userId, float $amount, string $reason): bool
{
    if ($this->isLocked($userId)) {
        throw new WalletLockedException("Wallet is locked for user $userId");
    }
    // ... proceed with credit
}
```

### 6.3 Invalid Amount

```php
public function validateAmount(float $amount): bool
{
    if ($amount <= 0) {
        throw new InvalidAmountException("Amount must be greater than zero");
    }
    if ($amount > MAX_TRANSACTION_AMOUNT) {
        throw new InvalidAmountException("Amount exceeds maximum allowed");
    }
    return true;
}
```

---

*Document Version: 1.0.0*  
*Last Updated: 2026-05-12*