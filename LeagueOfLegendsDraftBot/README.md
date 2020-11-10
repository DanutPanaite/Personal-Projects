<h1> League of Legends Draft Bot ~WIP </h1>
 
   <h3>The League of Legends draft simple explanation:</h3><br>
In League of Legends 2 teams of 5 players are fighting each other(the team on the left is the blue team and the other the red team) while
controlling champs similar to Chess. The game has 156 unique champions implemented, as of the writing of this text, and in a tournament setting a champion can
only exist in one team(Blind Pick allows for the same champion to be on different teams), champions are stronger or weaker based on the patch. The
tournament setting has multiple phases and I will briefly explain each one:<br>

1. First phase(Ban phase) - the two teams take turns banning a champion starting with the blue team, until each team has banned 3 champions for a
total of 6, 6 champions that won't be pickable in the game. <br>

2. Second phase(Pick phase) - now teams will start picking the champions they want to play in a orderly manner, blue team gets to pick one champion
first(will be referred to as B1), then the Red team will pick 2 champions(R1, R2), afterwards we go back to blue that also picks 2 champions(B2, B3)
next is Red that will pick only one champion(R3). Once both teams have picked 3 champions each the next phase begins.<br>

3. Third phase(Ban phase) - each team will ban champions like in the First phase but this time the number is limited to 2 champions per team, this
is when a team would ban champions that directly counter their champions or their composition as a whole.<br>

4. Fourth phase(Pick phase) - Red team will start picking first, being able to pick one champ(R4), then it goes to Blue that is allowed to pick 2
champs, then Red is allowed another pick, for a total of 5 champions in each team. <br>

   <h3>Motivation:</h3><br>
Having watched professional games a handful of times, we can notice that the players don't have a good understanding of the strategy that goes into
draft, they usually pick what's the most powerful champion objectively speaking or comfort champions(things they play very well and love playing), and
usually the Coach is in the same as the players, he does not impose his point of view enough, therefore I decided to try making a bot that will objectively
draft a good composition every time. Since all information is visible in the draft at every point it allowsfor a bot to be able to adapt and draft properly 
with enough information.

   <h3>Introduction to the Draft Bot, the rules he will use:</h3><br>
This script is a Draft Bot(or AI) for the game League of Legends. It uses the information fed to it manually(through a text file or such) and does
drafting choices based on that. This is a completely biased Draft Bot that can only make drafts based on the information feeder's read and thoughts
on the meta and the champion match-ups. The script will use an easy to follow order of priority as showcased here:<br>
1. Damage composition - the bot will, at all times, draft in such a way that you will never have full AP or full AD compositions. It will use a
MainDamageDealer(MDD) variable for every champ and it will choose to have at least one MDD from the minority side(be it AP or AD, usually AP), if it
can't accomplish that, the bot will try to include 2 champions that aren't MDDs, but that will have split damage output(50-50).<br>
2. Color theme - based on Nick "LS" De Cesare's draft philosophy(inspired from the popular card game Magic the Gathering), champions will be
appointed colors(Red, Blue, Green, White, Black) and by having a simple defined color wheel(which color beats which), the bot will choose champs in
such a way that they counter the enemies colors. At the same time the order in which champions will be chosen is highly reliant on the same colors
guide. Champions with a high number of colors can be picked early as they facilitate what LS calls "pivoting" during draft, meaning that you can
adapt to the enemy picks and still have a Color theme(1 or 2 main colors). To further expand on this idea I will provide an example: picking a
champion that is only Red in nature early in the draft, will force the following picks to play as Red, even though they would be White, Green, with
Red only in a small proportion, unable to pivot in the second draft phase can be fatal to a draft's/team's success.<br>
3. Champion theme - this is a concept that works very well paired with the previous one, each champion will have a way they want to play the game,
some can have multiple ones(usually White champions), the main idea is that we want to pick champions that fit with our theme, and we want our theme
to be countering the enemy's theme. Let's say the first 2 champions picked by the enemy have a Dive theme, which means they want to come to us and
kill us by being on top of us, now we must pivot to a composition that loves it when the enemy comes to them, something that likes to fight close to
each other, something that can kite back and thrives when the enemy comes into them.<br>
4. Lane match-ups - as for our final rule to follow, this is the most basic one, but the one that most amateurs and even professional players put
a lot of value on, I fully believe that lane match-ups aren't as important as people make them seem. Lane match-ups translate into which champion
beats which in a vacuum, and will provide early pressure for the team having better lanes. Which doesn't necessarily translate into better chances
of winning, it's only a problem of adapting to the lanes.<br>
