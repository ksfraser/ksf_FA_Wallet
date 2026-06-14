# AGENTS.md - ksf_FA_Wallet#

## Architecture Overview#

This repository implements **Digital Wallet System** extracted from TeraWallet/WooCommerce - supports top-ups, partial payments, cashback, and peer-to-peer transfers.

### Core Principles#
- **SOLID**: Single Responsibility, Open/Closed, Liskov Substitution, Interface Segregation, Dependency Inversion#
- **DRY**: Don't Repeat Yourself - extract reusable logic#
- **TDD**: Test-Driven Development - write tests first#
- **DI**: Dependency Injection - inject dependencies, don't hardcode#
- **SRP**: Single Responsibility Principle - each class has one reason to change#

## Repository Structure#

```
ksf_FA_Wallet/
├── sql/                    # Database schemas (FA TB_PREF tables)#
│   ├── fa_wallet_balances.sql#
│   ├── fa_wallet_transactions.sql#
│   └── fa_wallet_cashback_rules.sql#
├── includes/              # FA-specific DB classes#
│   ├── wallet_db.inc#
│   ├── wallet_transactions_db.inc#
│   └── ...#
├── src/                    # Business logic (namespace: Ksf\FA\Wallet\)#
│   ├── Contracts/        # WalletServiceInterface#
│   ├── Services/         # WalletService, CashbackService#
│   └── ValueObjects/    # Money, WalletId#
├── pages/                 # UI pages (FA admin)#
├── hooks.php#
├── composer.json#
└── ProjectDocs/#
    ├── Requirements.md#
    ├── RTM.md#
    ├── BABOK.md#
    └── UML.md#
```

## Dependencies#

- **ksf_FA_Wallet_Core** (business logic - extracted from TeraWallet)#
- **ksf_FA_CRM** (customer contacts)#
- **FrontAccounting 2.4+**#

## Development Workflow

All development is done in the **devel tree** (`~/Documents/ksf_FA_Wallet`). Do **not** edit files in the UAT bind point directly.

### Workflow Steps
1. **Develop** in this repo (feature branches preferred)
2. **Test**: run repo-appropriate tests
3. **Lint**: `php -l` on modified PHP files (no syntax errors)
4. **Commit** and **Push** branch to GitHub
5. **Merge** to `master` when ready
6. **Push** `master` to GitHub
7. **Deploy** to UAT by pulling in the Infrastructure bind point:

   ```
   cd ~/ksf_Infrastructure/fa_modules/ksf_FA_Wallet
   git stash -u
   git pull origin master
   git stash pop
   ```

### UAT Bind Point
| Path | Purpose |
|------|---------|
| `~/Documents/ksf_FA_Wallet` | Devel tree — all development, testing, commits |
| `~/ksf_Infrastructure/fa_modules/ksf_FA_Wallet` | UAT bind point — deployment target, integration testing (if mirrored) |

