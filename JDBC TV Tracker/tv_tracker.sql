drop database if exists tv_tracker;
create database tv_tracker;
use tv_tracker;

CREATE TABLE tv_users (
    user_id INT PRIMARY KEY 
				AUTO_INCREMENT,
    tv_username VARCHAR(255) NOT NULL,
    tv_nickname VARCHAR(255) NOT NULL,
    tv_password VARCHAR(255) NOT NULL
   
);

insert into tv_users(tv_username, tv_nickname, tv_password) 
	values('bboggioni', 'Breezy', "CogniJump23");
insert into tv_users(tv_username, tv_nickname, tv_password) 
	values('lboza', 'Lance', "CogniJump23");
insert into tv_users(tv_username, tv_nickname, tv_password) 
	values('jrivera', 'Jose', "CogniJump23");

CREATE TABLE tv_shows (
    show_id INT PRIMARY KEY 
				AUTO_INCREMENT,
    tv_showname VARCHAR(255) NOT NULL,
    tv_genre VARCHAR(255) NOT NULL   
);

#Insert Action Shows

insert into tv_shows(tv_showname, tv_genre)
	values('Altered Carbon', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('Cobra Kai', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('Narcos', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('The Night Agent', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('The Recruit', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('Daredevil', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('The Last Kingdom', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('Daybreak', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('Shadow and Bone', 'Action');
insert into tv_shows(tv_showname, tv_genre)
	values('The Witcher', 'Action');
    
#Insert Comedy Shows
insert into tv_shows(tv_showname, tv_genre)
	values('Only Murders in the Building', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Shameless', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('The Bear', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Frasier', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('The Office', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Ted Lasso', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Wednesday', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Gilmore Girls', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('Friends', 'Comedy');
insert into tv_shows(tv_showname, tv_genre)
	values('What We Do in the Shadows', 'Comedy');
    
#Insert Fantasy Shows

insert into tv_shows(tv_showname, tv_genre)
	values('Wolf Pack', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('Percy Jackson and the Olympians', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('Anne Rice\'s Mayfair Witches', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('House of the Dragon', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('The Sandman', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('Wolf Pack', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('Vikings:Valhalla', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('The Watch', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('The Dark Crystal: Age of Resistance', 'Fantasy');
insert into tv_shows(tv_showname, tv_genre)
	values('His Dark Materials', 'Fantasy');
    

CREATE TABLE user_shows (
    show_id INT,
    user_id INT,
    show_status INT,
    primary key (show_id, user_id)   
);
       
    
#Insert User_show records
insert into user_shows(show_id, user_id, show_status)
	values(	1, 2, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	2, 2, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	15, 2, 3);
insert into user_shows(show_id, user_id, show_status)
	values( 16, 2, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	27, 2, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	28, 2, 3);
insert into user_shows(show_id, user_id, show_status)
	values(	21, 1, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	22, 1, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	7, 1, 3);
insert into user_shows(show_id, user_id, show_status)
	values(	9, 1, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	15, 1, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	16, 1, 3);
insert into user_shows(show_id, user_id, show_status)
	values(	4, 3, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	10, 3, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	12, 3, 3);
insert into user_shows(show_id, user_id, show_status)
	values( 13, 3, 1);
insert into user_shows(show_id, user_id, show_status)
	values(	24, 3, 2);
insert into user_shows(show_id, user_id, show_status)
	values(	25, 3, 3);

    