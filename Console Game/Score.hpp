#pragma once
class Score{
public:

	int kill_score = 0;//done 1k:10s
	int xp_score = 0;//done 1x:1s
	int book_score = 0;//done 1b:5s
	int level_score = 0; //done +=current level*100
	int ach_score = 0;
	//TODO: achevements	int ach_score = 0;
	//TODO: Score shit; exporting scores
	int total_score();

	Score();
	virtual ~Score();
};

int Score::total_score()
{
	return kill_score+xp_score+book_score+level_score+ach_score;
}

Score::Score(){
}

Score::~Score(){
}