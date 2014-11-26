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
    CrapSim.basics
        inner join
    CrapSim.single_bets ON CrapSim.basics.sequence = CrapSim.single_bets.sequence;