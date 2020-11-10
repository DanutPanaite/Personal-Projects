#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <tuple>
#include <time.h>

using namespace std;

/*
    The League of Legends draft simple explanation:
In League of Legends 2 teams of 5 players are fighting each other(the team on the left is the blue team and the other the red team) while
controlling a champion. The game has 156 unique champions implemented, as of the writing of this text, and in a tournament setting a champion can
only exist in one team(Blind Pick allows for the same champion to be on different teams), champions are stronger or weaker based on the patch. The
tournament setting has multiple phases and I will briefly explain each one:
1. First phase(Ban phase) - the two teams take turns banning a champion starting with the blue team, until each team has banned 3 champions for a
total of 6, 6 champions that won't be pickable in the game.
2. Second phase(Pick phase) - now teams will start picking the champions they want to play in a orderly manner, blue team gets to pick one champion
first(will be referred to as B1), then the Red team will pick 2 champions(R1, R2), afterwards we go back to blue that also picks 2 champions(B2, B3)
next is Red that will pick only one champion(R3). Once both teams have picked 3 champions each the next phase begins.
3. Third phase(Ban phase) - each team will ban champions like in the First phase but this time the number is limited to 2 champions per team, this
is when a team would ban champions that directly counter their champions or their composition as a whole.
4. Fourth phase(Pick phase) - Red team will start picking first, being able to pick one champ(R4), then it goes to Blue that is allowed to pick 2
champs, then Red is allowed another pick, for a total of 5 champions in each team.

    Motivation:
Having watched professional games a handful of times, we can notice that the players don't have a good understanding of the strategy that goes into
draft usually picking what's the most powerful champion objectively speaking or comfort champions(things they play very well and love playing), and
the Coach is in the same boat as the players, he does not impose his point of view, therefore I decided to try making a bot that will objectively
draft a good composition every time, one that executed well can win games. Since all information is visible in the draft at every point it allows
for a bot to be able to adapt and draft properly with enough information.

    Introduction to the Draft Bot, the rules he will use:
This script is a Draft Bot(or AI) for the game League of Legends. It uses the information fed to it manually(through a text file or such) and does
drafting choices based on that. This is a completely biased Draft Bot that can only make drafts based on the information feeder's read and thoughts
on the meta and the champion match-ups. The script will use an easy to follow order of priority as showcased here:
1. Damage composition - the bot will, at all times, draft in such a way that you will never have full AP or full AD compositions. It will use a
MainDamageDealer(MDD) variable for every champ and it will choose to have at least one MDD from the minority side(be it AP or AD, usually AP), if it
can't accomplish that, the bot will try to include 2 champions that aren't MDDs, but that will have split damage output(50-50).
2. Color theme - based on Nick "LS" De Cesare's draft philosophy(inspired from the popular card game Magic the Gathering), champions will be
appointed colors(Red, Blue, Green, White, Black) and by having a simple defined color wheel(which color beats which), the bot will choose champs in
such a way that they counter the enemies colors. At the same time the order in which champions will be chosen is highly reliant on the same colors
guide. Champions with a high number of colors can be picked early as they facilitate what LS calls "pivoting" during draft, meaning that you can
adapt to the enemy picks and still have a Color theme(1 or 2 main colors). To further expand on this idea I will provide an example: picking a
champion that is only Red in nature early in the draft, will force the following picks to play as Red, even though they would be White, Green, with
Red only in a small proportion, unable to pivot in the second draft phase can be fatal to a draft's/team's success.
3. Champion theme - this is a concept that works very well paired with the previous one, each champion will have a way they want to play the game,
some can have multiple ones(usually White champions), the main idea is that we want to pick champions that fit with our theme, and we want our theme
to be countering the enemy's theme. Let's say the first 2 champions picked by the enemy have a Dive theme, which means they want to come to us and
kill us by being on top of us, now we must pivot to a composition that loves it when the enemy comes to them, something that likes to fight close to
each other, something that can kite back and thrives when the enemy comes into them.
4. Lane match-ups - as for our final rule to follow, this is the most basic one, but the one that most amateurs and even professional players put
a lot of value on, I fully believe that lane match-ups aren't as important as people make them seem. Lane match-ups translate into which champion
beats which in a vacuum, and will provide early pressure for the team having better lanes. Which doesn't necessarily translate into better chances
of winning, it's only a problem of adapting to the lanes.



*/
ifstream f("championData.txt");

string stringLower(string s){
    for(unsigned i = 0; i < s.length(); i++)
        s[i] = tolower(s[i]);
    return s;
}

int intLength(int n){
    int nr = 0;
    while(n != 0){
        nr++;
        n/=10;
    }
    return nr;
}

unsigned globalPosition = 1;
unsigned numberOfChamps = 0;
unsigned maxNameSize = 0;
unsigned numberOfCounters = 0;
unsigned noColorsNeeded = 3, noThemesNeeded = 3, noLanesNeeded = 2;
string roleOrder[5] = {"jungle", "adcarry", "support", "top", "mid"};
string compStructure[5];

void resetGlobalVariables(){
    string roleOrder[5] = {"jungle", "adcarry", "support", "top", "mid"};
}
struct counter{
    int championA, championB, level;
}counterStruct[500];

class champion{
    int pos;
    int damageMix;  //format 8020, 6535, etc, first is physical damage and second is magical damage
    string name;
    string colors;
    string theme;   //can choose from Dive, FB(front to back), Split
    string range;   //low(<500), medium (<600), high(>600)
    string lane;    //top, jungle, mid, ad carry, support
    bool MDD;
public:
    champion(int, string, string, string, string, string, bool);
    champion (champion&);
    virtual ~champion();

    virtual void citire (istream &in);
    virtual void afisare(ostream &out) const;
    champion& operator=(champion &p);
    friend istream& operator>>(istream&, champion&);
    friend ostream& operator<<(ostream&, champion&);

    int getPos(){return pos;}
    int getDamageMix(){return damageMix;}
    string getName(){return name;}
    string getColors(){return colors;}
    string getTheme(){return theme;}
    string getRange(){return range;}
    string getLane(){return lane;}
    bool getMDD(){return MDD;}

    void saveToFile(ofstream&);
    void readFile();

};
    champion::champion(int dmgMix = 9010, string nm = "", string clrs = "", string thm = "", string rng = "", string ln = "", bool bo = false){
        name = nm;
        colors = clrs;
        theme = thm;
        range = rng;
        lane = ln;
        MDD = bo;
        try
        {
            if(intLength(dmgMix) != 4)
                throw dmgMix;
        }
        catch(int x)
        {
            cout<<"Constructor error, damageMix format is wrong, use 4 digits.\n";
            exit(EXIT_FAILURE);
        }
        try
        {
            if(dmgMix%100 + dmgMix/100 != 100)
                throw dmgMix;
        }
        catch(int x)
        {
            cout<<"Constructor error, the damage percentages do not add up to 100.\n";
            exit(EXIT_FAILURE);
        }
        damageMix = dmgMix;
    }

    champion::champion (champion &p){
        pos = p.pos;
        damageMix = p.damageMix;
        name = p.name;
        colors = p.colors;
        theme = p.theme;
        range = p.range;
        lane = p.lane;
        MDD = p.MDD;
    }
    champion::~champion (){
        delete []this;
    }
    void champion::citire (istream &in){
        int maxColors = 5;
        pos = globalPosition;
        globalPosition++;
        string st;
        int inputVar;
        cout<<"Champion name:";
        in>>st;
        name=st;
        cout<<"Champion damage mix:";
        in>>inputVar;
        try
        {
            if(intLength(inputVar) != 4)
                throw inputVar;
        }
        catch(int x)
        {
            cout<<"Constructor error, damageMix format is wrong, use 4 digits.\n";
            exit(EXIT_FAILURE);
        }
        try
        {
            if(inputVar%100 + inputVar/100 != 100)
                throw inputVar;
        }
        catch(int x)
        {
            cout<<"Constructor error, the damage percentages do not add up to 100.\n";
            exit(EXIT_FAILURE);
        }
        damageMix = inputVar;
        cout<<"How many colors does the champion have: ";
        in>>inputVar;
        for(int i = 0; i < inputVar; i++)
        {
            cout<<"Input color "<<i+1<<" :";
            in>>st;
            if(stringLower(st) != "red" && stringLower(st) != "blue" && stringLower(st) != "green" && stringLower(st) != "black" && stringLower(st) != "white")
            {
                cout<<"Wrong color inputted, try Red, Blue, Green, Black or White. \n";
                i--;
            }
            colors += stringLower(st);
            colors += "/";
        }
        for (int i = maxColors - inputVar; i >0; i--)
        {
            colors += "none/";
        }
        cout<<"Champion theme:";
        in>>st;
        while(1){
            if(stringLower(st) != "dive" && stringLower(st) != "fb" && stringLower(st) != "split")
                cout<<"Wrong theme inputted, try Dive, FB or Split. \n";
            else{
                theme = st;
                break;
            }
            in>>st;
        }
        cout<<"Champion range:";
        in>>st;
        while(1){
            if(stringLower(st) != "low" && stringLower(st) != "medium" && stringLower(st) != "high")
                cout<<"Wrong range inputted, try Low, Medium, High. \n";
            else{
                range = st;
                break;
            }
            in>>st;
        }
        cout<<"How many lanes can the champion be played in: ";
        in>>inputVar;
        for(int i = 0; i < inputVar; i++)
        {
            cout<<"Input lane "<<i+1<<" :";
            in>>st;
            if(stringLower(st) != "top" && stringLower(st) != "jungle" && stringLower(st) != "mid" && stringLower(st) != "adcarry" && stringLower(st) != "support"){
                cout<<"Wrong lane inputted, try Top, Jungle, Mid, AD Carry or Support. \n";
                i--;
            }
            lane += stringLower(st);
            lane += "/";
        }
        for (int i = maxColors - inputVar; i >0; i--)
        {
            lane += "none/";
        }
        cout<<"Champion is MDD:";
        in>>st;
        while(1){
            if(stringLower(st) != "yes" && stringLower(st) != "no" )
                cout<<"Wrong answer, try Yes or No. \n";
            else{
                if(stringLower(st) == "yes")
                    MDD = true;
                else
                    MDD = false;
                break;
            }
            in>>st;
        }
    }

    void champion::afisare(ostream &out) const{
        out<<"Position: "<<pos<<"\n";
        out<<"Damage mix: "<<damageMix<<"\n";
        out<<"Name: "<<name<<"\n";
        out<<"Colors: "<<colors<<"\n";
        out<<"Theme: "<<theme<<"\n";
        out<<"Range: "<<range<<"\n";
        out<<"Lane: "<<lane<<"\n";
        out<<"MDD: "<<MDD<<"\n";
    }
    champion& champion::operator=(champion &p){
        if (this!=&p)
        {
            pos = p.pos;
            damageMix = p.damageMix;
            name = p.name;
            colors = p.colors;
            theme = p.theme;
            range = p.range;
            lane = p.lane;
            MDD = p.MDD;
        }
        return *this;
    }
    istream& operator>>(istream& in, champion& p){
        p.citire(in);
        return in;
    }

    ostream& operator<<(ostream& out, champion& p){
        p.afisare(out);
        return out;
    }
    void champion::saveToFile(ofstream &o){
        o<<pos<<"\n";
        o<<damageMix<<"\n";
        o<<name<<"\n";
        o<<colors<<"\n";
        o<<theme<<"\n";
        o<<range<<"\n";
        o<<lane<<"\n";
        o<<MDD<<"\n";
        o<<"-----------------------------------------------------------------------------\n";
    }
    void champion::readFile(){
        int p;
        string st;
        f>>p;
        pos = p;
        f>>p;
        damageMix = p;
        getline(f, st);
        getline(f, name);
        getline(f, colors);
        getline(f, theme);
        getline(f, range);
        getline(f, lane);
        f>>p;
        if(p == 1)
            MDD = true;
        else
            MDD = false;
    }

    champion *championVector; //global class vector for easier work with functions

void menu_output(){
    cout<<"\nBEHOLD! A DRAFT BOT!\n";
    cout<<"\n\t\tMENU:";
    cout<<"\n===========================================\n";
    cout<<"\n";
    cout<<"1. Input champion information manually ";
    cout<<"\n";
    cout<<"2. Get champion information from a file.";
    cout<<"\n";
    cout<<"3. Make a draft simulation.";
    cout<<"\n";
    cout<<"4. Save the champion information in a file.";
    cout<<"\n";
    cout<<"0. Quit.";
    cout<<"\n";
}

int getPosition(string name){
    for(unsigned i = 0; i < numberOfChamps; i++)
        if(stringLower(championVector[i].getName()) == stringLower(name)){
            return i;
        }
    return -1;
}

string getName(int pos){
    if(pos == -1)
        return "none";
    else
        return championVector[pos].getName();
}

int countFileLines(istream& f){
    int numberOfLines = 0;
    string line;
    while(getline(f, line)){
        numberOfLines++;
    }
    return numberOfLines;
}

void readCounters(istream& f, counter* counterStruct, int numberOfLines){
    string champion1, champion2;
    int pos1, pos2, counterLevel;
    getline(f, champion1); // getting rid of the Header
    for(int i = 0; i < numberOfLines; i++){
        f>>champion1;
        f>>champion2;
        f>>counterLevel;
        pos1 = getPosition(champion1);
        pos2 = getPosition(champion2);
        counterStruct[i].championA = pos1;
        counterStruct[i].championB = pos2;
        counterStruct[i].level = counterLevel;
    }
}
unsigned getLongestName(){
    unsigned max = 0;
    string st;
    for(unsigned i = 0; i < numberOfChamps; i++){
        st = championVector[i].getName();
        if(st.size() > max)
            max = st.size();
    }
    return max;
}
void draftLayout(int bluePicks[5], int redPicks[5], int blueBans[5], int redBans[5]){
    system("CLS");
    unsigned spaceBetween = 108;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------"<<endl;
    for(int i = 0; i < 5; i++){
        cout<<"|"<<getName(blueBans[i]);
        for(unsigned j = 0; j < maxNameSize - getName(blueBans[i]).size(); j++){
            cout<<" ";
        }
    }
    cout<<"|";
    cout<<"            BANS            ";
    for(int i = 4; i >= 0; i--){
        cout<<"|"<<getName(redBans[i]);
        for(unsigned j = 0; j < maxNameSize - getName(redBans[i]).size(); j++){
            cout<<" ";
        }

    }
    cout<<"|"<<endl;
    cout<<"----------------------------------------------------------------------------------------------------------------------------------";
    cout<<endl;
    for(unsigned i = 0; i < 5; i++){         //creating a box for every champion pick blue/red side
        //top row
        cout<<"|";
        for(unsigned j = 0; j < maxNameSize; j++){
            cout<<" ";
        }
        cout<<"|";
        for(unsigned j = 0; j < spaceBetween; j++){
            cout<<" ";
        }
        cout<<"|";
        for(unsigned j = 0; j < maxNameSize; j++){
            cout<<" ";
        }
        cout<<"|";
        cout<<endl;
        //middle row
        cout<<"|";
        cout<<getName(bluePicks[i]);
        for(unsigned j = 0; j < maxNameSize - getName(bluePicks[i]).size(); j++){
            cout<<" ";
        }
        cout<<"|";
        for(unsigned j = 0; j < spaceBetween; j++){
            cout<<" ";
        }
        cout<<"|";
        cout<<getName(redPicks[i]);
        for(unsigned j = 0; j < maxNameSize - getName(redPicks[i]).size(); j++){
            cout<<" ";
        }
        cout<<"|";
        cout<<endl;

        //bottom row
        cout<<"|";
        for(unsigned j = 0; j < maxNameSize; j++){
            cout<<" ";
        }
        cout<<"|";
        for(unsigned j = 0; j < spaceBetween; j++){
            cout<<" ";
        }
        cout<<"|";
        for(unsigned j = 0; j < maxNameSize; j++){
            cout<<" ";
        }
        cout<<"|";
        cout<<endl;
        cout<<"----------------------------------------------------------------------------------------------------------------------------------";
        cout<<endl;
    }
}


//for making it pretty pretty :D
void Color(int color){
 SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

bool cmp(counter a, counter b){
    return a.championA<b.championA;
}

unsigned numberOfOccurences(string st, string searched = "none"){
    unsigned occurences = 0;
    std::string::size_type pos = 0;
    while((pos = st.find(searched, pos)) != std::string::npos){
        occurences++;
        pos += searched.length();
    }
    return occurences;
}

void split(std::string str, std::string splitBy, std::vector<std::string>& tokens){
    /* Store the original string in the array, so we can loop the rest
     * of the algorithm. */
    tokens.push_back(str);

    // Store the split index in a 'size_t' (unsigned integer) type.
    size_t splitAt;
    // Store the size of what we're splicing out.
    size_t splitLen = splitBy.size();
    // Create a string for temporarily storing the fragment we're processing.
    std::string frag;
    // Loop infinitely - break is internal.
    while(true)
    {
        /* Store the last string in the vector, which is the only logical
         * candidate for processing. */
        frag = tokens.back();
        /* The index where the split is. */
        splitAt = frag.find(splitBy);
        // If we didn't find a new split point...
        if(splitAt == string::npos)
        {
            // Break the loop and (implicitly) return.
            break;
        }
        /* Put everything from the left side of the split where the string
         * being processed used to be. */
        tokens.back() = frag.substr(0, splitAt);
        /* Push everything from the right side of the split to the next empty
         * index in the vector. */
        tokens.push_back(frag.substr(splitAt+splitLen, frag.size()-(splitAt+splitLen)));
    }
}


void crossOutLane(int championPos, string lane = ""){
    if(lane != ""){
        for(int i = 0; i < 5; i++){
            if(roleOrder[i] == lane){
                roleOrder[i] = "none";
                return;
            }
        }
        return;
    }
    if(numberOfOccurences(championVector[championPos].getLane()) > 3){
        for(int i = 0; i < 5; i++){
            if(numberOfOccurences(championVector[championPos].getLane(), roleOrder[i]) >= 1){
                roleOrder[i] = "none";
                break;
            }
        }
    }
}

void checkLanes(int redPicks[5]){
    int champPos, noOfPicks = 0;
    vector<string> c1;
    vector<string> c2;
    for(int i = 0; i < 5; i++)
        if(redPicks[i] != -1)
            noOfPicks++;
    for(int i = 0; i < noOfPicks; i++){
        if(5 - numberOfOccurences(championVector[redPicks[i]].getLane()) == 2){  //champion is flexible -> search for a non flexibile champ with the same lane
            split(championVector[redPicks[i]].getLane(),"/", c1);
            for(int j = 0; j < noOfPicks; j++){
                if(numberOfOccurences(championVector[redPicks[j]].getLane(), c1[0]) == 1 && championVector[redPicks[j]].getName() != championVector[redPicks[i]].getName()){
                    if(5 - numberOfOccurences(championVector[redPicks[j]].getLane()) == 1){
                        crossOutLane(0, c1[0]);
                        cout<<championVector[redPicks[i]].getName()<<" "<<c1[1]<<endl;
                        crossOutLane(0, c1[1]); // second lane the champ can go to
                    }
                }
            c1.clear();
            }
        }
    }
}

bool suitableBasedOnR1(int R1, string name){
    int champPos = getPosition(name);
    if(name == championVector[R1].getName()){
        return false;
    }
    vector<string> theme;
    vector<string> color;;
    split(championVector[champPos].getColors(), "/", color);
    split(championVector[champPos].getTheme(), "/", theme);
    unsigned colorCount = 0, themeCount;
    if(color.size() > 2){
        for(unsigned i = 0; i < color.size()-1; i++){
            if(color[i] != "none"){
                if(numberOfOccurences(championVector[R1].getColors(), color[i]) > 0){
                    colorCount++;
                }

            }
        }
    }
    if(colorCount > 2){
        cout<<name<<" e satisfacator1"<<endl;
        return true;
    }
    for(unsigned i = 0; i < theme.size()-1; i++){
        if(numberOfOccurences(championVector[R1].getTheme(), theme[i]) == 1){
            themeCount++;
        }
    }
    if(colorCount > 1 && themeCount > 1){
        cout<<name<<" e satisfacator2"<<endl;
        return true;
    }
    return false;
}

void firstAPhaseRed(champion* firstPhaseChampionPool, int firstPhaseNoChamps, int bluePicks[5], int redPicks[5], int redBans[5], int blueBans[5]){
    string R1, R2;
    vector<tuple<int, int>> possiblePicks;
    draftLayout(bluePicks, redPicks, blueBans, redBans);
    //if we know that the enemy pick isn't a flex we take a direct counter from the pool and a very all around champion on R2
    unsigned bluelaneVersatility, selfLaneVersatility;
    bluelaneVersatility = 5 - numberOfOccurences(championVector[bluePicks[0]].getLane());
    if(bluelaneVersatility == 1){  //R1 picking
        for(unsigned i = 0; i < numberOfCounters; i++){
                if(counterStruct[i].championB == bluePicks[0]){
                    for(int j = 0; j < firstPhaseNoChamps; j++){
                        if(getName(counterStruct[i].championA) == firstPhaseChampionPool[j].getName() && counterStruct[i].level >= 70){;
                            possiblePicks.push_back(make_tuple(counterStruct[i].level, counterStruct[i].championA));
                        }
                    }
                }
        }
        if(!possiblePicks.empty()){
            sort(possiblePicks.begin(), possiblePicks.end());
            R1 = getName(get<1>(possiblePicks[possiblePicks.size()-1]));
            redPicks[0] = getPosition(R1);
            crossOutLane(bluePicks[0]);
        }
    }
    if(possiblePicks.empty()){
            if(stringLower(championVector[bluePicks[0]].getRange()) == "low" && championVector[bluePicks[0]].getMDD() == true){
                //then pick a high range that doesn't get countered or pick a medium range guy that counters their pick
                string lane;
                for(int i = 0; i < 5; i++){
                    if(roleOrder[i] != "none"){
                        lane = roleOrder[i];
                        break;

                    }
                }
                for(int j = 0; j < firstPhaseNoChamps; j++){
                    if(firstPhaseChampionPool[j].getRange() == "high" && numberOfOccurences(firstPhaseChampionPool[j].getLane(), lane) == 1){;
                        cout<<firstPhaseChampionPool[j].getName()<<endl;
                        possiblePicks.push_back(make_tuple(0, getPosition(firstPhaseChampionPool[j].getName())));
                    }
                }
                if(!possiblePicks.empty()){
                    sort(possiblePicks.begin(), possiblePicks.end());
                    unsigned random = rand() % possiblePicks.size();
                    R1 = getName(get<1>(possiblePicks[random]));
                    redPicks[0] = getPosition(R1);
                    selfLaneVersatility = 5 - numberOfOccurences(championVector[redPicks[0]].getLane());
                    if(selfLaneVersatility == 1){
                        crossOutLane(redPicks[0]);
                    }
                }

            }
            else{
                for(int i = 0; i < numberOfCounters; i++){
                    if(counterStruct[i].championB == bluePicks[0]){
                        for(int j = 0; j < firstPhaseNoChamps; j++){
                            if(getName(counterStruct[i].championA) == firstPhaseChampionPool[j].getName() && firstPhaseChampionPool[j].getRange() == "medium"){;
                                possiblePicks.push_back(make_tuple(counterStruct[i].level, counterStruct[i].championA));
                            }
                        }
                    }
                }
                if(!possiblePicks.empty()){
                    sort(possiblePicks.begin(), possiblePicks.end());
                    R1 = getName(get<1>(possiblePicks[possiblePicks.size()-1]));
                    cout<<R1;
                    redPicks[0] = getPosition(R1);
                    selfLaneVersatility = 5 - numberOfOccurences(championVector[redPicks[0]].getLane());
                    if(selfLaneVersatility == 1){
                        crossOutLane(redPicks[0]);
                    }
                }
            }
        }
        if(possiblePicks.empty()) {
            if(bluelaneVersatility == 1){
                for(unsigned i = 0; i < numberOfCounters; i++){
                        if(counterStruct[i].championB == bluePicks[0]){
                            for(int j = 0; j < firstPhaseNoChamps; j++){
                                if(getName(counterStruct[i].championA) == firstPhaseChampionPool[j].getName()){;
                                    possiblePicks.push_back(make_tuple(counterStruct[i].level, counterStruct[i].championA));
                                }
                            }
                        }
                    }
                    if(!possiblePicks.empty()){
                        sort(possiblePicks.begin(), possiblePicks.end());
                        R1 = getName(get<1>(possiblePicks[possiblePicks.size()-1]));
                        redPicks[0] = getPosition(R1);
                        crossOutLane(bluePicks[0]);
                    }
            }
        }
        if(possiblePicks.empty()){   //choose something flexible in order of importance(changes with each patch)
            string lane;
            lane = roleOrder[0];
            for(int i = 0; i < firstPhaseNoChamps; i++){
                if(numberOfOccurences(firstPhaseChampionPool[i].getLane(), lane) >=1 ){
                    possiblePicks.push_back(make_tuple(0, getPosition(firstPhaseChampionPool[i].getName())));
                }
            }
            if(!possiblePicks.empty()){
                unsigned random = rand() % possiblePicks.size();
                R1 = getName(get<1>(possiblePicks[random]));
                redPicks[0] = getPosition(R1);
                selfLaneVersatility = 5 - numberOfOccurences(championVector[redPicks[0]].getLane());
                if(selfLaneVersatility == 1){
                    crossOutLane(redPicks[0]);
                }
            }
        }
    possiblePicks.clear();
    draftLayout(bluePicks, redPicks, blueBans, redBans);
    //R2 picking
    string role;
    for(int i = 0; i < 5; i++){
        if(roleOrder[i] != "none"){
            role = roleOrder[i];
            break;
        }
    }
    for(int i = 0; i < firstPhaseNoChamps; i++){
        if(numberOfOccurences(firstPhaseChampionPool[i].getLane(), role) >=1 ){
            string champName = firstPhaseChampionPool[i].getName();
            cout<<champName<<endl;
            bool suitable = suitableBasedOnR1(redPicks[0], champName);
            if(suitable && firstPhaseChampionPool[i].getName() != championVector[redPicks[0]].getName()){
                cout<<champName<<" is suitable"<<endl;
                possiblePicks.push_back(make_tuple(0, getPosition(firstPhaseChampionPool[i].getName())));
            }
        }
    }
    if(!possiblePicks.empty()){
        unsigned random = 0;
        R2 = getName(get<1>(possiblePicks[random]));
        redPicks[1] = getPosition(R2);
        selfLaneVersatility = 5 - numberOfOccurences(championVector[redPicks[0]].getLane());
        if(selfLaneVersatility == 1){
            crossOutLane(redPicks[0]);
        }
        else if(selfLaneVersatility == 2){
            //check the other picks and see if any of them have the same lane, if so then we cross out the other lane
            vector<string> r1Lane;
            vector<string> r2Lane;
            split(championVector[redPicks[0]].getLane(), "/", r1Lane);
            split(championVector[redPicks[1]].getLane(), "/", r2Lane);
            for(int i = 0; i < r2Lane.size()-1; i++){
                if(r2Lane[i] != "none")
                    for(int j = 0; r1Lane.size()-1; j++){
                        if(r2Lane[i] == r1Lane[j] && r1Lane[j] != "none")
                            crossOutLane(redPicks[1], r2Lane[i]);
                            break;
                    }
            }
            for(int i = 0; i < 5; i++)
                cout<<roleOrder[i]<<" ";
        }
    }

}

unsigned colorToNumber(string color){
    if(color == "blue")
        return 0;
    if(color == "white")
        return 1;
    if(color == "green")
        return 2;
    if(color == "black")
        return 3;
    if(color == "red")
        return 4;
    return -1;
}

unsigned themeToNumber(string theme){
    if(theme == "fb")
        return 0;
    if(theme == "dive")
        return 1;
    if(theme == "split")
        return 2;
    return -1;
}

void vectorSorting(unsigned vectorLength, unsigned vect[5]){
    unsigned aux;
    for(unsigned i = 0; i < vectorLength - 1; i++){
        if(vect[i+1] > vect[i]){
            aux = vect[i];
            vect[i] = vect[i+1];
            vect[i+1] = aux;
        }
    }
}

void compAnalysis(int bluePicks[5]){
    vector<string> c1;
    unsigned noOfPicks = 0, colorFrequency[5] = {0}, themeFrequency[3] = {0};
    for(unsigned i = 0; i < 5; i++){
        if(bluePicks[i] != -1)
            noOfPicks++;
    }
    for(unsigned i = 0; i < noOfPicks; i++){
        split(championVector[bluePicks[i]].getColors(), "/", c1);
        for(unsigned j = 0; j < noOfPicks; j++){  //go through champions
            if(championVector[bluePicks[j]].getName() != championVector[bluePicks[i]].getName()){
                for(unsigned k = 0; k < c1.size()-1; k++){
                    if(c1[k] != "none"){
                        if(numberOfOccurences(championVector[bluePicks[j]].getColors(), c1[k]) > 0){
                             colorFrequency[colorToNumber(c1[k])]++;
                        }
                    }
                }
            }
        }
        c1.clear();
        split(championVector[bluePicks[i]].getTheme(), "/", c1);
        for(unsigned j = 0; j < noOfPicks; j++){  //go through champions
            if(championVector[bluePicks[j]].getName() != championVector[bluePicks[i]].getName()){
                for(unsigned k = 0; k < c1.size()-1; k++){
                    if(c1[k] != "none"){
                        if(numberOfOccurences(championVector[bluePicks[j]].getTheme(), c1[k]) > 0){
                             themeFrequency[themeToNumber(c1[k])]++;
                        }
                    }
                }
            }
        }
    }
    //vectorSorting(5, colorFrequency);
    //vectorSorting(3, themeFrequency);
    for(int k = 0; k < 5; k++)
        colorFrequency[k]/= 2;
    for(int k = 0; k < 3; k++)
        themeFrequency[k]/= 2;
    for(int i = 0; i < 5; i++)
        cout<<colorFrequency[i]<<" ";
    cout<<endl;
    for(int i = 0; i < 3; i++)
        cout<<themeFrequency[i]<<" ";
    cout<<endl;

}

void firstBPhaseRed(champion* firstPhaseChampionPool, int firstPhaseNoChamps, int bluePicks[5], int redPicks[5], int redBans[5], int blueBans[5]){
    //creating an updated champion pool
    champion* firstPhaseBChampionPool;
    int firstPhaseBNoChamps = 0;
    int colorVersatility, themeVersatility, laneVersatility;

    for(int i = 0; i < firstPhaseNoChamps; i++){
        colorVersatility = 5 - numberOfOccurences(firstPhaseChampionPool[i].getColors());
        themeVersatility = 3 - numberOfOccurences(firstPhaseChampionPool[i].getTheme());
        laneVersatility = 5 - numberOfOccurences(firstPhaseChampionPool[i].getLane());
        bool alreadyPicked = false;
        if(colorVersatility >= noColorsNeeded || themeVersatility >= noThemesNeeded || laneVersatility >= noLanesNeeded){
            for(int j = 0; j < 5; j++){
                if(bluePicks[j] == getPosition(firstPhaseChampionPool[i].getName()) || redPicks[j] == getPosition(firstPhaseChampionPool[i].getName()) || blueBans[j] == getPosition(firstPhaseChampionPool[i].getName()) || redBans[j] == getPosition(firstPhaseChampionPool[i].getName())){
                        alreadyPicked = true;
                        break;
                }
            }
            if(!alreadyPicked){
                firstPhaseBNoChamps++;
            }
        }
    }
    firstPhaseBChampionPool = new champion[firstPhaseBNoChamps];
    for(int i = 0, count = 0; i < firstPhaseNoChamps; i++){
        colorVersatility = 5 - numberOfOccurences(firstPhaseChampionPool[i].getColors());
        themeVersatility = 3 - numberOfOccurences(firstPhaseChampionPool[i].getTheme());
        laneVersatility = 5 - numberOfOccurences(firstPhaseChampionPool[i].getLane());
        bool alreadyPicked = false;
        if((colorVersatility >= noColorsNeeded || themeVersatility >= noThemesNeeded || laneVersatility >= noLanesNeeded)){
            for(int j = 0; j < 5; j++){
                if(bluePicks[j] == getPosition(firstPhaseChampionPool[i].getName()) || redPicks[j] == getPosition(firstPhaseChampionPool[i].getName()) || blueBans[j] == getPosition(firstPhaseChampionPool[i].getName()) || redBans[j] == getPosition(firstPhaseChampionPool[i].getName())){
                        alreadyPicked = true;
                        break;
                }
            }
            if(!alreadyPicked){
                firstPhaseBChampionPool[count] = firstPhaseChampionPool[i];
                count++;
            }
        }
    }
    compAnalysis(bluePicks);
}

void draftSimulationRed(){
    champion* intermediatePool;
    string championName;
    unsigned firstPhaseNoChamps = 0, intermediateNumberOfChamps = 0, colorVersatility = 0, themeVersatility = 0, laneVersatility = 0;
    int bluePicks[5], redPicks[5], redBans[5], blueBans[5];
    for(int i = 0 ; i < 5; i ++ ){
        bluePicks[i] = -1;
        redPicks[i] = -1;
        blueBans[i] = -1;
        redBans[i] = -1;
    }
    //first ban phase
    /*
    for(int i = 0; i < 6; i++){
        if(i % 2 == 0){
            //cout<<"Input B"<<i/2+1<<" champion: ";
            cout<<"Input blue side ban number "<<i/2+1<<": ";
            cin>>championName;
            champPos = getPosition(championName);
            blueBans[i/2] = champPos;
        }
        else{
            cout<<"Input red side ban number "<<i/2+1<<": ";
            cin>>championName;
            champPos = getPosition(championName);
            redBans[i/2] = champPos;
        }
    }
    intermediateNumberOfChamps = numberOfChamps - 6;
    cout<<intermediateNumberOfChamps;
    intermediatePool = new champion[intermediateNumberOfChamps];
    for(int i = 0, falseN = 0; unsigned(i) < numberOfChamps; i++){
        bool banned = false;

        for(int j = 0; j < 5; j++){
            if(redBans[j] == i || blueBans[j] == i){
                banned = true;
            }
        }
        if(banned == true){
            ;
        }
        else{
            intermediatePool[falseN] = championVector[i];
            falseN++;
        }

    }
    */
    draftLayout(bluePicks, redPicks, blueBans, redBans);
    //create champion pool for the first three picks
    intermediatePool = championVector;
    intermediateNumberOfChamps = numberOfChamps;

    champion *firstPhaseChampionPool;
    for(int i = 0; i < intermediateNumberOfChamps; i++){
        colorVersatility = 5 - numberOfOccurences(intermediatePool[i].getColors());
        themeVersatility = 3 - numberOfOccurences(intermediatePool[i].getTheme());
        laneVersatility = 5 - numberOfOccurences(intermediatePool[i].getLane());
        if(colorVersatility >= noColorsNeeded || themeVersatility >= noThemesNeeded || laneVersatility >= noLanesNeeded){
            firstPhaseNoChamps++;
        }
    }
    firstPhaseChampionPool = new champion[firstPhaseNoChamps];
    for(int i = 0, j = 0; i < intermediateNumberOfChamps; i++){
        colorVersatility = 5 - numberOfOccurences(intermediatePool[i].getColors());
        themeVersatility = 3 - numberOfOccurences(intermediatePool[i].getTheme());
        laneVersatility = 5 - numberOfOccurences(intermediatePool[i].getLane());
        if(colorVersatility >= noColorsNeeded || themeVersatility >= noThemesNeeded || laneVersatility >= noLanesNeeded){
            firstPhaseChampionPool[j] = intermediatePool[i];
            j++;
        }
    }

    //blue side first phase
    for(int i = 0; i < 2; i++){
        if(i == 0){
            cout<<"Input B"<<i+1<<" champion: ";
            cin>>championName;
            //verify if banned or already picked
            bluePicks[i] = getPosition(championName);
            firstAPhaseRed(firstPhaseChampionPool, firstPhaseNoChamps, bluePicks, redPicks, redBans, blueBans);
            draftLayout(bluePicks, redPicks, blueBans, redBans);
        }
        else{
            cout<<"Input B"<<i+1<<" champion: ";
            cin>>championName;
            //verify if banned or already picked
            bluePicks[i] = getPosition(championName);
            cout<<"Input B"<<i+2<<" champion: ";
            cin>>championName;
            //verify if banned or already picked
            bluePicks[i+1] = getPosition(championName);
            firstBPhaseRed(firstPhaseChampionPool, firstPhaseNoChamps, bluePicks, redPicks, redBans, blueBans);
            //draftLayout(bluePicks, redPicks, blueBans, redBans);
        }
    }
    //draftLayout(bluePicks, redPicks, blueBans, redBans);

}

void draftSimulation(){
    //int bluePicks[5], redPicks[5];
    //string side;

    //choosing for what side the bot drafts red/blue
    /*
    while(1){
        cout<<"Choose on which side(red/blue) the bot is on: ";
        cin>>side
        if(stringLower(side) != "red" && stringLower(side) != "blue"){
            cout<<"Wrong choice, try red or blue."<<endl;
        }
        else{
            break;
        }
    }
    */

}

int main()
{
    int option;///optiunea aleasa din meniu
    int champFileLength = 9;
    option=0;
    srand (time(NULL));
    numberOfChamps = countFileLines(f)/9;
    do
    {
        menu_output();
        cout<<"\nInput the wanted action: ";
        cin>>option;
        if (option==1)
        {
            int secondaryOption = 0;
            cout<<"Input the number of champions inserted into the database: ";
            cin>>numberOfChamps;
            championVector=new champion[numberOfChamps];
            for(unsigned i = 0; i < numberOfChamps; i++)
                cin>>championVector[i];
            if (numberOfChamps>0)
            {
                cout<<"\n Printing the inputted champions:\n";
                for(unsigned i=0; i < numberOfChamps; i++)
                   {
                        cout<<"\n"<<championVector[i];
                        cout<<"--------------------------\n";
                   }
            }
            else
                cout<<"The number of champions can not be negative.\n";
            cout<<"Do you want to save the champions inputted to a text file?\n 1 - Yes \n 0 - No \n";
            cin>>secondaryOption;
            if(secondaryOption){
                ofstream outputFile("championData.txt");
                outputFile<<"--------------------------------Champion Data--------------------------------\n";
                for(unsigned i = 0; i < numberOfChamps; i++){
                    championVector[i].saveToFile(outputFile);
                }
                outputFile.close();
            }
            ifstream counterText("counters.txt");
            numberOfCounters = countFileLines(counterText)-1;
            counterText.close();
            counterText.open("counters.txt");
            readCounters(counterText, counterStruct, numberOfCounters);
            sort(counterStruct,counterStruct+numberOfCounters,cmp);
            maxNameSize = getLongestName();

        }
        if (option==2)
        {
            string line;
            championVector=new champion[numberOfChamps];
            for(unsigned i = 0; i < numberOfChamps; i++){
                int noLinesSkip = i*champFileLength;
                f.close();
                f.open("championData.txt");
                getline(f,line);
                for(int j = 0; j < noLinesSkip; j++)
                    getline(f, line);
                championVector[i].readFile();
            }
            cout<<endl;

            ifstream counterText("counters.txt");
            numberOfCounters = countFileLines(counterText)-1;
            counterText.close();
            counterText.open("counters.txt");
            readCounters(counterText, counterStruct, numberOfCounters);
            sort(counterStruct,counterStruct+numberOfCounters,cmp);
            maxNameSize = getLongestName();
        }
        if (option==3)
        {
            string draftSide;

            while(1){
                cout<<"Choose on which side(red/blue) the bot is on: ";
                cin>>draftSide;
                if(stringLower(draftSide) != "red" && stringLower(draftSide) != "blue"){
                    cout<<"Wrong choice, try red or blue."<<endl;
                }
                else{
                    break;
                }

            }
            if(stringLower(draftSide) == "red"){
                    draftSimulationRed();
            }
        }
        if (option==4)
        {
            int redPicks[5] = {4, 2, 1, -1, -1};
            checkLanes(redPicks);
        }
        if (option==0)
        {
            cout<<"\nEXIT\n\n";
        }
        if (option<0||option>4)
        {
            cout<<"\nInvalid action\n";
        }
        cout<<"\n";
        resetGlobalVariables();
        system("pause"); ///Pauza - Press any key to continue...
        system("cls");   ///Sterge continutul curent al consolei

    }while(option!=0);
    return 0;
}
