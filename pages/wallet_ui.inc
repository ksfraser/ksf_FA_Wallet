<?php
/**
 * ksf_FA_Wallet - Admin UI Pages
 */

/**
 * Display wallet management page for admin
 */
function wallet_admin_page() {
    page(_($help_context = "Wallet Management"));
    
    // Handle actions
    if (isset($_POST['adjust_balance'])) {
        $user_id = get_post('user_id', 0);
        $amount = get_post('amount', 0);
        $type = get_post('type', 'credit');
        $details = get_post('details', '');
        
        $walletManager = init_wallet_manager();
        
        try {
            if ($type === 'credit') {
                $walletManager->credit($user_id, $amount, $details);
                display_notification(_('Balance credited successfully'));
            } else {
                $walletManager->debit($user_id, $amount, $details);
                display_notification(_('Balance debited successfully'));
            }
        } catch (Exception $e) {
            display_error($e->getMessage());
        }
    }
    
    if (isset($_POST['toggle_lock'])) {
        $user_id = get_post('user_id', 0);
        $walletManager = init_wallet_manager();
        
        if ($walletManager->isLocked($user_id)) {
            $walletManager->unlockWallet($user_id);
            display_notification(_('Wallet unlocked'));
        } else {
            $walletManager->lockWallet($user_id);
            display_notification(_('Wallet locked'));
        }
    }
    
    // Display user wallet table
    display_wallet_users_table();
    
    end_form();
}

/**
 * Display table of users with wallet balances
 */
function display_wallet_users_table() {
    $sql = "SELECT u.user_id, u.user_name, u.real_name, 
            COALESCE(w.balance, 0) as balance,
            COALESCE(w.is_locked, 0) as is_locked
            FROM " . TB_PREF . "users u
            LEFT JOIN fa_wallet_balances w ON u.user_id = w.user_id
            ORDER BY u.user_name";
    
    $result = db_query($sql, "Could not get users");
    
    start_table(TABLESTYLE);
    $th = [_('User ID'), _('Username'), _('Real Name'), _('Balance'), _('Status'), _('Actions')];
    table_header($th);
    
    $k = 0;
    while ($user = db_fetch($result)) {
        alt_table_row($k);
        
        label_cell($user['user_id']);
        label_cell($user['user_name']);
        label_cell($user['real_name']);
        amount_cell($user['balance'], 2);
        label_cell($user['is_locked'] ? _('Locked') : _('Active'));
        
        $actions = "";
        $actions .= "<a href='wallet_transactions.php?user_id=" . $user['user_id'] . "'>" . _('View Transactions') . "</a> | ";
        $actions .= "<a href='wallet_adjust.php?user_id=" . $user['user_id'] . "'>" . _('Adjust Balance') . "</a>";
        
        label_cell($actions);
        
        end_row();
    }
    
    end_table();
}

/**
 * Adjust user wallet balance page
 */
function wallet_adjust_page($user_id) {
    page(_($help_context = "Adjust Wallet Balance"));
    
    // Get user info
    $sql = "SELECT * FROM " . TB_PREF . "users WHERE user_id=" . (int)$user_id;
    $result = db_query($sql, "Could not get user");
    $user = db_fetch($result);
    
    if (!$user) {
        display_error(_('User not found'));
        end_page();
        return;
    }
    
    // Get current balance
    $walletManager = init_wallet_manager();
    $balance = $walletManager->getBalance($user_id);
    
    echo "<h3>" . sprintf(_('Adjust Wallet for %s'), $user['real_name']) . "</h3>";
    echo "<p><strong>" . _('Current Balance') . ":</strong> " . number_format($balance, 2) . "</p>";
    
    start_form();
    start_table(TABLESTYLE);
    
    hidden('user_id', $user_id);
    
    yesno_list_row(_('Type'), 'type', 0, _('Credit'), _('Debit'));
    amount_row(_('Amount'), 'amount', 0, null, null, 2);
    textarea_row(_('Details'), 'details', '', 50, 3);
    
    end_table();
    
    submit_center('adjust_balance', _('Adjust Balance'));
    end_form();
}

/**
 * View user wallet transactions
 */
function wallet_transactions_page($user_id) {
    page(_($help_context = "Wallet Transactions"));
    
    $sql = "SELECT * FROM fa_wallet_transactions 
            WHERE user_id=" . (int)$user_id . " AND deleted=0 
            ORDER BY date DESC 
            LIMIT 100";
    $result = db_query($sql, "Could not get transactions");
    
    start_table(TABLESTYLE);
    $th = [_('Date'), _('Type'), _('Amount'), _('Details'), _('Created By')];
    table_header($th);
    
    $k = 0;
    while ($trans = db_fetch($result)) {
        alt_table_row($k);
        
        label_cell($trans['date']);
        label_cell(ucfirst($trans['type']));
        if ($trans['type'] === 'credit') {
            amount_cell($trans['amount'], 2, 'class="amount-credit"');
        } else {
            amount_cell($trans['amount'], 2, 'class="amount-debit"');
        }
        label_cell($trans['details']);
        label_cell($trans['created_by']);
        
        end_row();
    }
    
    end_table();
    
    end_page();
}

/**
 * Initialize wallet manager with FA repository
 */
function init_wallet_manager() {
    require_once __DIR__ . '/../../ksf_Wallet_Core/src/Wallet/WalletManager.php';
    require_once __DIR__ . '/../../ksf_Wallet_Core/src/Wallet/Transaction.php';
    require_once __DIR__ . '/FA_WalletRepository.php';
    
    $repository = new FA_WalletRepository($GLOBALS['db']);
    return new \Ksf\Wallet\WalletManager($repository);
}
