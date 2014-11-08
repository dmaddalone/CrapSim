select 
    basics.sequence,
    table_come_out,
    beginning_bankroll,
    before_pass_bet_wager,
    before_pass_odds_wager,
    before_pass_point,
    before_bankroll,
    roll,
    after_pass_bet_wager,
    after_pass_odds_wager,
    after_bankroll
from
    basics
        inner join
    single_bets ON basics.sequence = single_bets.sequence;