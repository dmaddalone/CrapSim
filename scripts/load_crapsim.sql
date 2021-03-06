/*
    Create database if it does not exist.
*/
system echo '\nCreating database'
CREATE DATABASE IF NOT EXISTS CrapSim; 

/*
    Create basic table.
*/
system echo '\nCreating basics table'
DROP TABLE IF EXISTS CrapSim.basics;
CREATE TABLE IF NOT EXISTS CrapSim.basics
    (sequence INT, 
     odds INT,
     beginning_bankroll INT,
     table_come_out BOOL,
     table_point INT,
     before_bankroll INT,
     roll INT,
     after_bankroll INT);

/*
    Create single_bets table.
*/
system echo '\nCreating single_bets table'
DROP TABLE IF EXISTS CrapSim.single_bets;
CREATE TABLE IF NOT EXISTS CrapSim.single_bets
    (sequence INT, 
     before_pass_bet_wager INT,
     before_pass_odds_wager INT,
     before_pass_point INT,
     before_dont_pass_bet_wager INT,
     before_dont_pass_odds_wager INT,
     before_dont_pass_point INT,
     before_field_bet_wager INT,

     after_pass_bet_wager INT,
     after_pass_odds_wager INT,
     after_pass_point INT,
     after_dont_pass_bet_wager INT,
     after_dont_pass_odds_wager INT,
     after_dont_pass_point INT,
     after_field_bet_wager INT);

/*
    Truncate tables.
*/
system echo '\nTruncating basics table'
TRUNCATE TABLE CrapSim.basics;

system echo '\nTruncating single_bets table'
TRUNCATE TABLE CrapSim.single_bets;

/* 
    Load tables.
*/
system echo '\nLoading basics table'
LOAD DATA LOCAL INFILE '/home/maddalone/CCPP/CrapSim/CrapSimBasics.csv'
    INTO TABLE CrapSim.basics
    FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
    LINES TERMINATED BY '\n';

system echo '\nLoading single_bets table'
LOAD DATA LOCAL INFILE '/home/maddalone/CCPP/CrapSim/CrapSimSingleBets.csv'
    INTO TABLE CrapSim.single_bets
    FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
    LINES TERMINATED BY '\n';
