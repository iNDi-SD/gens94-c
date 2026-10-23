# The routines

Every function in `game/`, by folder and file: the C name, the original routine it is (the
original assembly module and label, as the community's byte-matching rebuild names them), and what it does.
Generated from the code's own comments by `tools/routines.py`. A helper is a piece split out of an
original routine (its comment says which) or a small function of the port's own (the 68000's
arithmetic, its jumps through RAM).

875 functions: 759 original routines, 116 helpers.

## Play: the game on the ice (skating, the AI, checks, the puck, goalies, penalties, periods)

### `game/play/assign.c` (18)

| Function | Original | What it does |
|---|---|---|
| `assignment_prologue` | (a helper) | the common prologue of the positional assignments: nothing while locked in an animation, donothing with the clock stopped, the bench check, nothing for a human; a new assignment resets the timers. |
| `aidef_timer` | (a helper) | the timer reload from aidef, one less while the crowd record is broken; 1 when it ran out |
| `check4bench` | `assign94.asm:check4bench` | Check if player a3 should go to the bench: with a line change pending (newpnum) and the puck not on him, switch him to assbench; when the new player is himself, just take his new position. |
| `take_new_position` | (a helper) | the new player takes over the object: position from newpos, his first assignment |
| `assbench` | `assign94.asm:assbench` | Player a3 should go to the bench: skate to the gate, glide through it (SPAwallleft), then the new player comes on (setplayer). |
| `asseben` | `assign94.asm:asseben` | Player a3 should exit the bench area: hop the boards at his slot. |
| `asspenalty` | `assign94.asm:asspenalty` | Player a3 should go to the penalty box: skate to the box on his team's side (stacked by how many are in it), turn to the boards and hop over. |
| `assdopen` | `assign94.asm:assdopen` | Add player a3 to the penalty box. |
| `assepen` | `assign94.asm:assepen` | Player a3 should exit the penalty area: hop back onto the ice. |
| `assgoaliebreakwait` | `assign94.asm:assgoaliebreakwait` | In a shootout or penalty shot park the idle goalie off the ice, else exit. |
| `assfaceoff` | `assign94.asm:assfaceoff` | Players do nothing until the faceoff is over. |
| `assfaceoffpl` | `assign94.asm:assfaceoffpl` | The two players taking the faceoff: his stick position (from the animation frame and his hand) goes to fofdata2 for the drop; a computer player starts swinging, sometimes early. |
| `donothing` | `assign94.asm:donothing` | The player should do nothing: coast. |
| `assstanley` | `assign94.asm:assstanley` | The player skates with the Stanley Cup overhead to the middle of the screen. |
| `assscore` | `assign94.asm:assscore` | Player a3 celebrates a goal: skate toward the middle, then celebrate (the scorer pumps his arm) at a random moment. |
| `assdefo` | `assign94.asm:assdefo` | Player a3 is a defensive player on offense: hold the point just inside the blue line on his side, following the puck across. |
| `assdefd` | `assign94.asm:assdefd` | Player a3 is a defensive player on defense: stay between the attackers and his net, on the puck's side. |
| `asswingd` | `assign94.asm:asswingd` | Player a3 is a winger on defense: cover the far side, level with the puck in the defensive zone. |

### `game/play/checks.c` (84)

| Function | Original | What it does |
|---|---|---|
| `divs_ovf` | (a helper) | divs.w with the overflow flag |
| `aidef_timer` | (a helper) | The timer / aidef prologue shared by the assignments: when the timer runs out reload it from aidef, one less while the crowd meter record is broken (sflags7 bit 6). |
| `goalie_timer_reload` | (a helper) | the goalie variant: aidef / 4, one less with the crowd record broken |
| `breakaway_crowd` | (a helper) | count the breakaway and excite the crowd (asspuckc, assnearest, breakaway) |
| `zone_target` | (a helper) | a random point inside skating zone z of a zone table: {x base, x range, y base, y range} |
| `asswingo` | `checks94.asm:asswingo` | Player a3 is a winger on offense: skate to a random spot in the zone the puck is in (or will be), evading the puck carrier. |
| `asscenterd` | `checks94.asm:asscenterd` | Player a3 is the center on defense: stay between the puck and his own high slot. |
| `asscentero` | `checks94.asm:asscentero` | Player a3 is the center on offense. |
| `assgoaliectrl` | `checks94.asm:assgoaliectrl` | Joypad controlled goalie: out of the crease area the pad takes over (pf3gctrl) and checkanim runs; else time his hold on the puck and his timer. |
| `assgoaliecpu` | `checks94.asm:assgoaliecpu` | a3 = goalie. |
| `checkanim` | `checks94.asm:checkanim` | The goalie's own play: the stick slam after a goal, skating back to the crease, passing the puck when he has it, diving on a loose puck, and reading the shot (puckcross) to pick a save or move across the crease. |
| `ClampYPosition` | `checks94.asm:ClampYPosition` | d1 = y clamped to +-$103, minus goal line d3. |
| `AdjustFacingDirection` | `checks94.asm:AdjustFacingDirection` | Turn facedir one step toward direction d0. |
| `goaliesave` | `checks94.asm:goaliesave` | Set the save animation for goalie a3 from where the puck will cross (a0 = puckcross entry, d3 = his goal line): the direction of the shot relative to his facing picks the side, the height and distance pick the type. |
| `assgoalietopuck` | `checks94.asm:assgoalietopuck` | The goalie skates to a loose puck coming his way when no skater of either team is close to it. |
| `breakaway` | `checks94.asm:breakaway` | Puck carrier a3 on a breakaway: beep on a new assignment and check the skating direction toward the net. |
| `BreakawayOffsidesFlagSet` | `checks94.asm:BreakawayOffsidesFlagSet` | On a new assignment clear the offsides and breakaway bits of a3. |
| `chkpuckc` | `checks94.asm:chkpuckc` | Puck carrier a3: check for a breakaway (not in a shootout or penalty shot), else assign asspuckc. |
| `asspuckc_chkdir` | (a helper) | the a0 callback of skateto in asspuckc: count the opponents converging on the puck (threat) and turn away from one that is right in front |
| `asspuckc` | `checks94.asm:asspuckc` | Player a3 is the puck handler: carry it up the ice to a spot from .postab2 (the shootout path instead), looking for a shot or a pass now and then. |
| `chk4lc` | `checks94.asm:chk4lc` | See if the computer should call a line change: with the puck in the neutral zone, now and then, when the line's energy is low. |
| `chkpk` | `checks94.asm:chkpk` | Z set if killing a penalty. |
| `chkpk2` | `checks94.asm:chkpk2` | Z set when sf2pwrtm (team on the power play) differs from pfteam. |
| `chk4shot` | `checks94.asm:chk4shot` | Computer puck carrier a3 looks for a shot. |
| `CompShoot` | `checks94.asm:CompShoot` | Computer shot: drop the return address (the caller's assignment ends) and aim from the puck's distance to the goal line. |
| `chk4pass` | `checks94.asm:chk4pass` | Computer puck carrier a3 looks for a good pass to a random teammate. |
| `EvadePC` | `checks94.asm:EvadePC` | Player a3 should avoid the puck carrier if he's on their team: with the carrier close, skate away from him; with offsides on, along the blue line around the puck. |
| `checkob` | `checks94.asm:checkob` | Bring up the little warning ref when a player on a3's team is over the blue line ahead of the puck (offsides on). |
| `assbreakaway` | `checks94.asm:assbreakaway` | Assignment for a breakaway: the carrier keeps it while he skates at the net, everyone else (and the carrier once he turns) goes to assnearest. |
| `assnearest` | `checks94.asm:assnearest` | A special assignment used for the player who is nearest the puck but doesn't have it: find his team's nearest skater (tmneardist), hand the assignment to him if it is someone else, else chase the puck (or the carrier) and look for a check. |
| `check4check` | `checks94.asm:check4check` | Look for a good opportunity to check an opponent: now and then (less with penalties on), an opponent within $1E in the direction he faces. |
| `asspassrec` | `checks94.asm:asspassrec` | Assignment for catching a pass: coast until the puck is loose, then either wait for it or (not a human team, puck on the attack half) go for a one-timer. |
| `SkateToTempTarget` | `checks94.asm:SkateToTempTarget` | Unreferenced: skate to temp3 / temp4. |
| `assshoot` | `checks94.asm:assshoot` | Assignment for computer shooting: wind up (SetShotMode) and release (ShotMode with the C button) when temp2 runs out. |
| `puckshootout` | `checks94.asm:puckshootout` | Puck logic for the shootout: on a new assignment clear the pads and select the skater, then run the attempt. |
| `SelectPenaltyShotSkater` | `checks94.asm:SelectPenaltyShotSkater` | Pick the skater for the penalty shot from team BA_Team: the best rated (sum of the attribute nibbles) player on the ice or bench (any player in a shootout). |
| `reset_ice_objects` | (a helper) | place every object for the faceoff / penalty shot: the two nets, the puck shadow, scroll to the spot, reset the benches and lines |
| `puckpenshot` | `checks94.asm:puckpenshot` | Puck start for a penalty shot / shootout attempt: black out, put the skater on the puck at center ice and the goalie in his net, everyone else off the ice. |
| `UpdatePenaltyShotEnd` | `checks94.asm:UpdatePenaltyShotEnd` | End the penalty shot when the goalie has the puck, the puck is loose or the timer runs out. |
| `EndPenaltyShotPlay` | `checks94.asm:EndPenaltyShotPlay` | Stop play after a penalty shot or shootout attempt: freeze the window, stop the clock, clear the penalty shot flags. |
| `puckfaceoff` | `checks94.asm:puckfaceoff` | This is where the action starts: on a new assignment settle the period end, delayed penalties, pulled goalies and the song, then start the line changes and wait for them (WaitForFaceoffLineChanges) before puckfaceoff2. |
| `SetFaceoffComputerLine` | `checks94.asm:SetFaceoffComputerLine` | Faceoff: start a line change for team d0 when the computer controls it (line changes on). |
| `StartFaceoffLineChange` | `checks94.asm:StartFaceoffLineChange` | Start the line change box for team d0's sort object before a faceoff; the puck's temp1/temp3 (home) or temp2/temp4 (away) time it. |
| `WaitForFaceoffLineChanges` | `checks94.asm:WaitForFaceoffLineChanges` | Run both teams' faceoff line change timers (temp1 temp2); continue when both have run out. |
| `UpdateFaceoffLineChangeTimer` | `checks94.asm:UpdateFaceoffLineChangeTimer` | Count down one team's faceoff line change timer at offset d0 ($40 = temp1, $42 = temp2) of a3; the object number sits 4 bytes on. |
| `chkgoalie` | (a helper) | the body of ChkGoalies for one team: d0 = team number, a2 = the team, a1 = the other |
| `ChkGoalies` | `checks94.asm:ChkGoalies` | Computer pulls the goalie on a delayed penalty (or late in the game when behind). |
| `returngoalie` | (a helper) |  |
| `ReturnGoalies` | `checks94.asm:ReturnGoalies` | If the computer pulled his goalie, look to see if he should return him (he stays out for a faceoff in the attack zone). |
| `CPgoalie` | `checks94.asm:CPgoalie` | See if the computer should pull his goalie: third period, two goals behind, under a minute left, play (d1 = y) in the attack half. |
| `CompLine` | `checks94.asm:CompLine` | Find a good line for the computer team a2 to switch to (a1 = the other team): the power play / penalty kill line pair, else a rested line matched to the opponent's. |
| `puckfaceoff2` | `checks94.asm:puckfaceoff2` | Face off control logic and general setup for action: on a new assignment black out, reset the rink and every object around the faceoff spot, draw the faceoff window and pick the drop time; then count the drop time down (updatefaceoff) and drop the puck (Endfaceoff). |
| `ResetAndSelectPlayers` | `checks94.asm:ResetAndSelectPlayers` | c1playernum / c2playernum = -1, then changeplayer for each human team (pads 3 and 4 borrow the pad 1 and 2 variables). |
| `updatefaceoff` | `checks94.asm:updatefaceoff` | a3 = puck. |
| `Endfaceoff` | `checks94.asm:Endfaceoff` | Drop the puck: pick the faceoff winner from the two men's button timing (fofdata2), send the puck off in his direction (or a random one) with a hop. |
| `pucknorm` | `checks94.asm:pucknorm` | Assignment for the puck most of the time: age the crossing table, bungie the puck to its carrier's hot spot, icing and offsides, the delay of game whistle when it sits still, then the collisions. |
| `ChkOffsides` | `checks94.asm:ChkOffsides` | Check for offsides: a3 = puck crossing a blue line with a skater of the attacking team already over it flags the whole team (tmflags bit 4). |
| `ClearOffsidesIfAllPlayers` | `checks94.asm:ClearOffsidesIfAllPlayers` | a2 = team struct: clear the team offsides flag once no skater is past the line. |
| `a2offsides` | `checks94.asm:a2offsides` | Player a2 touched the puck: offsides penalty if his team is flagged and the puck is past the blue line. |
| `a2touchpuck` | `checks94.asm:a2touchpuck` | Player a2 touches the puck: remember him for the scoring, look for offsides and icing. |
| `puckIChk` | `checks94.asm:puckIChk` | Check for icing of the puck: a loose puck crossing the far goal line outside the crease sets ifcgl. |
| `puckunflip` | `checks94.asm:puckunflip` | Stop the spinning puck. |
| `puckflip` | `checks94.asm:puckflip` | Start the puck spinning; d0 bit 0 picks the direction. |
| `puckshadow` | `checks94.asm:puckshadow` | Assignment for the puck shadow: follow the puck, one below; when its frame is not the puck it is the goal siren instead. |
| `findpc_calc` | (a helper) | one goal line of findpc: a1 = the puckcross entry, d1 = the sideline, d4 = the goal line |
| `findpc` | `checks94.asm:findpc` | Find where and when the puck will cross each goal line (if at all), into puckcross (x, frames) x 2. |
| `skateto` | `checks94.asm:skateto` | Skate player a3 toward d0/d1, every 12 frames: the direction to the point (9 = there), through the aux routine a0 (collision avoidance), then the acceleration. |
| `avdgoal_ch1` | (a helper) | the bar check of avdgoal: d2 / d3 = y of the player and the target relative to a goal line; crossing it within the net's width moves the target x off the post |
| `avdgoal` | `checks94.asm:avdgoal` | Don't try to skate through the goal: if the line from a3 to d0/d1 crosses a net, move the point around it (deltax / deltay). |
| `skatetopuckinit` | `checks94.asm:skatetopuckinit` | Find where the puck is going to be. |
| `skatetopuck` | `checks94.asm:skatetopuck` | Player a3 should skate to the puck, every 10 frames; a skater facing a puck coming straight at him from a little way off sweep checks. |
| `assexit` | `checks94.asm:assexit` | Exit the current assignment on player a3. |
| `assinsert` | `checks94.asm:assinsert` | Insert a new assignment on player a3 (the current one resumes after it). |
| `assreplace` | `checks94.asm:assreplace` | Replace the current assignment on player a3. |
| `vtoa` | `checks94.asm:vtoa` | d0/d1 = x/y vector -> d0 = facing direction 0-7 (0 = up, clockwise), 8 = no direction. |
| `GetHot` | `checks94.asm:GetHot` | Long on the stack = object; returns d0/d1 = the hot spot (where the puck sits) of the object's current frame, from HotList, flipped with the sprite and swapped for the horizontal rink. |
| `SetSPA` | `checks94.asm:SetSPA` | a3 = object, d1 = animation. |
| `doplayeracc` | `checks94.asm:doplayeracc` | Player a3 gets acceleration in direction d0 (8 = none, 9 = stop): decide whether to skate backwards (defending in his own zone, or already going that way), turn toward the direction with the turning animation when moving, then playeracc. |
| `goalieacc` | `checks94.asm:goalieacc` | Goalie gets acceleration in direction d0: a human goalie in his crease turns to face the puck (AdjustFacingDirection) and stops when idle; otherwise turn one step toward d0 and skate. |
| `noturn0` | `checks94.asm:noturn0` | doplayeracc when the wanted direction needs no turn (.ftab entry 0). |
| `noturn` | `checks94.asm:noturn` | Reverse the acceleration direction when skating backwards, then playeracc. |
| `playeracc` | `checks94.asm:playeracc` | Accelerate player a3 in direction d2: the direction's unit vector scaled by his agility and weight (goalies double), not into a wall, over the elapsed frames; capped at his top speed, which his energy, speed rating and a fight injury set; then now and then burn energy (line changes on). |
| `dostop` | `checks94.asm:dostop` | Player a3 stops: the stopping animation when moving fast enough. |
| `stopna` | `checks94.asm:stopna` | Stop with no animation: slow the velocity toward 0 by $96. |

### `game/play/collide.c` (45)

| Function | Original | What it does |
|---|---|---|
| `checkcoll` | `collide94.asm:checkcoll` | d2/d3 = new x/y, a3 = object. |
| `checkplcoll` | `collide94.asm:checkplcoll` | d2/d3 = x/y, a3 = object. |
| `checkcx` | `collide94.asm:checkcx` | d4 = other object number * 2, d2 = x, d5 = delta y, a3 = moving object. |
| `newcheck` | `collide94.asm:newcheck` | Start a new check sound: one of 4 sounds $1C-$1F, never the last one again. |
| `ci` | `collide94.asm:checkint` | (.ci). |
| `checkint` | `collide94.asm:checkint` | Interference, each way round. |
| `checkcheck` | `collide94.asm:checkcheck` | Players are in contact: CCStart for a3 on a2, then a2 on a3. |
| `CheckWeightOdds` | (a helper) | (native) CCStart's odds that a check fails: d0 less the checker's weight plus the checked player's, summed in a byte as the original does (sub.b / add.b: a heavy player checked by a light pad player wraps round to a small number, and falls easily) |
| `CCStart` | `collide94.asm:CCStart` | Player a3 is checking player a2 (d4 = impact). |
| `checkagr` | `collide94.asm:checkagr` | a3 = checking player, a2 = player being checked. |
| `holdcheck` | `collide94.asm:holdcheck` | a2 is in a hold animation looking to hold a3, who must be within one direction of where a2 faces. |
| `Bcheck` | `collide94.asm:Bcheck` | a2 is sweep checking, a3 is the player being checked. |
| `FallDown` | `collide94.asm:FallDown` | Player a2 falls down, player a3 is the hitting player. |
| `setInjuryType` | `collide94.asm:setInjuryType` | a2 = player injured, a3 = player checking. |
| `checkwallcoll` | `collide94.asm:checkwallcoll` | d2/d3 = x/y to test, a3 = object, wcradiusx/y = radius. |
| `setass` | `collide94.asm:checkgoal` | .setass. |
| `GetPeriodTimeRemaining` | `collide94.asm:GetPeriodTimeRemaining` | d0 = (gsp << 14 \| PerTimeTotal) - gameclock. |
| `checkgoal` | `collide94.asm:checkgoal` | a2 = goal object, a3 = object, d2/d3 = x/y. |
| `checkgoalp` | `collide94.asm:checkgoalp` | Player a3 against goal a2 (oval): CheckBump, then wallcoll without setting the wall collision bit. |
| `CheckBump` | `collide94.asm:CheckBump` | a2 = goal, a3 = player. |
| `wallcollb` | `collide94.asm:wallcollb` | a3 = object, d0/d1 = cos/sin of the wall. |
| `wallcoll` | `collide94.asm:wallcoll` | d0/d1 = cosine/sine of the wall, a3 = object. |
| `checkpuckcoll_sfx` | `collide94.asm:checkpuckcoll_sfx` | Puck in the air: sfx 5 once when sflags4 bit 2 is set. |
| `chkgoalie` | `collide94.asm:checkpuckcoll` | .chkgoalie. |
| `ccx` | `collide94.asm:checkpuckcoll` | .ccx. |
| `checkpuckcoll` | `collide94.asm:checkpuckcoll` | a3 = puck. |
| `puckstick` | `collide94.asm:puckstick` | a2 = player, a3 = puck, d0 = distance squared. |
| `setd0player` | `collide94.asm:puckglue` | .setd0player. |
| `puckglue` | `collide94.asm:puckglue` | Player a2 takes the puck: faceoff and pass stats, the crowd, a goalie hold time, then pad control. |
| `puckbody` | `collide94.asm:puckbody` | The puck hits player a2 and bounces off; a high, fast puck makes him fall, a slower one starts the catch animation. |
| `puckgoalie` | `collide94.asm:puckgoalie` | The puck hits goalie a2 (d0/d1 = goalie - puck x/y). |
| `deflect` | `collide94.asm:deflect` | Random puck direction on a deflection. |
| `getpde` | `collide94.asm:getpde` | a3 = player -> d0 = his energy level (tmpde of his team). |
| `makepde` | `collide94.asm:makepde` | Scale d0 by player a0's energy level (full while sflags7 bit 4). |
| `setpde` | `collide94.asm:setpde` | d1 = roster number * 2, a2 = team, d0 = new energy level (floor 0). |
| `setpersonel` | `collide94.asm:setpersonel` | Set the personnel of team a2: PlList gives the players wanted on the ice; each sort object gets newpos / newpnum. |
| `TryAddPlayerToList` | `collide94.asm:TryAddPlayerToList` | d0 = player number (1 based), d4 = PlList slot. |
| `SetPlList` | `collide94.asm:SetPlList` | Build PlList, the players wanted on the ice now, from the current line by position priority; an unavailable player is replaced from the substitution list for his position, or from the whole roster. |
| `SetupPenaltyShot` | `collide94.asm:SetupPenaltyShot` | Team a2: the shooter plays centre, the goalie stays, every other player is made unavailable with the wait assignment. |
| `forcepldata` | `collide94.asm:forcepldata` | Force the players of team a2 to their requested position and roster slot (faceoffs only). |
| `rb` | `collide94.asm:ResetBench` | .rb. |
| `ResetBench` | `collide94.asm:ResetBench` | Everyone out of the penalty box and onto his bench. |
| `Setplass` | `collide94.asm:Setplass` | Player a3's initial assignment by position. |
| `ClampNibble` | `collide94.asm:ClampNibble` | Clamp byte d3 to 0-$1E. |
| `setplayer` | `collide94.asm:setplayer` | Bring player d3 of the roster onto the ice as sort object a3 and set his attributes from the team data, through AttributeCalc, with the power play / penalty kill, home / away and third period bonuses. |

### `game/play/goalie.c` (7)

| Function | Original | What it does |
|---|---|---|
| `ManualGoalieMenu` | `goalie94.asm:ManualGoalieMenu` | The "Manual Goalie" menu item: toggle the goalie mode of the pause pad; in a penalty shot or shootout the pad then takes the skater or the goalie on its side by that mode. |
| `RunArenaAnim` | `goalie94.asm:RunArenaAnim` | Run the arena animation arenaanim (negative: none): the first time load its frame list and graphics from ArenaAnims, then count the frame time down by the elapsed frames and step. |
| `NextArenaFrame` | `goalie94.asm:NextArenaFrame` | Read frame arenaframeidx of the list: arenaframe = frame, arenaframetime = time; frame $FF loops to the start, $FE ends the animation. |
| `SetCameraTop` | `goalie94.asm:SetCameraTop` | Unreferenced. |
| `StartArenaAnim` | `goalie94.asm:StartArenaAnim` | Start arena animation d0; RunArenaAnim loads it. |
| `SetFaceoffAnim` | `goalie94.asm:SetFaceoffAnim` | sflags7 bit 0, faceoffanim = d0. |
| `EndArenaAnim` | `goalie94.asm:EndArenaAnim` | arenaanim = -1, sflags7 bit 0 cleared. |

### `game/play/hockey.c` (23)

| Function | Original | What it does |
|---|---|---|
| `ClearShotData` | `hockey94.asm:ClearShotData` | Clear $E4 words at outputbuffer. |
| `restoreteams` | `hockey94.asm:restoreteams` | Put both teams' rosters on the bench. |
| `ResetClock` | `hockey94.asm:ResetClock` | Set the period length and stop the clock; the late game song trigger time is a random moment in the second half. |
| `GetPeriodTime` | `hockey94.asm:GetPeriodTime` | The period length in seconds for the option. |
| `periodicevents` | `hockey94.asm:periodicevents` | Called every game loop pass: penalties, the crowd, the arena animation, the sound and the clock, then once a second the goalies, the excitement, the late game song, the bench energy, injuries and the power play. |
| `CheckInjury` | `hockey94.asm:CheckInjury` | Once a second: count InjCntDown down and show the injury box at zero. |
| `RestoreBenchEnergy` | `hockey94.asm:RestoreBenchEnergy` | Once a second: restore energy for the players on the bench (line changes on). |
| `CheckLateGameSong` | `hockey94.asm:CheckLateGameSong` | Third period: choose and play a song once at the random trigger time ResetClock set. |
| `UpdateCwdExcite` | `hockey94.asm:UpdateCwdExcite` | Once a second: track the peak and running total of the crowd excitement, then decay the level by 1. |
| `updatecrowdf` | `hockey94.asm:updatecrowdf` | Every game loop pass: the crowd level calms down and the crowd animation frame is picked from it (cd0: frame / time pairs by level and step). |
| `clockcont` | `hockey94.asm:clockcont` | Monitor the period clock: at zero stop everything and end the period. |
| `clockcont_0` | `hockey94.asm:clockcont_0` | End of period. |
| `EndDemoOnPress` | `hockey94.asm:EndDemoOnPress` | Any button on the pad just read ends the demo. |
| `demoread` | `hockey94.asm:demoread` | Monitor the pads in demo mode: start pauses, anything else ends the demo. |
| `SetupPauseScreen` | `hockey94.asm:SetupPauseScreen` | The draw routine of the pause menu: the window plane over the horizontal rink, the crowd silenced, plane 3 erased. |
| `seta2` | `hockey94.asm:seta2` | The team struct of the pause joystick. |
| `DoGameFrame` | `hockey94.asm:DoGameFrame` | Wait for a vblank, then one frame of game logic: d7 (elapsed) = the frames since the last pass. |
| `Gameloop` | `hockey94.asm:Gameloop` | The game: a frame, the demo check, the pause when it is asked for. |
| `start_period` | (a helper) | StartPer after the stack reset |
| `StartPer` | `hockey94.asm:StartPer` | Start a period: overtime flag, the faceoff animation, then with a fresh stack: sound off, the rink, the clock, the opening faceoff, two frames, the crowd and the period tune, and the game loop. |
| `Pausemode` | `hockey94.asm:Pausemode` | The pause menu: fade out, silence, the menu for the pausing pad's team (shootout or alternate item lists), run it until it is left, then put the rink back (vertical or horizontal as it was), redraw the scores and fade in. |
| `Begin` | `hockey94.asm:Begin` | The cold start from Start: clear the game's RAM ($B000-$DEF3), note PAL, start the sound, look for the 4 Way Play adaptor, the publisher's logo screen, save RAM, the line data, the attract screen, the default options, then the title (Opening). |
| `StartGame` | `hockey94.asm:StartGame` | A new game: the presses counted, 30 second periods when pad 1 holds Start+A+C, offsides, the shot buffer, the team stats (hot / cold tables in a shootout), the scores, the rink, the crowd record, then the intermission into the first period. |

### `game/play/input.c` (38)

| Function | Original | What it does |
|---|---|---|
| `doinput` | `input94.asm:doinput` | Process controller input for player a3: d0 = dpad, d1 = new buttons, d2 = changed buttons, d3 = held buttons, d4 = controller 0 / 2 (d6 = his SCnum). |
| `ClampTargetY` | `input94.asm:ClampTargetY` | Clamp the target y d1 for player a3 to the goal line d3 (d0 cleared when a3 has the puck). |
| `doinput_cbut` | `input94.asm:doinput_cbut` | C button (bit 5) for the goalie: a press reads the shot and starts a save (goaliesave); held, it holds the save pose (SPAcnt). |
| `doinput_chkanim` | `input94.asm:doinput_chkanim` | Nothing while a3's animation runs; a goalie stops at the edges of his crease area, else skate (doplayeracc). |
| `doinput_onetimer` | `input94.asm:doinput_onetimer` | a3 is set for a one-timer: C (bit 5) starts the one-timer shot (a pass from a skater, puck loose on the attack half), else a burst or skating. |
| `doinput_ispc` | `input94.asm:doinput_ispc` | a3 has the puck: pass mode, shot mode, or start a pass / shot by the buttons (B pass, A held = line change, C shot). |
| `doinput_islocked` | `input94.asm:doinput_islocked` | a3 is locked in an animation: B (bit 4) changes the controlled player, who then has to let go of B (pflags3 bit 6). |
| `getGoalieSCnum` | `input94.asm:getGoalieSCnum` | d0 = SCnum of the goalie in the 6 sort objects up to object d0, -1 if none. |
| `faceoffinput` | `input94.asm:faceoffinput` | Controller processing for the faceoff: the dpad goes to fodir1 fodir2 (the pull), B swings early (SPAfaceoff) else the normal swing. |
| `SetLCmode` | `input94.asm:SetLCmode` | Initiate the line change option if available (line changes on, the box not already up for his team), then draw the box. |
| `SetLCmode2` | `input94.asm:SetLCmode2` | a2 = team struct. |
| `setlccords` | `input94.asm:setlccords` | Set printx / printy to the coordinates of the line change box of team a2 (the home side flips with gmode's gmdir). |
| `getlchoice` | `input94.asm:getlchoice` | d0 = 0-2 (a2 = team): the line number that choice means, from the current line and the power play situation; -1 = no such choice. |
| `getlchoice2` | `input94.asm:getlchoice2` | Add $15 to d0 for each player more or fewer than the other team has on the ice, 3 x the current line, then look it up. |
| `lineinput` | `input94.asm:lineinput` | Process input for line changes: d1 = new button presses (A / B / C pick a line); a pass in progress goes on; a cancelled box is redrawn. |
| `lcfound` | `input94.asm:lcfound` | d2 = choice made 0-2: set the team's line and close the box. |
| `lcfound2` | `input94.asm:lcfound2` | Close the line change box of team a2. |
| `burst` | `input94.asm:burst` | C button press: a burst of speed in the facing direction, paid for in energy (line changes on). |
| `holdplayer` | `input94.asm:holdplayer` | A button hold: hook the last player he hit (Acheck). |
| `Acheck` | `input94.asm:Acheck` | Start SPAhook, or SPAHold if a0 is ahead. |
| `setpassmode` | `input94.asm:setpassmode` | Initialize pass mode: the pass direction starts as his facing. |
| `passmode` | `input94.asm:passmode` | In pass mode: B released (or a one-timer pass) passes; the dpad sets the direction and passes. |
| `dopass` | `input94.asm:dopass` | Pass the puck in passdir, to the best teammate there if any (passto), else just send it that way; a one-timer pass goes to the receiver's spot. |
| `passto` | `input94.asm:passto` | Pass the puck to player a0 from a3: a human goalie's pass hands the controller to the receiver. |
| `passtoa0` | `input94.asm:passtoa0` | Pass the puck to player a0: he gets apassrec; the puck's velocity is solved so that it meets him where he will be (passspeed pixels per second, his speed per quarter second), with a hop timed to the meeting. |
| `changeplayer` | `input94.asm:changeplayer` | The pad d4 takes the nearest free skater (chgplayer); a3 sweep checks when it is the pad's own player. |
| `Sweepcheck` | `input94.asm:Sweepcheck` | Start a sweep check on player a3. |
| `setc1player` | `input94.asm:setc1player` | / setc2player. |
| `setc2player` | (a helper) |  |
| `restorepl` | `input94.asm:restorepl` | d1 = the old player: give him back to the computer (unless he is in a line change box); d0 = the new one takes the pad, except a goalie the pad does not control in a penalty shot / shootout (the first skater instead). |
| `Findhittype` | `input94.asm:Findhittype` | Look for the type of swing: d0 = launch direction relative to his facing and stick side. |
| `SetShotMode` | `input94.asm:SetShotMode` | Initiate a shot by player a3: wind up forehand or backhand toward the far boards. |
| `ShotMode` | `input94.asm:ShotMode` | Shot input: the dpad aims, the windup adds speed until the C button changes (or the swing is forced past the half way point for weak shooters); at the end of the animation the shot goes (prepshot). |
| `prepshot` | `input94.asm:prepshot` | Check certain conditions before shooting: a shot from in close at a standing goalie goes top shelf (sflags5 bit 4). |
| `doshot` | `input94.asm:doshot` | The stick is at the puck: launch it toward the goal. |
| `CalcAngleOffset` | `input94.asm:CalcAngleOffset` | The area of the triangle puck - goalie - post over the distance from the goalie to the puck: which side of the goalie the post is seen on. |
| `shotdiradj` | `input94.asm:shotdiradj` | Where a computer player (or a one-timer) shoots: at the post the goalie leaves more open, straight when he covers the middle, 8 with no goalie. |
| `setpads` | `input94.asm:setpads` | Put the SCnum of a3 in the d4 nibble of PadControlBits; d4 = -2 puck carrier, 0 / 2 pads, 4 replay target. |

### `game/play/onetimer.c` (8)

| Function | Original | What it does |
|---|---|---|
| `puckvzadj` | `onetimer94.asm:puckvzadj` | Set puckvz for a top shelf shot from the distance to the goal line over puckvy, at most $7FFF. |
| `OneTimerPass` | `onetimer94.asm:OneTimerPass` | puckvz = sqrt(12 * onetimerheight), then puckvx / puckvy toward the target in puckvz / 3 frames; flip the puck. |
| `OneTimerTarget` | `onetimer94.asm:OneTimerTarget` | The pass target for receiver a3: an offset by facing from the near / far / goalie tables, +-10 at random, into onetimertargetx / y; sflags6 bit 2 = near. |
| `assonetimer` | `onetimer94.asm:assonetimer` | Assignment $23, player a3 shooting a one-timer: take control of him, start the animation, then shoot when the puck arrives (EndOneTimer). |
| `setonetimeranim` | `onetimer94.asm:setonetimeranim` | The one-timer animation: SPAshotf or SPAshotb from the angle to the goal. |
| `PuckOnAttackHalf` | `onetimer94.asm:PuckOnAttackHalf` | Returns the Z flag: set when the puck is not on the half of the goal player a3 shoots at. |
| `onetimershot` | `onetimer94.asm:onetimershot` | Do the one-timer shot, credit the last two passers as the assists, excite the crowd and count the attempt. |
| `CheckOneTimerFacing` | `onetimer94.asm:CheckOneTimerFacing` | Only its Z flag is used: the shot is a forehand when the sprite's flip bit matches the side the goal is on. |

### `game/play/penalty.c` (66)

| Function | Original | What it does |
|---|---|---|
| `loadTeamStruct` | `penalty94.asm:loadTeamStruct` | a2 = team struct of player a3, a1 = the other team's. |
| `AddPenalty` | `penalty94.asm:AddPenalty` | Add penalty d0 for player a3. |
| `AddPenalty2` | `penalty94.asm:AddPenalty2` | Forced penalties like the faceoff and game over. |
| `PenaltyManager` | `penalty94.asm:PenaltyManager` | Called every game loop pass (d7 = elapsed frames). |
| `chkprogress` | `penalty94.asm:chkprogress` | Control the progress of the ref and the game through the penalty events: the penalty shot message timer, then when the stop delay runs out and a penalty with minutes is pending switch to the horizontal rink for the player to enter the box (InProgress takes it from there). |
| `PenShotChk` | `penalty94.asm:PenShotChk` | a3 = checker, a2 = player hit, d0 = penalty. |
| `getBAplayerInfo` | `penalty94.asm:getBAplayerInfo` | d0 = SCnum of the player on the breakaway, a3 = the checker. |
| `InProgress` | `penalty94.asm:InProgress` | Ref in progress: update graphics, stats and the penalty information. |
| `coinsearch` | `penalty94.asm:coinsearch` | a2 = team. |
| `checkfornewpen` | `penalty94.asm:checkfornewpen` | Look for a new penalty (entered through AddPenalty). |
| `Stop4Pen` | `penalty94.asm:Stop4Pen` | a0 = PenBuf entry + 2 (the penalty number and player sit before it; puckfaceoff and puckshootout branch here with a0 = their own code). |
| `limitfo` | `penalty94.asm:limitfo` | Limit the faceoff to 5-20 feet from the walls, for every player in PenBuf (the first entry is looked at even when empty). |
| `UpdatePA` | `penalty94.asm:UpdatePA` | Animate the ref in the ref window. |
| `SetPA` | `penalty94.asm:SetPA` | Start the ref animation d0 (the penalty number). |
| `SetPA2` | `penalty94.asm:SetPA2` | The next frame / delay pair of the ref animation of RefPen (frame in the high byte, delay * 8 frames in the low byte; negative = the last one). |
| `PushRef` | `penalty94.asm:PushRef` | Tell vblank what to display: d0 = ref frame into RefRamMap, $40 clears the window. |
| `prefmes` | `penalty94.asm:prefmes` | Print the message for penalty d0 (negative clears it). |
| `ClearPenaltyMessage` | `penalty94.asm:ClearPenaltyMessage` | Blank the horizontal mode penalty message line. |
| `PenGoalStuff` | `penalty94.asm:PenGoalStuff` | After a goal: a1 = scored on team, a2 = scoring team. |
| `ClearPenaltyBuffer` | `penalty94.asm:ClearPenaltyBuffer` |  |
| `updatepentime` | `penalty94.asm:updatepentime` | Update the time remaining on all penalized players, once a second (sflags3 bit 6 marks that tick). |
| `penbox_check` | (a helper) | a player's box time is 5 seconds or less: beep, and release him at 0 |
| `ProcessPenaltyList` | `penalty94.asm:ProcessPenaltyList` | a2 = team. |
| `RemovePlayerFromList` | `penalty94.asm:RemovePlayerFromList` | Remove the entry before a0 from a penalty box list by shifting the rest down (the list ends with a negative byte). |
| `chkatop` | `penalty94.asm:chkatop` | Attack time of possession stat, once a second. |
| `releasepl` | `penalty94.asm:releasepl` | A player's penalty time is up so let him out: a2 = team, d0 = player * 2. |
| `GetLowestPen` | `penalty94.asm:GetLowestPen` | a2 = shorthanded team, a3 = team on the power play. |
| `clrpwrplay` | (a helper) | the end of a power play: the EASN logo comes back |
| `updatepwrplay` | `penalty94.asm:updatepwrplay` | Show the graphic and time remaining for a power play (nothing when gmode2 bit 1 is set). |
| `ClrHor` | `penalty94.asm:ClrHor` | Revert the graphics to the vertical ice rink: reload the rink tiles and the home team's graphics, redraw the scoreboard. |
| `SetHor` | `penalty94.asm:SetHor` | Switch the graphics to the horizontal ice rink. |
| `printscores1` | `penalty94.asm:printscores1` | Draw the scoreboard. |
| `PrintTeamNameAndScore` | `penalty94.asm:PrintTeamNameAndScore` | The abbreviation of team a2 at printx / printy, then its score as 2 digits at x $1C. |
| `PrintTeamLogoAndScore` | `penalty94.asm:PrintTeamLogoAndScore` | The team logo map (d0 = offset) 5 columns left of printx, then the score of a2 in the big font, centered 2 rows lower. |
| `EASNLogo` | `penalty94.asm:EASNLogo` | The EASN logo map at x 1, y $19 on the vertical rink if no power play. |
| `dispen` | (a helper) | the penalty box list of team a2: the players without a coincidental penalty first, then those with one |
| `USBoard` | `penalty94.asm:USBoard` | Update the scoreboard, including the players in the penalty box and their time remaining. |
| `pplpen` | `penalty94.asm:pplpen` | One penalty box line: the player's number and time remaining. |
| `linebar` | `penalty94.asm:linebar` | The energy bar of line d0 for team a2 at printx / printy (16 steps). |
| `getlinee` | `penalty94.asm:getlinee` | d0 = line number, a2 = team struct. |
| `AvgCline` | `penalty94.asm:AvgCline` | The average energy of the current line on team a2. |
| `ChkShotStat` | `penalty94.asm:ChkShotStat` | Determine if a shot was taken and add it to the stats: crowd, team (power play and period shots), shooter and goalie shots against. |
| `intermission_team` | (a helper) | refill one team (a2) and pick the other's (a3) starting line from the players on the ice |
| `SetupTeamForIntermission` | `penalty94.asm:SetupTeamForIntermission` | Reset the bench, then for each team refill the energy and pick the starting line. |
| `RestoreTeamEnergy` | `penalty94.asm:RestoreTeamEnergy` | a2 = team: max energy for every player. |
| `reenergizeteam` | `penalty94.asm:reenergizeteam` | Max energy for team a2 except the players at tmpdst -4; players marked -3 go to the bench (-2). |
| `getscore` | (a helper) | the weighted goals of one team's period: d0 = scoring team, d1 = the other team. |
| `SetScore` | `penalty94.asm:SetScore` | Add to the score of the ticker game a0. |
| `InitScores` | `penalty94.asm:InitScores` | Initialize the other games' scores / period in the playoffs. |
| `UpdateScores` | `penalty94.asm:UpdateScores` | Update the ticker score values (PeriodOver). |
| `NewTicker` | `penalty94.asm:NewTicker` | Show the next ticker score once the clock is at 8:00 or less. |
| `NewTicker2` | `penalty94.asm:NewTicker2` | The next ticker game, skipping this game. |
| `CheckGameTickerStatus` | `penalty94.asm:CheckGameTickerStatus` | d3 = game. |
| `ticker_team` | (a helper) | one team's name and score on the next ticker row |
| `NewTicker3` | `penalty94.asm:NewTicker3` | Display the ticker score for game d3 (a0 = its gstruct); nothing for a negative d3. |
| `ClearTickerArea` | `penalty94.asm:ClearTickerArea` | Erase the ticker box. |
| `SetTickerAreaPosition` | `penalty94.asm:SetTickerAreaPosition` | printx / printy / printm for the ticker box (x 3, y $17; map 3 in pause mode). |
| `PrintStringFromList` | `penalty94.asm:PrintStringFromList` | Print string d0 of the String list a1 with printsmall. |
| `AdvanceStringPtr` | `penalty94.asm:AdvanceStringPtr` | a1 = string d0 of the String list a1. |
| `hl_save` | (a helper) | StartHL2's .sv / .lo: keep the game's RAM from gmode on (and ScoreSumbytes) at M68K_RAM while a highlight game uses it |
| `hl_restore` | (a helper) |  |
| `hl_random_result` | (a helper) | StartHL2's .ranres: settle highlight game a0 at random when tied, and mark it final |
| `hl_set_team` | (a helper) | StartHL2's .setteam: refill the energy, personnel and player data of team a2 |
| `StartHL2` | `penalty94.asm:StartHL2` | Play the highlight of game a0 (d1 = its number): its ticker score, "Highlight from game:", then the two teams on the rink with a random 60-180 seconds left, the computer playing until 180 frames after the clock stops or C / start (a tie is settled at random); the game's RAM is kept aside meanwhile. |
| `DoHiLights` | `penalty94.asm:DoHiLights` | The highlights: each game of the round (GetShifter, last first) whose highlight flag is set (gsfhl, cleared here), unless the series is over (gsfso). |
| `Intermission` | `penalty94.asm:Intermission` | Between periods (and after the game, which first updates the records): the zamboni crosses under the period's menu (the playoff and shootout lists in their modes), the ticker shows the other games' scores, then the highlights; start cuts it short. |

### `game/play/period.c` (21)

| Function | Original | What it does |
|---|---|---|
| `EndShootout` | `period94.asm:EndShootout` | Shootout over: freeze the window, then the 5 skaters of the winning side are set up above or below the view and celebrate (assscore). |
| `ClampPositionBox` | `period94.asm:ClampPositionBox` | Unreferenced: clamp temp3 / temp4 of a3 to the box of his position. |
| `ClearGameStats` | `period94.asm:ClearGameStats` | Clear the shootout state, both team structs, PenBuf and the game flags. |
| `AddArenaAnimSprite` | `period94.asm:AddArenaAnimSprite` | Add the frame arenaframe of the arena animation to the sprite table (64 sprites at most) for the pieces inside the view. |
| `PrintPlayerNameRight` | `period94.asm:PrintPlayerNameRight` | The name of player d0 of team a2, moved left so it ends before column $29 (a trailing pad byte or two not counted), then the hot / cold icon. |
| `SkipNameNumber` | `period94.asm:SkipNameNumber` | Unreferenced: the "NN F. |
| `SetStatScroll` | `period94.asm:SetStatScroll` | Plane A vertical scroll = VertLineScrolling - $50, with disflags bit 2 set while it writes. |
| `PrintStatScrollArrows` | `period94.asm:PrintStatScrollArrows` | The scroll arrows of the Game Statistics screen. |
| `DisplayTeamStatsScreen` | `period94.asm:DisplayTeamStatsScreen` | The team logos, then each stat row (without the power play rows when penalties are off) centred, with the home value at x $22 and the visitors' at x 9. |
| `PrintStatValue` | `period94.asm:PrintStatValue` | Print the stat row a0 (two team struct offsets) for team a2 centred at printx: the value (offsets $A and $352 are times), "/second" if any, " (pct%)" for passing; offset $FFFF is the shooting percentage. |
| `updatePPTeamTime` | `period94.asm:updatePPTeamTime` | One more second of power play time for the team on the power play. |
| `GetTeamRating` | `period94.asm:GetTeamRating` | The TeamRatings byte of team $28(a2). |
| `ShortenMsgTimer` | `period94.asm:ShortenMsgTimer` | Cap msgtimer at 2, unless a second pad is on and a3 is not the puck carrier. |
| `team_logo` | (a helper) | PeriodStatsScreen: team's logo bitmap at printx / printy, its colours as palette 2 -> the next char |
| `period_num` | (a helper) | PeriodStatsScreen's .num: team a2's total of period k (goals, or shots in the matchup), added to matchuptimer, in 2 or 3 columns |
| `period_team` | (a helper) | PeriodStatsScreen's .team: team a2's goals (or shots, matchup) by period, then the total |
| `period_title` | (a helper) | PeriodStatsScreen's .title and .lines |
| `period_lines` | (a helper) |  |
| `PeriodStatsScreen` | `period94.asm:PeriodStatsScreen` | The "Period Stats" menu item: both team logos, then each team's goals (or shots) by period with the overtime column when there was one, and the total. |
| `stats_scroll` | (a helper) | GameStatisticsScreen's .scroll: move the rows by PlayerScrollCtr within 0..matchupvis, stopping at a row boundary; at the boundaries print the row that scrolls in |
| `GameStatisticsScreen` | `period94.asm:GameStatisticsScreen` | The "Game Statistics" menu item: both teams' totals (DisplayTeamStatsScreen) under the team blocks; up / down scroll the rows (to $70, or $30 without penalties), start leaves. |

### `game/play/shootout.c` (16)

| Function | Original | What it does |
|---|---|---|
| `ShootoutModeBox` | `shootout94.asm:ShootoutModeBox` | "SHOOTOUT MODE": the shooter vs. |
| `NextShooter` | `shootout94.asm:NextShooter` | The next shootout shooter: BA_Team, the goalie he faces, his roster slot from the team's shooter list, and a new skate path (for a3). |
| `CountShootoutGoals` | `shootout94.asm:CountShootoutGoals` | After each shot: once both teams have shot in round 5 or later with the goals not level, the shootout ends (gmode2 bit 3, EndShootout); else the other team shoots, and after both, the next shooter and round. |
| `ShootoutWonBy` | `shootout94.asm:ShootoutWonBy` | "SHOOTOUT WON BY" and the winning team's name, in the big font. |
| `InitShooters` | `shootout94.asm:InitShooters` | The six starters of the home team (shootoutteam 0: the list below homeshootnum) or the away team (below shootoutteam), roster index - 1 each. |
| `ClearShootout` | `shootout94.asm:ClearShootout` | Clear the player structs ($800 bytes from SortCords) and the shootout state: round 1, no goals, both shooter lists. |
| `ShootoutShooters` | `shootout94.asm:ShootoutShooters` | Shootout Shooters for team a2 (a menu item, in a shootout only): the 5 shooter slots and the goalie's; up / down a shooter slot, left / right the first shooter or the goalie, C a player for the slot from a list (ShooterSelectList), Start leaves. |
| `ShooterSelectList` | `shootout94.asm:ShooterSelectList` | C on slot TestList: "{Select Player}", the team's skaters (the goalies for the goalie slot) in Satt, the slot's player highlighted ($C000); up / down the player, left / right the attribute column, C takes him, Start goes back. |
| `attrib_column` | (a helper) | the attribute column DispAttribCtr of a column list (each a String and a long): past the end DispAttribCtr goes back one |
| `PrintShooterList` | `shootout94.asm:PrintShooterList` | The column header (the overall column with the shooters' own mask: ShooterOverallTxt / ShooterOverallTxt2), then six rows of Satt from PlayerScrollCtr: name and attribute (GetNameandAttrib), row VertLineScrolling highlighted. |
| `ShootersBackground` | `shootout94.asm:ShootersBackground` | The screen cleared (ClearShooterScreen), the background (ScoutMap, no tiles), the "Shootout" title box and team a2's block. |
| `ClearShooterScreen` | `shootout94.asm:ClearShooterScreen` | Plane B's top $A rows erased. |
| `ResetShooterScroll` | `shootout94.asm:ResetShooterScroll` | The list scroll words (palfadenew + $5A, + $7A) cleared. |
| `PrintShooterSlots` | `shootout94.asm:PrintShooterSlots` | "Shooters", slots 1. |
| `PrintShooterBox` | `shootout94.asm:PrintShooterBox` | The box with the full name of slot TestList's player (getname), centred on column $15. |
| `PrintShooterNames` | `shootout94.asm:PrintShooterNames` | The six slots' last names (FormatPlayerNameShort) from row $E, two rows apart, the goalie's at ($19, $E); slot TestList in font set 2. |

## Screen: the picture (video, sprites, the camera, replays, the crowd, cards, title and attract)

### `game/screen/attract.c` (5)

| Function | Original | What it does |
|---|---|---|
| `EASportsScreen` | `attract94.asm:EASportsScreen` | The publisher's logo screen, faded in with the fades-only vblank, until a button or $50 x 4 frames (about 5 s). |
| `UpdatePlayoffLevel` | `attract94.asm:UpdatePlayoffLevel` | In the new playoffs: pojoy = 7 (or $B with FourWayPlay) - OptNOP. |
| `DrawMatchupBitmaps` | `attract94.asm:DrawMatchupBitmaps` | The visitors' and the home team's bitmaps (DrawTeamBitmap) with their colours from TeamPalettes (the menu's teams in Regular Season and Shootout), then the team structs (setteams) and the palette at once. |
| `DrawTeamBitmap` | `attract94.asm:DrawTeamBitmap` | Rows d1 * 2 and the next of TeamBitmaps' map (no tiles) at printx / printy from char d4 -> d4. |
| `VBlank_SetOptions` | `attract94.asm:VBlank_SetOptions` | The setup screen's vblank: unless dfng, the sprites (when dfok) and the fade; then vcount and the music. |

### `game/screen/cards.c` (56)

| Function | Original | What it does |
|---|---|---|
| `wallcollduringcheck` | `cards94.asm:wallcollduringcheck` | During a check, skater a2 (not a goalie) is tested against the corners and walls at his position with his size + $17. |
| `CountGoalies` | `cards94.asm:CountGoalies` | homegoalies / awaygoalies = the goalies of each team. |
| `CrowdRecordIO` | `cards94.asm:CrowdRecordIO` | Team d1's 16-byte crowd record (CrowdRecBase + team * 16) in save RAM from / to a0: written when sflags6 bit 6 is set, else read. |
| `ReadCrowdRecord` | `cards94.asm:ReadCrowdRecord` | Read team d1's crowd record to a0. |
| `setSlotBit` | `cards94.asm:setSlotBit` | sflags6 bit 5 (the slot) = the puck carrier is in the slot: in his attacking zone (y past $58 toward the net he shoots at) within $47 of the middle. |
| `CountPlayers` | `cards94.asm:CountPlayers` | homeplayers / awayplayers = the players of each team. |
| `PlayerRecordIO` | `cards94.asm:PlayerRecordIO` | The 4-byte record of roster slot d0 of team d1 (PlayerRecordOffsets) in save RAM from / to a0: written when sflags6 bit 6 is set. |
| `TeamRecordIO` | `cards94.asm:TeamRecordIO` | Team (or user) d1's 16-byte record (TeamRecBase + d1 * 16) from to a0. |
| `UpdatePlayerRecords` | `cards94.asm:UpdatePlayerRecords` | After a game, team a2 (number d1, d5 goalies, opponent d6): each roster slot's record keeps the best game (goals, or saves for the goalies) with the user names and the opponent. |
| `UpdateTeamRecord` | `cards94.asm:UpdateTeamRecord` | After a game: record d1 (a user's) of team a1 against a2 (team numbers d3 / d4, the other user d2): count the game (up to 9000), the win or the tie, and on a win keep the most goals and the biggest margin (as the 68k computes it: word 0 of a2 minus a2's goals) with who and against whom. |
| `UpdateCrowdRecord` | `cards94.asm:UpdateCrowdRecord` | Team a1 (number d1, d6 goalies, opponent d2, users d4 / d5): the crowd record keeps the most goals, the most saves by one goalie (worked out in the high byte of TempWord1) and, for the home team, the loudest crowd (CrowdPeak); written when anything changed. |
| `UpdateRecords` | `cards94.asm:UpdateRecords` | After a game, with save RAM and user records on: the player, crowd and team records of both teams (ThreeStars is the buffer), then the save RAM checksum. |
| `ResetCardTimer` | `cards94.asm:ResetCardTimer` | The player card timer to $AA ($53 when C was pressed). |
| `PlayerCardTimer` | `cards94.asm:PlayerCardTimer` | Each frame of the setup screen's first minute: once carddelay has run out a card is up (bit 0); count cardtimer down (C skips $50 of it); at $52 the next card, on the other side, with both blocks and logos forgotten (PlayerCardScreen); below 0 the timer starts again. |
| `print_setup` | (a helper) | printsmall in the setup font (sflags6 bit 3) |
| `PlayerCardScreen` | `cards94.asm:PlayerCardScreen` | A player card on one side (bit 1: the visitors' on the left, else the home team's on the right): the team block erased, a logo box with the picture of the team's featured player (FeaturedPictures; the next one after each visitors' card), a framed box with his number and name and, with save RAM, his user records. |
| `PrintRecordValue` | `cards94.asm:PrintRecordValue` | The record line of player d1 of team d0 at a1: "Record " (a copy of RecordTxt), the value (GetRecordValue) and goals / goal or saves / save; an empty String when there is no value. |
| `CopyString_p` | (a helper) |  |
| `AppendNumber` | `cards94.asm:AppendNumber` | d0 (0-999, no leading zeros) as a String at a1, padded to an even length. |
| `GetRecordValue` | `cards94.asm:GetRecordValue` | Player d1 of team d0's record from save RAM (ReadPlayerRecord to $FFFF0000) -> d0 = its value byte, d1 = 1 when he is one of the team's goalies (the first entries, as many as the nibbles of its .sodds word). |
| `PrintRecordHolder` | `cards94.asm:PrintRecordHolder` | "by " (RecByTxt) and the record holder's user name (AppendRecordHolder) at a1; an empty String when there is none. |
| `PrintRecordVs` | `cards94.asm:PrintRecordVs` | "vs. |
| `player_record` | (a helper) | the player record a0, or (a0 = 0) player d1 of team d0's read to $FFFF0000 |
| `AppendRecordHolder` | `cards94.asm:AppendRecordHolder` | The user name of the record holder (byte 1 of record a0) as a String at a1 (AppendUserName, trimmed). |
| `AppendRecordVs` | `cards94.asm:AppendRecordVs` | The user name (byte 3 of record a0), a space and the opponent's city (byte 2, AppendTeamName) as a String at a1. |
| `WriteNameLog` | `cards94.asm:WriteNameLog` | The user name log to save RAM (NameLogIO). |
| `ReadNameLog` | `cards94.asm:ReadNameLog` | The user name log from save RAM to namelog (NameLogIO). |
| `NameLogIO` | `cards94.asm:NameLogIO` | Read (sflags6 bit 6 clear) or write the name log: NameLogBytes at save RAM NameLogBase. |
| `AppendUserName` | `cards94.asm:AppendUserName` | User d2's name (12 bytes of namelog, a 0 as a space) as a String at a1, trimmed (TrimSpaces) with sflags6 bit 7; user 0: an empty String (NoNameTxt). |
| `AppendTeamName` | `cards94.asm:AppendTeamName` | Append the abbreviation of team d0 (its low byte) to a1. |
| `TrimSpaces` | `cards94.asm:TrimSpaces` | Drop the trailing spaces of String a2 (zeroed) and pad its length to even. |
| `DrawPlayerPicture` | `cards94.asm:DrawPlayerPicture` | -> a0 = the picture of player d0 of team d1: his from the team's FeaturedPictures list, else a stock one (NoPicGoalie1 / 2, NoPicSkater1 / 2 by the bit 0 of his fifth attribute byte; the first as many players as the team's .sodds word has nibbles are its goalies). |
| `CalcAttrib` | `cards94.asm:CalcAttrib` | The rating of player d0 of team a2 for the attributes set in the high word of d4 (bit 15 down to 0, a nibble each, from the end of his 8 attribute bytes back): each nibble times its weight / 2 (OvrPlayerWgtList / OvrGoalWgtList for the overall masks, else AttribWgtList; the Wgt and Fight nibbles as they are). |
| `PlayerCards` | `cards94.asm:PlayerCards` | The Player Cards (a menu item, with save RAM only): 40 columns, the card screen picture, the corner logo and the arena, the card orders (BuildCardPlayerLists), then the cards of the team of the pad that paused (DrawPlayerCard): left / right and C page, A the other team, Start leaves. |
| `WrapCardIndex` | `cards94.asm:WrapCardIndex` | Card index (a0) wrapped into 0 ... |
| `DrawPlayerCard` | `cards94.asm:DrawPlayerCard` | The card of index (a0) in the team's colours: card 0 the team's records (PrintCardRecords); else the player's picture, his first and last names, his team, his records and this game's goals and assists or saves, his overall rating and his number and position. |
| `PrintCardRecords` | `cards94.asm:PrintCardRecords` | Card 0: both sides cleared to the logo, the team's name, its starting line and rating, and its records (the crowd record block): goals, saves and the crowd level. |
| `ClearCardText` | `cards94.asm:ClearCardText` | Erase a card side at (d0, d1): the team's logo bitmap (TeamLogoBitmaps, TeamLogoPalettes) drawn over it, palettes 1's second half kept. |
| `PrintCardRecordLine` | `cards94.asm:PrintCardRecordLine` | A record line at (d0, d1) when there is one (d2): the value and its label (TempMaxSpd), then the holder (d3) and " vs.", then the opponent's user (d4) and city (d5). |
| `PrintCrowdRecordLine` | `cards94.asm:PrintCrowdRecordLine` | The crowd level record line (CrowdLevelTxt). |
| `record_label` | (a helper) | the record label: "<n> Saves" / "Saves by" (with a holder), the singular after it for 1 |
| `PrintSaveRecordLine` | `cards94.asm:PrintSaveRecordLine` | The saves record line (CardSavesTxt / CardSavesByTxt). |
| `PrintGoalRecordLine` | `cards94.asm:PrintGoalRecordLine` | The goals record line (CardGoalsTxt / CardGoalsByTxt). |
| `PrintOverallRating` | `cards94.asm:PrintOverallRating` | "Overall Rating" at (d1, d2): player d0's (CalcAttrib with the goalie mask when matchup is set), x 100 / d1, adjusted (AttribAdjust). |
| `PrintStartingLine` | `cards94.asm:PrintStartingLine` | "Starting Line" at (d0, d1), then a row a position (LinePosNames): the team's starter there (its TeamList line at +6, FormatPlayerNameShort). |
| `PrintCardPosition` | `cards94.asm:PrintCardPosition` | At (d1, d2) player d0's number (GetJerseyString) and Goalie, Forward or Defenseman (his roster index against GetGoalieCount and GetDefenseStart). |
| `PrintTeamRating` | `cards94.asm:PrintTeamRating` | "Team Rating" at (d0, d1): the card team's (GetTeamRating), 2 wide. |
| `PrintCardTeam` | `cards94.asm:PrintCardTeam` | The card team's city and nickname centred on rows d1 and d1 + 1. |
| `PrintCentered_p` | (a helper) |  |
| `GetJerseyString` | `cards94.asm:GetJerseyString` | Player d0 of the card team: his number as a 2 digit String at a1 (a space for a leading 0). |
| `PrintGoalsAssists` | `cards94.asm:PrintGoalsAssists` | "Goals" and "Assists" at (d1, d2): player d0's this game ($B4 and $CE of the team struct). |
| `PrintSaves` | `cards94.asm:PrintSaves` | "Saves" and "Save %" at (d1, d2): goalie d0's shots ($E8 of the team struct) less his goals against ($B4), and that x 100 / the shots (0 with none). |
| `DrawPictureBox` | `cards94.asm:DrawPictureBox` | The card player's picture (DrawPlayerPicture; no map: the common one) unpacked and drawn at (1, 8), its palette's second half to palette 2. |
| `GetCardPlayer` | `cards94.asm:GetCardPlayer` | cardplayer = the roster index of card cardindex (from 1) of the card team's order (homecardlist / awaycardlist). |
| `BuildCardPlayerLists` | `cards94.asm:BuildCardPlayerLists` | Both teams' card orders. |
| `BuildCardPlayerList` | `cards94.asm:BuildCardPlayerList` | The card order of team a0 at a1: its 6 starters (its TeamList line at +6) in roster order, then the others from a1 + 6. |

### `game/screen/crowd.c` (16)

| Function | Original | What it does |
|---|---|---|
| `LoadCrowdRec` | `crowd94.asm:LoadCrowdRec` | CrowdRecord = the arena record of HomeTeam from save RAM, $50 when none. |
| `Crowd_Noise` | `crowd94.asm:Crowd_Noise` | The crowd meter each frame: crowd noise on / off from the excitement against the record, and every $14 ticks the meter reading and its peak. |
| `stopna2` | `crowd94.asm:stopna2` | Slow the velocity of a3 toward 0 by $7D0. |
| `checkcornercoll` | `crowd94.asm:checkcornercoll` | The corner circles (radius 64) and the side walls for object a3 at d2 / d3 with wcradiusx / wcradiusy; a hit goes to wallcollb. |
| `HotColdLoop` | `crowd94.asm:HotColdLoop` | The loop of Create_HotCold_Table from entry d1 down. |
| `Create_HotCold_Table` | `crowd94.asm:Create_HotCold_Table` | Fill the hot / cold table of team a0 with 416 random values (-9 ... |
| `Clamp0to100` | `crowd94.asm:Clamp0to100` | Unused. |
| `AttributeCalc` | `crowd94.asm:AttributeCalc` | Attribute d3 of player a3 * 5 plus his hot / cold value / 3, limited to 0 ... |
| `NextHomeHotPlayer` | (a helper) |  |
| `NextAwayHotPlayer` | (a helper) |  |
| `NextHomeColdPlayer` | (a helper) |  |
| `NextAwayColdPlayer` | (a helper) |  |
| `BuildHotColdLists` | `crowd94.asm:BuildHotColdLists` | The hot / cold player lists of both teams. |
| `SortHotColdStarters` | `crowd94.asm:SortHotColdStarters` | Sum the hot / cold values of the 6 starters of team a0 into TempBuffer (byte pairs: player, sum), then sort them by sum. |
| `CompareHotColdTotals` | `crowd94.asm:CompareHotColdTotals` | awayhotter = 1 when the away total is higher; d0 = -1, $22 or $23 by the difference. |
| `GetHotColdTotal` | `crowd94.asm:GetHotColdTotal` | The hot / cold total of the starters of team a0. |

### `game/screen/display.c` (32)

| Function | Original | What it does |
|---|---|---|
| `updatesound` | `display94.asm:updatesound` | Move the crowd noise volume (psg noise channel, asv) toward crowdlevel. |
| `KillCrowd` | `display94.asm:KillCrowd` | Silence the crowd noise (psg). |
| `dma_entry` | (a helper) | one DMA list entry |
| `sat_entry` | (a helper) | one hardware sprite: y, size / link, tile word, x |
| `VBlank` | `display94.asm:VBlank` | The main vblank code for game play: DumpSprites when the frame is ready (dfok), the palette fade, then the game clock (a second every $AAA jiffies, the shootout clock instead in a penalty shot / shootout, a beep at 60 seconds), vcount and the music. |
| `vb2` | `display94.asm:vb2` | The vblank used for palette fades only, no DMAs. |
| `DumpSprites` | `display94.asm:DumpSprites` | Transfer the scroll values, the sprite table and the DMA list. |
| `DumpSprites2` | `display94.asm:DumpSprites2` | The sprite table, then the DMA list. |
| `DoDMAList` | `display94.asm:DoDMAList` | Transfer every entry of the DMA list, last first. |
| `SetScroll2` | `display94.asm:SetScroll2` | Write Hscroll / Vscroll to the VDP. |
| `setvideo` | `display94.asm:setvideo` | Not vblank code: set up RAM for the vblank transfers. |
| `updatescroll` | `display94.asm:updatescroll` | Scroll from Hpos / Vpos; when the view moves to a new rink row queue the map rows that scrolled in (from Rinktilelist, or RevRinkTilelist in a reverse angle replay), up to $20 of them. |
| `showref` | `display94.asm:showref` | Queue the 8 rows of the ref window map (RefRamMap) when it changed. |
| `checkfo` | `display94.asm:checkfo` | The faceoff sprites: 3 entries of fofdata2 (word frame, word flags) from FaceOffSprites at fodropx / fodropy. |
| `showzam` | `display94.asm:showzam` | The zamboni: frame 1, frame 2-4 by x, then frame 5 (home not ahead) or a frame by x. |
| `SetSframe` | `display94.asm:SetSframe` | Draw one sprite frame: a0 = frame list, d0 / d1 = x / y, d2 = the frame, d3 = its start char in VRAM. |
| `crowd_frame` | (a helper) | one crowd frame (0 = none) from CrowdFrameList, clipped to the view; horizontal frames are 31 later |
| `showcrowd` | `display94.asm:showcrowd` | The crowd sprites: up to 3 frames per penalty box nibble, then the two crowdframe frames. |
| `clock_digits` | (a helper) | the ROM address of the '0'..'9' Strings inside showclock (a local label, not in the symbol table): the first "\0\4" "0" "\0" after the routine's entry |
| `clock_digit` | (a helper) | one clock digit of the horizontal rink |
| `clock_char` | (a helper) | one tile of the vertical clock, written backward from a0: the high word of the divu result |
| `showclock` | `display94.asm:showclock` | The game clock on screen. |
| `sso_arrow` | (a helper) | the off-screen arrow of one controlled player: a0 = the object, a3 = the sso entry, d3 = its first arrow frame |
| `checksso` | `display94.asm:checksso` | Arrows for the controlled players when they are off screen (pads 3 and 4 too on the four way play). |
| `setsortcords` | `display94.asm:setsortcords` | addframe for the 16 objects in OOlist order. |
| `setffo` | `display94.asm:setffo` | The 7 objects tied to the rink scrolling (the gloves and the pads), each moved by its x offset. |
| `uppads` | `display94.asm:uppads` | Update the gloves object and the 6 pad objects from the pad nibbles of PadControlBits / PadControlBits34 ($E = no change, $F = none), queueing new labels. |
| `QueuePadLabel` | `display94.asm:QueuePadLabel` | Queue the 3 character label of a pad object: two digits (bits 7-4 and 3-0, $F = blank) and a letter by bits 10-8 (none while sflags7 bit 7 is set). |
| `RenderSmallFontChar` | `display94.asm:RenderSmallFontChar` | DMA one small font tile to the object's chars: d2 = the character, d0 = the char slot. |
| `addframe` | `display94.asm:addframe` | Project object a3 with find3d, then addframe2. |
| `addframe2` | `display94.asm:addframe2` | d0 / d1 = the object's screen position. |
| `find3d` | `display94.asm:find3d` | Input d0 / d1 = field x / y, d2 = height. |

### `game/screen/replay.c` (21)

| Function | Original | What it does |
|---|---|---|
| `getpzjoy` | `replay94.asm:getpzjoy` | Read the joystick of the pad that paused: pads 3 and 4 on the four way play (pausepad 3 or 4), else pad 2 when sfpj is set, pad 1 otherwise. |
| `ShowReplayIcon` | `replay94.asm:ShowReplayIcon` | Show the replay icon (8 x 4 from the rink tiles) and start the $F0 frame replayicontimer. |
| `EraseReplayIcon` | `replay94.asm:EraseReplayIcon` | Erase the 8 x 4 replay icon. |
| `ShowReplayBanner` | `replay94.asm:ShowReplayBanner` | Show ReplayMap at 0,0 (16 x 11) and stop the icon timer. |
| `EraseReplayBanner` | `replay94.asm:EraseReplayBanner` | Erase the 16 x 11 area ShowReplayBanner drew. |
| `suba4` | `replay94.asm:suba4` | a4 = the current replay frame. |
| `adda4` | `replay94.asm:adda4` | Step forward one frame. |
| `adda42` | `replay94.asm:adda42` | adda4 without the look-ahead: stop at the record point (d7 = 0), else show the frame and step past it. |
| `adda43` | `replay94.asm:adda43` | a4 += $62, wrapping from replayend to M68K_RAM. |
| `UpdateCameraPos` | `replay94.asm:UpdateCameraPos` | Camera object a5 = object a3's x / y (negated for the reverse angle), Hpos / Vpos clamped to the rink. |
| `RevReplayAdj` | `replay94.asm:RevReplayAdj` | Flag a reverse angle frame conversion (sflags4 bit 7), then RestoreReplayFrame. |
| `RestoreReplayFrame` | `replay94.asm:RestoreReplayFrame` | Unpack replay frame a4 into the 16 objects and the other replay variables (the inverse of updatereplay). |
| `ClampReplayView` | `replay94.asm:ClampReplayView` | Clamp Hpos to -$3C..$3C and Vpos to -$C8..$100. |
| `updatereplay` | `replay94.asm:updatereplay` | Called every frame to save a replay frame at recbpr: the 16 objects (x, y, frame and palette bits packed in a long each), 6 x 3 bytes of roster numbers and positions, the puck heights, the last sound, the elapsed count, the pad bits and the view. |
| `hold_at_boards` | (a helper) | a player in a board / bench check animation is held at the boards |
| `tick_byte` | (a helper) | sub.b d7 from a byte timer, stopping at 0 |
| `updateplayers` | (a helper) |  |
| `updateanim` | `replay94.asm:updateanim` | Frame switch control on object a3: step SPAcnt by the elapsed frames, advance SPAnum through the animation (SPAlist entry SPA, frame list by facing direction) and pick the frame, with a 4 frame de-glitch minimum between frame switches. |
| `freezewindow` | `replay94.asm:freezewindow` | Lock the view at the current position. |
| `checkwindow` | `replay94.asm:checkwindow` | Move Hpos / Vpos toward the puck (or the puck carrier, leading toward the goal he attacks): the target yc1 / xc1 is followed with a 1/16 step when it leaves a dead zone around the view, clamped to the rink. |
| `ReplayMode` | `replay94.asm:ReplayMode` | The instant replay, from the pause menu. |

### `game/screen/scout.c` (17)

| Function | Original | What it does |
|---|---|---|
| `ScoutCrowdRecord` | `scout94.asm:ScoutCrowdRecord` | The text player's '@' -> a1 = the home team's crowd record in dB (save RAM team record byte 8; 80 when none) as a String at TempBuffer. |
| `ScoutingReport` | `scout94.asm:ScoutingReport` | The MATCHUPS screen before a game (with the fades-only vblank): the hot / cold players, the song, Ron Barr and his text (the paragraph list in TestList: hello, welcome, the teams' form, their hot / cold players, the crowd record, start), the matchups. |
| `PageMatchup` | `scout94.asm:PageMatchup` | The matchup by d0 (+1 / -1, wrapping in 0-6), the page timer again, the players, pictures and ratings. |
| `PrintMatchupRating` | `scout94.asm:PrintMatchupRating` | Team a0's rating (GetTeamRating) as 2 digits at printx / printy over two spaces -> d0 = the rating. |
| `PrintTwoSpaces` | `scout94.asm:PrintTwoSpaces` | Two spaces at printx / printy, printx kept. |
| `DrawMatchupPictures` | `scout94.asm:DrawMatchupPictures` | The matchup's two 6 x 6 pictures, the visitors' at x 2, the home team's at x $20, y $F: for matchup 0 the team logos, else the players' pictures. |
| `DrawMatchupPicture` | `scout94.asm:DrawMatchupPicture` | Player picture a0 (UnpackPicture) at printx / printy with the PicturePalette palette (and its map when the picture has none). |
| `DrawMatchupLogo` | `scout94.asm:DrawMatchupLogo` | Team logo a0 at printx / printy, the colours of team d3 / 4 (from GetTeamLogo) in colour family 1 (d5 = 2) or 2. |
| `DrawMatchupBitmap` | `scout94.asm:DrawMatchupBitmap` | A 6 x 6 dobitmap, then the 8 x 3 name area below it erased (at y $19, x 2 or $20). |
| `GetTeamLogo` | `scout94.asm:GetTeamLogo` | d3 = team -> a0 = its logo bitmap, d3 = team x 4. |
| `player_rating` | (a helper) | the rating of player d0 of team a2 (CalcAttrib with the overall weights of a skater or, in the goalie matchup, a goalie) as a percentage, adjusted (AttribAdjust) |
| `PrintMatchupRatings` | `scout94.asm:PrintMatchupRatings` | Matchup 0: 'Overall' and the teams' ratings (home at x $22, visitors at x 4, y $16); 1-6: the position, the two players' names on row $17 (the home player's right justified) and their ratings. |
| `GetMatchupPlayers` | `scout94.asm:GetMatchupPlayers` | matchuphome / matchupvis = the two teams' players of the matchup. |
| `GetMatchupPlayer` | `scout94.asm:GetMatchupPlayer` | -> d0 = the roster index of team a0's player for the matchup: slot MatchupLineSlots[matchup] (matchupslot; 0 goalie, 1 / 2 defense, 3 / 5 wings, 4 center) of the first of its line sets. |
| `RestartAdvantageMarks` | `scout94.asm:RestartAdvantageMarks` | The advantage marks from nothing again. |
| `PrintAdvantageMarks` | `scout94.asm:PrintAdvantageMarks` | Each frame: the marks at x $13, y $16, ']' growing toward the home team when its rating is higher, '[' toward the visitors when lower (one more every 7 frames, up to 3), blank when even. |
| `GetTeamNickname` | `scout94.asm:GetTeamNickname` | The text player's '=' (a2 the home team) and '*' -> a1 = the team's nickname, or 'Home' / 'Visitors' when it has none (the all-star teams). |

### `game/screen/title.c` (38)

| Function | Original | What it does |
|---|---|---|
| `ChooseSong` | `title94.asm:ChooseSong` | SongNum = song SongIndex of the six of team HmTeam (TeamSongs), or one of the eight RandomSongs when that is 0; none ($FFFF) with gmode bit 4. |
| `EndOneTimer` | `title94.asm:EndOneTimer` | End player a3's one-timer: flags cleared, onetimerplayer = none, glide, then assexit (goalie) or Setplass, and restart the animation. |
| `ReadGoaliePulled` | `title94.asm:ReadGoaliePulled` | -> the flags of tst.w on the goalie word of a3's team (Z: the goalie is pulled). |
| `ShootoutShootCheck` | `title94.asm:ShootoutShootCheck` | Shootout skater a3 with the puck -> Z: hold the shot (before shootoutclock 4, before the path starts, then until the path end is near or the puck has stopped after $F). |
| `SkatePath` | `title94.asm:SkatePath` | Shootout skate path: d0 / d1 = the current point (sopathx / sopathy, mirrored for the bottom net in the test); within $A of it (or $12 while standing) go on to the next one. |
| `NextPathPoint` | `title94.asm:NextPathPoint` | Step the shootout path sopath to its next point (x turned for a left-handed skater a3); at the end marker ($80) set sflags7 bit 3, sopathend and sopathdir. |
| `StartShootoutPath` | `title94.asm:StartShootoutPath` | Pick one of the seven shootout skate paths at random and start it (NextPathPoint, for skater a3). |
| `RandomFaceoffAnim` | `title94.asm:RandomFaceoffAnim` | -> d0 = a random faceoff animation (FaceoffAnims), 6 when the faceoff spot is at fox <= -$40, foy <= $C0. |
| `LeadSong` | `title94.asm:LeadSong` | Outside a shootout, when one team leads: choose the lead song (SongIndex 1 when the home team leads, 4 when the away team does) and flag it in sflags8 bit 6. |
| `chgplayer` | `title94.asm:chgplayer` | Pad d4 takes the free skater of its team nearest to where the puck is going (not a goalie, not unavailable, locked or on the other pad, and in a penalty shot or shootout only the shooter or the goalie). |
| `CountButtonPress` | `title94.asm:CountButtonPress` | Count a button press of pad d4 (homepresses / awaypresses), and in the next long the presses with a direction other than 8 or a3's facing. |
| `PSandSOpassdir` | `title94.asm:PSandSOpassdir` | Penalty shot / shootout: passdir = the direction at the end of the skate path, turned (passdirlist) for the bottom net. |
| `LoadHomeTeamGfx` | `title94.asm:LoadHomeTeamGfx` | d4 = char: load the home team's graphics (TeamGfxList) at d4 - $18 -> d4 = the char after them. |
| `HotColdIcon` | `title94.asm:HotColdIcon` | Team a2: the icon by the name of iconplayer (x 2 away, $20 home, y $19): the hot icon for the team's hot player, the cold one for its cold player, else clear it. |
| `clrTmPdst` | `title94.asm:clrTmPdst` | Team a0: all 26 roster slots on the bench (-2), then the end word -1. |
| `ClearPenalties` | `title94.asm:ClearPenalties` | No penalties: the counters, PenBuf and both teams' slots. |
| `PlaceBoardFall` | `title94.asm:PlaceBoardFall` | Player a3 falling into the boards (or over the bench): move him half way to the boards for the animation SPA. |
| `PrintParenNumber` | (a helper) | PrintPlayerGoals / PrintPlayerAssists: " (n)" with byte d0 of the team table, then the next row at x $E |
| `PrintPlayerGoals` | `title94.asm:PrintPlayerGoals` | d0 = player, a2 = team: " (goals)", next row. |
| `PrintPlayerAssists` | `title94.asm:PrintPlayerAssists` | d0 = player, a2 = team: " (assists)", next row. |
| `ReadLineData` | `title94.asm:ReadLineData` | Read the $100 bytes at save RAM LineDataBase to databuffer; sflags bit 4 cleared, lastsfx = none, recbpr = M68K_RAM. |
| `picture` | (a helper) |  |
| `screen_layout` | (a helper) | the screens' VRAM layout: planes at $C000 / $A000, window $F000, sprites $F800, hscroll $FC00 |
| `HiScoreScreen` | `title94.asm:HiScoreScreen` | The attract mode's second screen (a bitmap and HiScoreImg) with the fades-only vblank, for $50 x 4 frames or until a button. |
| `newTitleScreen` | `title94.asm:newTitleScreen` | The title screen (TitleScreenImg, the NHL shield, the NHLPA logo, the title) with its own vblank (VBlank_TitleScreen: a line scroll), the title song, then the credits (Credits, then CreditsList) scrolling up a row at a time until they end or Start. |
| `CreditsPrintRow` | `title94.asm:CreditsPrintRow` | Clear the six rows below the screen (Vscroll / 8 + $1C) and print the Strings from a1 there, one a row, each centred, until one that starts with a control code -> a1 at that one. |
| `CreditsWait` | `title94.asm:CreditsWait` | Wait for the vblank, count clampcounter down by 2 (to 0) and read the pads -> Start pressed (the 68k then returns from its caller too). |
| `VBlank_TitleScreen` | `title94.asm:VBlank_TitleScreen` | The title's vblank: unless dfng, the line scroll table (at SortCords) and Vscroll, the sprites, the fade; then vcount, CreditsScrollStep and the music. |
| `CreditsLineScroll` | `title94.asm:CreditsLineScroll` | The title's line scroll table at SortCords: $51 lines of plane B from $FD80 (one less every other line), then $61 from $100 (4 more a line); the step every $50 frames by $14. |
| `CreditsScrollStep` | `title94.asm:CreditsScrollStep` | Every TempWord2 frames (then every frame) the lines of the line scroll table move: plane B's of the first $51 by + Hscroll until they wrap to 0, the next $66 by - Hscroll down to 0. |
| `UnpackPicture` | `title94.asm:UnpackPicture` | A picture a2 (a tile count, then 3 bytes a row of 8 pixels, 3 bits each) to 4-bit tiles at picturebuf (the count first), each pixel + 5. |
| `DrawPlayoffSprite` | `title94.asm:DrawPlayoffSprite` | The PlayoffSprite sprite at playoffspritex / playoffspritey in Satt (an empty entry when it is off screen), the list ended there (Sattsize). |
| `PlayedByText` | `title94.asm:PlayedByText` | User d0's name (GetLogName from the name log, as a String at nameentrybuf) in "(played by NAME)" at TempBuffer -> a1; user 0: PlayedByNone. |
| `AttribAdjust` | `title94.asm:AttribAdjust` | An attribute under $32 becomes d0 / 2 + $19. |
| `WriteLineData` | `title94.asm:WriteLineData` | databuffer back to save RAM at LineDataBase, the checksum; then as ReadLineData after. |
| `GetLeagueCrowdRecord` | `title94.asm:GetLeagueCrowdRecord` | d0 = the team (of NumTeams, the last first) with the loudest crowd record (byte 8 of ReadCrowdRecord's buffer at ThreeStars; 0 counts as $50). |
| `ClearWinRecords` | `title94.asm:ClearWinRecords` | The win records (bytes 8-$B of the 8 records of $10 at ThreeStars) cleared, the $80 bytes written back at TeamRecBase (WriteSRAM, MakeSRAMChecksum). |
| `GetTeamStruct` | `title94.asm:GetTeamStruct` | a2 = the home team struct, or the visitors' when team d2 is not its team ($28 of the struct). |

### `game/screen/video.c` (79)

| Function | Original | What it does |
|---|---|---|
| `set_nib` | (a helper) |  |
| `UnpackNibbles` | `video94.asm:UnpackNibbles` | a0 = packed data, d0 = count: unpack count 4-bit values (high nibble first) into words at nibblebuffer. |
| `WeightedRandomSelect` | `video94.asm:WeightedRandomSelect` | d0 = number of word weights at nibblebuffer -> d0 = an index picked with probability weight / total. |
| `remap` | `video94.asm:remap` | a0 = tile words, a1 = 8-byte colour table, d0 = words, d1 = VRAM address: write the tiles to VRAM with every pixel's colour index n replaced by nibble n of the table (even n the high nibble of byte n/2, odd n the low one). |
| `forcefade` | `video94.asm:forcefade` | Run the fade to palfadenew whatever the interrupts and disflags are doing: the fades-only vblank (vb2) steps it until palcount goes negative. |
| `fade_black` | (a helper) | forceblack / forceblack2: fade to black over palcount frames, keeping palfadenew |
| `cramfade` | `video94.asm:cramfade` | Vblank code: step colour RAM toward palfadenew. |
| `CopyPaletteToCRAM` | `video94.asm:CopyPaletteToCRAM` | Copy all 64 palfadenew colours to colour RAM, protected from the vblank, and end the fade (palcount negative). |
| `randomd0s` | `video94.asm:randomd0s` | d0 = range -> d0 = random number with -range <= d0 < range. |
| `randomd0` | `video94.asm:randomd0` | d0 = range -> d0 = random 0..range-1. |
| `sroot` | `video94.asm:sroot` | d0.l -> d0 = its square root. |
| `sfx` | `video94.asm:sfx` | Word on the stack: start sound effect n (remembered in lastsfx), none if negative. |
| `song` | `video94.asm:song` | Word on the stack: start song n, none if negative. |
| `wait_vcount` | (a helper) | the vblank interrupt advances vcount; the 68k spins on it (.0 cmp.w (vcount).w,d0 / beq.s .0) |
| `waitx` | `video94.asm:waitx` | d0 = frames: wait that many vblanks or until a button is pressed on any pad (pads 3 and 4 in four way play). |
| `waitxsr` | `video94.asm:waitxsr` | waitx for the zamboni crossing: d0 = frames. |
| `waitjoy` | `video94.asm:waitjoy` | Wait a frame at a time until a button is pressed on any pad -> d1 = the new presses. |
| `orjoy` | `video94.asm:orjoy` | -> d1 = the new presses of pads 1 and 2 (all four in four way play). |
| `nodiag` | `video94.asm:nodiag` | d1 = new presses, d3 = held: when a diagonal is held, drop the new direction presses (d1 bits 0-3). |
| `ProcessInputWithRepeat` | `video94.asm:ProcessInputWithRepeat` | nodiag, then key repeat: holding the buttons d3 unchanged (d2 = 0) for 16 frames, then every 5th frame, presses them again (d1 = d3). |
| `ReadJoy` | `video94.asm:ReadJoy` | d0 = pad byte (active low) -> d0 = direction 0-8 (jdtab) in bits 0-3 and the buttons in bits 4-7, d1 = the buttons down (all 8 bits). |
| `read_pad` | (a helper) | ReadJoy1-4: read pad n -> d0 = direction and buttons, d1 = new presses, d2 = changed buttons, d3 = held buttons; ljn keeps the held buttons for the next read |
| `ReadJoy1` | `video94.asm:ReadJoy1` | Pad 1 (pad4way1). |
| `ReadJoy2` | `video94.asm:ReadJoy2` | Pad 2 (pad4way2). |
| `ReadJoy3` | `video94.asm:ReadJoy3` | Four way play pad 3 (pad4way3). |
| `ReadJoy4` | `video94.asm:ReadJoy4` | Four way play pad 4 (pad4way4). |
| `DoDMApro` | `video94.asm:DoDMApro` | DoDMA protected from the vblank (dfng). |
| `dma_68k` | (a helper) | DoDMA's .nd: one DMA transfer from 68k memory, with the Z80 off the bus. |
| `DoDMA` | `video94.asm:DoDMA` | a0 = source, d0 = words, d1 = VRAM address: DMA to VRAM. |
| `DoDMA_nd2` | `video94.asm:DoDMA_nd2` | d0 = words, d1 = VRAM destination, d2 = VRAM source: VRAM to VRAM copy by DMA, protected from the vblank. |
| `DoFill` | `video94.asm:DoFill` | d0 = words, d1 = VRAM address, d2 = word: fill VRAM through the data port, two words a pass (through dmaram). |
| `WaitDMA` | `video94.asm:WaitDMA` | Spin while the VDP status says a DMA is running. |
| `setVram` | `video94.asm:setVram` | d0 = colour: fade every colour to it, then setVram_0. |
| `setVram_0` | `video94.asm:setVram_0` | Clear VRAM and set the VDP registers from disflags (32 / 40 columns), Map1col1 (64 or 128 wide), VmMap1-3, VSPRITES and VSCRLPM; no window, whole-screen scroll. |
| `Vmaddr` | `video94.asm:Vmaddr` | d0 = VRAM address: set the VDP to write there (a0 = the data port). |
| `dobitmap` | `video94.asm:dobitmap` | a0 = palettes, a1 = map, a2 = tiles, d0/d1 = start x/y in the map, d2/d3 = width/height, d4 = first char, d5 = which of the four palettes to load (bits 0-3). |
| `DecompressGraphicsWithCallback` | `video94.asm:DecompressGraphicsWithCallback` | DecompressGraphics with the 8-byte colour table that follows the call (cb, its 68k address) as the remap table. |
| `DecompressGraphicsPlain` | `video94.asm:DecompressGraphicsPlain` | DecompressGraphics without a remap table. |
| `DecompressGraphics` | `video94.asm:DecompressGraphics` | a2 = graphics (a word count of tiles, bit 15 = packed, then the data), d4 = first char: load them at char d4 (DMA, remap through callbackPtr, or DecompressBytecode) -> d4 = next char. |
| `DecompressBytecode` | `video94.asm:DecompressBytecode` | a0 = packed tiles, d1 = VRAM address: run the byte code (one handler per high nibble, jump_table) into the ring buffer, which goes to VRAM each time it fills and at the end code. |
| `ring_put` | (a helper) | put one byte in the ring buffer; a full buffer goes to VRAM |
| `Opcode_CopyLiteral` | `video94.asm:Opcode_CopyLiteral` | Opcodes 0-1: copy (low 5 bits)+1 bytes from the data. |
| `Opcode_ClearBytes` | `video94.asm:Opcode_ClearBytes` | Opcode 2: (low 4 bits)+1 zero bytes. |
| `Opcode_Fillbytes` | `video94.asm:Opcode_Fillbytes` | Opcode 3: the next byte (low 4 bits)+3 times. |
| `CopyBackwardRun` | `video94.asm:CopyBackwardRun` | Copy count+1 bytes from dist back in the ring buffer, forward. |
| `CopyBackwardReverseRun` | `video94.asm:CopyBackwardReverseRun` | The same, walking the source backwards. |
| `Opcode_CopyBackwardShort` | `video94.asm:Opcode_CopyBackwardShort` | Opcodes 4-7: count bits 0-2, distance bits 3-5. |
| `Opcode_CopyBackwardMedium` | `video94.asm:Opcode_CopyBackwardMedium` | Opcode 8: count low 4 bits, distance the next byte. |
| `long_count` | (a helper) | opcodes 9 and F: count = low 4 bits and the top bit of the next byte (asl.b / roxl.b), distance = the next byte's low 7 bits + 1 |
| `Opcode_CopyBackwardLong` | `video94.asm:Opcode_CopyBackwardLong` | Opcode 9. |
| `Opcode_CopyBackwardExtended1` | `video94.asm:Opcode_CopyBackwardExtended1` | Opcode A: 6-bit count, 6-bit distance. |
| `Opcode_CopyBackwardExtended2` | `video94.asm:Opcode_CopyBackwardExtended2` | Opcode B: 7-bit count, 5-bit distance. |
| `Opcode_CopyBackwardReverseShort` | `video94.asm:Opcode_CopyBackwardReverseShort` | Opcodes C-D: count bits 0-1, distance bits 2-4. |
| `Opcode_CopyBackwardReverseMedium` | `video94.asm:Opcode_CopyBackwardReverseMedium` | Opcode E: count low 4 bits, distance the next byte. |
| `Opcode_CopyBackwardReverseLong` | `video94.asm:Opcode_CopyBackwardReverseLong` | Opcode F. |
| `FlushOutputBuffer` | `video94.asm:FlushOutputBuffer` | Send the ring buffer (all 256 bytes when it wrapped, else the idx bytes in it) to the VRAM address d3, through remap when callbackPtr is set, and advance d3. |
| `print_map` | (a helper) | the map selected by printm (a byte offset from VmMap1: 0, 4 or 8, though the 68000 takes any): its VRAM address and row shift |
| `xyVmMap` | `video94.asm:xyVmMap` | Set the VDP to write at printx/printy on the map printm. |
| `eraser` | `video94.asm:eraser` | d0/d1 = width/height, d2 = map word: fill the rectangle at printx/printy (printy advances, wrapping at 32 rows). |
| `frame_row` | (a helper) | Framer's .tbline: one row of the frame from the three-entry map row at framer word index col: left, w-2 middles, right |
| `Framer` | `video94.asm:Framer` | d0/d1 = width/height: draw a frame filled in (framer.map, its tiles at framercset) with the top left corner at printx/printy. |
| `font_char` | (a helper) | the font map a printed character comes from: setup font when sflags6 bit 3 |
| `printsmall_p` | `video94.asm:printsmall` | Print a String with control codes (negative bytes, ControlCodeJumpTable) at printx/printy/printm with printa; the char set is printfontset (smallfontchars, or setupfontchars when sflags6 bit 3). |
| `ControlCode_SetMapAndPosition` | `video94.asm:ControlCode_SetMapAndPosition` | Code -8: attribute, map, x, y. |
| `ControlCode_SetMap` | `video94.asm:ControlCode_SetMap` | Code -1: the next byte is the map (1-3) -> printm. |
| `ControlCode_SetAttribute` | `video94.asm:ControlCode_SetAttribute` | Code -2: the next byte is palette / priority -> printa. |
| `print_p` | `video94.asm:print` | Print a String at printx/printy/printm with printa. |
| `FormatAndPrintTime` | `video94.asm:FormatAndPrintTime` | d0 = period (bits 14-15) and seconds (bits 0-13): print the period label (" 1", " 2", " 3", "OT"), two columns on, the time as m:ss. |
| `push_string` | (a helper) | the String length word and padding that end PushTime / PushNumber: the text runs from p to end; a pad byte keeps the String even. |
| `PushTime` | `video94.asm:PushTime` | d0 = seconds -> a1 = a String " m:ss" (or "mm:ss") in mesarea. |
| `PushNumber` | `video94.asm:PushNumber` | d0 -> a1 = a String of it in decimal, no leading zeros. |
| `PushNumberWidth` | `video94.asm:PushNumberWidth` | d0 = number, d1 = width -> a1 = a String of it right-justified in width columns (spaces before the first digit) at PushWidthBuf. |
| `appstring_p` | `video94.asm:appstring` | a1 = String, a3 = String in RAM: append the first to the second (the text of a3 ends at its first zero byte after the first character). |
| `printbig_p` | `video94.asm:printbig` | Print a String in the big font (two rows high, chars one or two columns wide) at printx/printy; a negative byte starts a new position as in print. |
| `PrintBigChar` | `video94.asm:PrintBigChar` | d0 = ASCII: put its big font tiles (bfasciicon: map column, negative for a one-column char) at d4/d5, top and bottom, and advance d4. |
| `PutBigTile` | `video94.asm:PutBigTile` | d3 = tile: write it plus the pen attribute at d4/d5 on printm. |
| `AddSmallFont` | `video94.asm:AddSmallFont` | d4 = first char: load the small font tiles there -> d4 = next. |
| `AddFramer` | `video94.asm:AddFramer` | d4 = first char: load the framer tiles there -> d4 = next. |
| `AddTeamBlock` | `video94.asm:AddTeamBlock` | Load the team block tiles at char 2 -> d4 = next char. |

## Menus: setup, the options, records, stats

### `game/menus/menu.c` (8)

| Function | Original | What it does |
|---|---|---|
| `SetMenuPrintX` | `menu94.asm:SetMenuPrintX` | printx = the left edge of the menu box: 5 in 32 column mode, else 9. |
| `PrintMenuItem` | `menu94.asm:PrintMenuItem` | Print item a1 -> a1 past its text. |
| `UpdateMenuSelection` | `menu94.asm:UpdateMenuSelection` | Clamp the selection to the items, scroll the four rows shown, and print the menu: the title, the rows (the selected one marked), { and } when there are items above / below. |
| `DrawMenuScreen` | `menu94.asm:DrawMenuScreen` | Draw the screen behind the menu (menudraw), frame the menu box, print it and fade in. |
| `InitMenuState` | `menu94.asm:InitMenuState` | Start a menu: a0 = item list, a1 = screen draw routine, item 0 selected and shown first; falls into DrawMenuScreen -> d1. |
| `HandleMenuInput` | `menu94.asm:HandleMenuInput` | Act on the pad bits d1: down / up move the selection, C runs the selected item's handler (with a2 = the pause pad's team) and redraws the menu unless the handler drew its own screen (sflags5 bit 1). |
| `PutTeamBlock` | `menu94.asm:PutTeamBlock` | Two rows of 11 map words from TeamBlockMap + d0 at printx / printy; printx moves on 11. |
| `vcountwait` | `menu94.asm:vcountwait` | Wait until vcount moves off oldvcount, then put it back. |

### `game/menus/optsetup.c` (29)

| Function | Original | What it does |
|---|---|---|
| `GameSetUp` | `optsetup94.asm:GameSetUp` | The setup screen: the line data and the saved playoff state, the modes' start changes undone (SetupStart), the screen (setoptions), random teams after a demo, the playoff tree, then the menu until Start (SetupStart) or six minutes (SetupDemo). |
| `GoNewPlayoffs` | `optsetup94.asm:GoNewPlayoffs` | New playoffs: NewPO. |
| `GoContinuePlayoffs` | `optsetup94.asm:GoContinuePlayoffs` | Continue playoffs: ContinuePlayoffs. |
| `MoveMenuFrame` | `optsetup94.asm:MoveMenuFrame` | Move the frame from line d7 by d0 (+-2 a line): scroll the six lines shown (setupfirstline 0-3; setupprevline the one before, sflags5 bit 2 on a scroll), skip line 1-3 in continued playoffs and Team 2 in new playoffs, redraw the menu background under the old frame and draw the new one -> d7 = the new line. |
| `SetMenuArea` | `optsetup94.asm:SetMenuArea` | The menu area: printx $10, printy $E -> d0 = 23 x d1 = 13. |
| `SetFrameRect` | `optsetup94.asm:SetFrameRect` | The frame of line d3: at row $E + d3 - setupfirstline * 2, -> d0 = 23 x d1 = 3, or 5 for Team 1 in the new playoffs (a two-line frame). |
| `FixModeOptions` | `optsetup94.asm:FixModeOptions` | What a mode does not allow: Shootout and Demo turn User Records off, Demo sets both goalies to Auto Control; then the names after a scroll and the values (PrintOptions) in the setup font (sflags6 bit 3). |
| `ChangeOption` | `optsetup94.asm:ChangeOption` | Add d2 to option d7 (d2 = 0: only keep it in its range) and wrap it (WrapOption). |
| `WrapOption` | `optsetup94.asm:WrapOption` | d3 + d2 wrapped into d5 ... |
| `PrintOptions` | `optsetup94.asm:PrintOptions` | Keep each option in its range (ChangeOption, d2 = 0) and print the values of the lines shown, the scroll marks and the matchup bitmaps; unless a player card is up, restart the card timer. |
| `PrintOptionValue` | `optsetup94.asm:PrintOptionValue` | The value of option d7 at x $11 on its row (none past row $1B): the team names (PrintOptTeamName), 'N/A' for the period length of a shootout, the FourWayPlay list of Players. |
| `PrintScrollMarks` | `optsetup94.asm:PrintScrollMarks` | At x 2: '}' at row $19 when lines follow below, '{' at row $F when lines are above, else a space. |
| `PrintOptTeamName` | `optsetup94.asm:PrintOptTeamName` | A blank value, then team d3 of option d7 (its city; the full names of the two New York teams, known by number). |
| `PrintOptionNames` | `optsetup94.asm:PrintOptionNames` | The option names (OptionNames) at x 3, two rows apart from row $F - setupfirstline * 2; only rows $F-$19 are printed. |
| `setoptions` | `optsetup94.asm:setoptions` | Build the setup screen, with its own vblank (VBlank_SetOptions) and the graphics held back (dfng): the team bitmap tiles (LoadSetupTiles), the framer, small font and setup font tiles, the screen and menu bitmaps, the logo box tiles, the sprites. |
| `ReadSetupPads` | `optsetup94.asm:ReadSetupPads` | Each frame for up to $5460 frames (six minutes): the pads (four with FourWayPlay) with key repeat -> d1 = the first presses other than C (0 at the end). |
| `SetupDemo` | `optsetup94.asm:SetupDemo` | No press for six minutes: the demo (demoflag clear, a new seed, Regular Season, Demo, line changes and penalties on); falls into SetupStart. |
| `SetupStart` | `optsetup94.asm:SetupStart` | Start: the Goalies option to both teams, pojoy, a new seed; OptLine and OptPlayMode kept in TmpOptLine2 / TempOptPlayMode while Auto line changes play as On (sflags7 bit 4) and Shootout as Regular Season (gmode2 bit 0, ClearShootout); then the playoff tree (maketree). |
| `SetupTilesRemap` | (a helper) | the colour table LoadSetupTiles remaps the visitors' TeamBitmaps with (its 68k address) |
| `LoadSetupTiles` | `optsetup94.asm:LoadSetupTiles` | The TeamBitmaps tiles from char 2, then again remapped (teambitmapchars) -> d4 = the char after them. |
| `EraseCard` | `optsetup94.asm:EraseCard` | If a player card is up (cleared here, and both logos forgotten): erase its logo box (8 x 8) and its text box (25 x 8). |
| `DrawHomeBlock` | `optsetup94.asm:DrawHomeBlock` | The home team block: the card erased, the logo box and the HomeTeam logo (logoteam) at x $19. |
| `DrawVisBlock` | `optsetup94.asm:DrawVisBlock` | The same for the visitors: the logo box, the logo at x 9. |
| `DrawTeamLogo` | `optsetup94.asm:DrawTeamLogo` | The logo of team logoteam at printx, y 6 (6 x 6), with its colours in colour family 1 (home) or 2 (visitors), the rest of the palettes kept; then $B4 frames before a player card. |
| `LogoBoxRight` | `optsetup94.asm:LogoBoxRight` | The logo box at x $1C (logobox1chars). |
| `LogoBoxHome` | `optsetup94.asm:LogoBoxHome` | The logo box at x $18 (home block). |
| `LogoBoxLeft` | `optsetup94.asm:LogoBoxLeft` | The logo box at x 3 (logobox2chars). |
| `LogoBoxVis` | `optsetup94.asm:LogoBoxVis` | The logo box at x 8 (visitors block). |
| `DrawLogoBox` | `optsetup94.asm:DrawLogoBox` | The LogoBoxMap box (8 x 8) at printx, y 5 from chars d4. |

### `game/menus/records.c` (30)

| Function | Original | What it does |
|---|---|---|
| `UserNameEntry` | `records94.asm:UserNameEntry` | With user records on (OptUserRec 0): the name entry for pad 1's team, then pad 2's when it has the other. |
| `unframe_letter` | (a helper) | the letter grid cursor: erase the box at letter d5 (eraser with char 1) and print the letter over it without the frame |
| `NameEntryScreen` | `records94.asm:NameEntryScreen` | The NAME ENTRY screen for team nameentryvis (with the setup screen's vblank): its name and logo, the name log; up / down pick a name, B edits it in the letter grid (A / C move along the name, the pad moves over the letters, B back to the log), Start stores it (StoreUserName). |
| `NameLetterGrid` | `records94.asm:NameLetterGrid` | Toggle the letter grid (cursoron): on, the grid of letters and its help; off, the name back from the log, its area cleared and the log's help. |
| `NameEntryHelp` | `records94.asm:NameEntryHelp` | The name log's help text. |
| `PrintNamePrompt` | `records94.asm:PrintNamePrompt` | "Enter new name:" for an empty name log entry namelogsel, else "Select or replace:". |
| `SyncLetterCursor` | `records94.asm:SyncLetterCursor` | -> d5 = the grid index of the name's first letter (which is set to the grid's letter), 0 when it is not in the grid. |
| `FindLetter` | `records94.asm:FindLetter` | -> d0 = the index of letter d0 in LetterGrid (the first 31), $1E when not there. |
| `PrintNameCursor` | `records94.asm:PrintNameCursor` | The " < " cursor under letter d4 of the name (when the grid is on). |
| `PrintCurLetter` | `records94.asm:PrintCurLetter` | Grid letter d5 at printx / printy (when the grid is on). |
| `LetterGridPos` | `records94.asm:LetterGridPos` | printx / printy of grid letter d5 (6 a row, 2 cells apart) -> d0 d1 = its 3 x 3 box. |
| `PrintNameLog` | `records94.asm:PrintNameLog` | The seven names of the name log (GetLogName, PrintNameField). |
| `MoveLogArrows` | `records94.asm:MoveLogArrows` | When the selection namelogsel moved: the old ] [ arrows erased, the new ones printed (PrintLogArrows). |
| `PrintLogArrows` | `records94.asm:PrintLogArrows` | The two Strings at a1 on name log row namelogarrows (x 3 and $12); then namelogarrows = namelogsel. |
| `GetNameLength` | `records94.asm:GetNameLength` | namelength = the length of the name being entered (up to a '-' or 0, at most 12). |
| `PrintNameField` | `records94.asm:PrintNameField` | The 12-character name field from a1, '-' past namelength. |
| `NameEntryFramer` | `records94.asm:NameEntryFramer` | Framer with the name entry frame tiles (nameframechars). |
| `GetLogName` | `records94.asm:GetLogName` | Name log entry namelogsel to a1 ('-' for each 0), namelength = its letters. |
| `StoreUserName` | `records94.asm:StoreUserName` | Store the name entered ('-' as 0) as name log entry namelogsel when it changed: the log to save RAM (WriteNameLog), the records of whoever had that entry forgotten (WriteNameRecord); then this pad's user is the entry (0 for no name). |
| `WriteNameRecord` | `records94.asm:WriteNameRecord` | A name log entry replaced: user namelogsel goes from every save RAM record (the player records' holder and opponent user bytes, the crowd records', its team record cleared). |
| `SkipOtherUserName` | `records94.asm:SkipOtherUserName` | When namelogsel is the name the other pad picked, step past it in the direction d0 (0 counts as forward) -> whether it stepped (Z). |
| `RecordHoldersScreen` | `records94.asm:RecordHoldersScreen` | The Record Holders (a menu item): the corner logo and the arena, the user records from save RAM sorted three ways (ReadTeamRecords, CalcWinPercents); page 0 the win records (A + C held clears them: ClearWinRecords), pages 1 and 2 the goals and saves records, left / right; Start leaves. |
| `PrintRecordPage` | `records94.asm:PrintRecordPage` | Page TempWord1: 0 the win records, else the player records. |
| `PrintRecordTitles` | `records94.asm:PrintRecordTitles` | The column titles of page TempWord1 (WinRecTitles, GoalRecTitles, SaveRecTitles). |
| `PrintWinRecords` | `records94.asm:PrintWinRecords` | The 7 rows of the win records in winsort's order: the user's name (PrintRecordName), win %, wins, losses (games less wins and ties) and ties; a blank row (RecParenTxt) without games. |
| `ClearRecordArea` | `records94.asm:ClearRecordArea` | The record rows erased ($28 x $12 at row $A) with the caller's d2 as the tile (the pad's changed buttons: the 68k never loads it), printx / printy / printm kept. |
| `PrintPlayerRecords` | `records94.asm:PrintPlayerRecords` | The 7 rows of page 1 (goals: record bytes 0-2, recsort1) or 2 (saves: bytes 4-6, recsort2): the user's name, the record, "by" its team, "vs." the other; a blank row without one. |
| `PrintRecordName` | `records94.asm:PrintRecordName` | Row d7's number ("1. |
| `CalcWinPercents` | `records94.asm:CalcWinPercents` | For the 8 user records at ThreeStars: the win % byte (wins x 100 / games, 0 without games: winpcts), the games (wingames) and ties (winties); then rows 1-7 (winsort) bubble sorted on the win % (a signed byte), the ties, then the games, each descending. |
| `ReadTeamRecords` | `records94.asm:ReadTeamRecords` | The $80 bytes of user records from save RAM to ThreeStars, then rows 1-7 bubble sorted on record byte 0 (recsort1) or, with sflags6 bit 6, byte 4 (recsort2), descending as signed bytes. |

### `game/menus/setup.c` (35)

| Function | Original | What it does |
|---|---|---|
| `inline_table` | (a helper) | the remap table that follows a DecompressGraphicsWithCallback call in routine [from, to) |
| `PeriodOver` | `setup94.asm:PeriodOver` | The period is over: count it (gsp) and flip the ends; after the third period a tied game goes to overtime, after overtime only a tied playoff game goes on; else the game is over (gsp 4). |
| `reset_team` | (a helper) | resetplstuff's .top: team a2's skaters lose their flags and glide again |
| `resetplstuff` | `setup94.asm:resetplstuff` | Reset the team variables and the players of both teams. |
| `SprSort` | `setup94.asm:SprSort` | Sort the sort objects by y (x in horizontal mode): Ylist = the coordinates, OOlist = the order (byte offsets into Ylist), OOlistpos = each one's place. |
| `setupEASNmap` | `setup94.asm:setupEASNmap` | Load the EASN logo tiles at EASNcset, colours remapped -> d4. |
| `setupIceRinkPal` | `setup94.asm:setupIceRinkPal` | The ice rink palettes 0 and 1 (Rinktilelist) to palfadenew. |
| `setplayercolors` | `setup94.asm:setplayercolors` | Each team's colours (team data + its palette offset) to palfadenew palettes 2 (home) and 3 (visitors). |
| `IntermissionStart` | `setup94.asm:IntermissionStart` | Between periods: reset the clock, two vblanks with the sound off between, the end of period song, the ticker scores (and the playoff team stats at the end of the game), the intermission, then the next period or the game over. |
| `defaultsprites` | `setup94.asm:defaultsprites` | d4 = first char: the 4 screen objects (sso, 0x14 bytes) and the 7 objects tied to the rink scrolling (pads, 0x1C bytes) from their tables, each given its chars, then defaultsprites2 -> d4. |
| `defaultsprites2` | `setup94.asm:defaultsprites2` | The 16 sort objects (players, nets, puck, shadow) cleared and set from the table (position, frame, attribute, chars, radius, first assignment, flags), with the sort order lists, then SprSort -> d4. |
| `CopyTeamBlockMapData` | `setup94.asm:CopyTeamBlockMapData` | a0 = the TeamBlocks map, d0 = team, d1 = VRAM address, a1 = map word buffer: copy the team's 22 block chars from the loaded tiles to d1 on (VRAM to VRAM DMA) and store their map words (priority, palette of the map word, the char) -> a1. |
| `setupTeamBlocksMap` | `setup94.asm:setupTeamBlocksMap` | d4 = first char: the TeamBlocks tiles at d4 + $2C, then the home and visitor blocks copied to basetileoffset and basetileoffset + $16 -> d4 = basetileoffset + $2C. |
| `setupice` | `setup94.asm:setupice` | Set up the rink: the VDP layout (32 columns, maps at $C000 / $E000 $F000, sprites at $DC00, scroll at $FC00), clear VRAM, then load the rink, team, EASN, energy bar, crowd, sprite, framer, font and team block tiles, the default objects and palettes, flip the goals for the second period's ends, the pad and logo sprites, and the game vblank. |
| `InitTeamStructure` | `setup94.asm:InitTeamStructure` | Team struct a2 for team d0: the team number ($28), its data (tmdata, from TeamList) and its line sets at $16A (the layout by OptLine). |
| `InitTeamLines` | `setup94.asm:InitTeamStructure` | +$10. |
| `setteams` | `setup94.asm:setteams` | Both team structs from HomeTeam / VisTeam, with their sort objects. |
| `clearTeamStats` | `setup94.asm:clearTeamStats` | Clear both team structs, keeping the first $1A0 bytes of each hot / cold table (+$1A2, through homehotcoldsave / awayhotcoldsave) and the goalie words; the last roster slot's place is -1. |
| `setupice_highlight` | `setup94.asm:setupice_highlight` | Rebuild the rink objects for a highlight without reloading the tiles (the sprite chars setupice kept in spritechars), then the game vblank and a fade in. |
| `GameOver` | `setup94.asm:GameOver` | After the game: the playoff password (EncodePW), in the playoffs their stats and the playoff screen; then back to the title (ExitToOpening). |
| `ExitToOpening` | `setup94.asm:ExitToOpening` | The title song, then the options again (Opening2, with a new stack). |
| `Opening` | `setup94.asm:Opening` | The title screen, then into Opening2. |
| `Opening2` | `setup94.asm:Opening2` | With a new stack: clear the game's variables ($B000-$D03D), the options (GameSetUp), the name entry, then Opening3. |
| `Opening3` | `setup94.asm:Opening3` | The playoff screen (in the playoffs), the scouting report (not in a shootout), then the game (StartGame). |
| `PlayoffScreen` | `setup94.asm:PlayoffScreen` | In the playoffs: the tree (PlayoffTreeSetup by gamelevel: the team blocks, the arrows, the series scores), the round's title and the playoff sprite, with its own vblank (VBlank_Playoff); left / right scroll it a page ($70 pixels) at a time, Start leaves. |
| `HandlePlayoffInput` | `setup94.asm:HandlePlayoffInput` | Both pads: Start leaves the playoff screen (-> 1, the 68k's PlayoffScreenExit), right / left set the scroll step; then UpdatePlayoffScroll. |
| `UpdatePlayoffScroll` | `setup94.asm:UpdatePlayoffScroll` | The tree one step (PlayerScrollCtr) within its pages (up to gamelevel x $70 either way), stopping on a page boundary; the sprite's x while it shows. |
| `PlayoffScreen_waitvsync` | `setup94.asm:PlayoffScreen_waitvsync` | When the fade has run out, colour $21 flashes ($EE eor) and the fade starts again; then wait for the vblank. |
| `PrintSeriesScore` | `setup94.asm:PrintSeriesScore` | The best-of-7 wins "t-b" of game struct a2 at printx / printy. |
| `DrawTreeArrow` | `setup94.asm:DrawTreeArrow` | Column d0 of the arrows map (Arrowsmap, 2 x $17) at printx printy. |
| `DrawTeamBlocks` | `setup94.asm:DrawTeamBlocks` | Team block d1 (team x 2: its row of Teamblocksmap) at printx printy, highlighted (printa $6000) for the user's playoff team. |
| `VBlank_Playoff` | `setup94.asm:VBlank_Playoff` | The playoff screen's vblank: unless dfng, the sprites (and the tree's scroll, $FEA0 + DispAttribCtr) and the fade; then vcount and the music. |
| `InitScoutingDisplay` | `setup94.asm:InitScoutingDisplay` | Start the text player: no line yet, a new paragraph next -> a0 = the paragraph list (TestList). |
| `UpdateScoutingDisplay` | `setup94.asm:UpdateScoutingDisplay` | The text player: each time its delay (typedelay; longer after a comma or a full stop; none with asv set) runs out, the next word of the paragraph (PlayerScrollCtr: its offset in ScoutTextScript; negative: the next paragraph of TestList) to the text box, a line at a time (ScrollDisplayUp); the special characters put in the team data: the city of the team in form ($), the goalies ([ ]), the hot / cold players (< > \| \), the crowd record (@), the users' names (# %), nicknames (= *), arenas (^ ;) and the rosters' ends (_ + { }). |
| `ScrollDisplayUp` | `setup94.asm:ScrollDisplayUp` | The next line of the text box; past its seventh, the box's 8 rows one row up (VRAM to VRAM DMA). |

### `game/menus/stats.c` (75)

| Function | Original | What it does |
|---|---|---|
| `GetDefenseStart` | `stats94.asm:GetDefenseStart` | d0 = the roster index of team a2's first defenseman: its goalies (GetGoalieCount) and forwards (GetForwardCount). |
| `ReloadRinkGraphics` | `stats94.asm:ReloadRinkGraphics` | After a screen over the rink: back to 32 columns, then reload the rink (reversed in a reverse angle replay), team, font, framer, EASN, energy bar and crowd tiles and the palettes. |
| `SetupScreen` | `stats94.asm:SetupScreen` | The common start of the stats screens: fade out, 40 columns, the framer and small font tiles, the background (ScoutMap) once plain and once rows d0 to d1 with its palettes, a second small font, and plane A cleared -> d4 = the char after them. |
| `ExitStatsScreen` | `stats94.asm:ExitStatsScreen` | Leave a stats screen: fade out, then ReloadRinkGraphics. |
| `WaitVSyncAndReadInput` | `stats94.asm:WaitVSyncAndReadInput` | Wait for the next vblank and a new button press -> d1 (and the other pad words). |
| `PlayoffStatsScreen` | `stats94.asm:PlayoffStatsScreen` | Playoff Stats: the playoff totals (ReadTeamStats) of the user's playoff team, as PlayerStatsList with d7 = 1. |
| `PlayerStatsScreen` | `stats94.asm:PlayerStatsScreen` | Player Stats (a menu item): PlayerStatsList with d7 = 0. |
| `PlayerStatsList` | `stats94.asm:PlayerStatsList` | The stats of team a2's players, this game's (d7 = 0) or the playoffs' (1): sorted by the column chosen with left / right, scrolled with up / down; A shows the other team (game stats only), Start leaves (ExitStatsScreen). |
| `UpdateStatsListScroll` | `stats94.asm:UpdateStatsListScroll` | Each frame: the list scrolls by PlayerScrollCtr within 0 ... |
| `SetStatsListScrollReg` | `stats94.asm:SetStatsListScrollReg` | The list's vertical scroll: VSRAM word 1 = VertLineScrolling - $68. |
| `DrawStatsList` | `stats94.asm:DrawStatsList` | The column headers (the sort column highlighted; the goalie columns for DispAttribCtr -1), the sort column's title, then the players by that column (ties by lower number; goalies only for the goalie column) into Satt, ended by $FF, and the window's six rows. |
| `DrawStatsRow` | `stats94.asm:DrawStatsRow` | Row d3 of the sorted list (Satt): its rank, the player's name, then his five columns (the sort column highlighted) or, in the goalie column, his saves, shots and save %; nothing past the list's end. |
| `GetStatColumn` | `stats94.asm:GetStatColumn` | -> d2 = stat column d1 of player d0 (ReadStatColumn); a goalie has only columns 1 and 2, both his assists. |
| `ReadStatColumn` | `stats94.asm:ReadStatColumn` | -> d2 = stat column d1 of player d0: this game's from the team struct's byte arrays (StatColumnOffsets: one or two of them), the playoffs' (d7) from their RAM tables (GetStatsValue). |
| `GetStatsValue` | `stats94.asm:GetStatsValue` | d2 + word d0 of the playoff RAM table at StatsOffsetTbl2 + d1 (0: none). |
| `DrawStatsListArrows` | `stats94.asm:DrawStatsListArrows` | The up / down arrows (ScrollArrowTbl2): up when scrolled, down when more rows follow. |
| `ShowScores` | `stats94.asm:ShowScores` | The night's other games (gsstruct) over the Scores bitmap; up / down scroll, Start leaves (ExitStatsScreen). |
| `UpdateScoresScroll` | `stats94.asm:UpdateScoresScroll` | Each frame: the list scrolls by PlayerScrollCtr within 0 ... |
| `UpdateVertScrollReg` | `stats94.asm:UpdateVertScrollReg` | VSRAM word 1 = VertLineScrolling - $30. |
| `DisplayGameInfo` | `stats94.asm:DisplayGameInfo` | Game d3 of gsstruct (not the user's, gamenum, nor one with flag bit 1 or 2): its two teams and scores (PrintGameInfo) and its period (PerLabels). |
| `PrintGameInfo` | `stats94.asm:PrintGameInfo` | Team d0's city at x 6 and score d1 (2 digits) at x $18. |
| `DrawScrollArrows` | `stats94.asm:DrawScrollArrows` | Other Scores' up / down arrows (ScrollArrowTable: none, up, down; both reads past it, as on the cartridge). |
| `LineEditor` | `stats94.asm:LineEditor` | The Line Editor for team a2 (a menu item): the slot cursor, C picks a player for the slot (SelectLinePlayer), Start the exit menu (LineEditorExit). |
| `LineEditorRedraw` | `stats94.asm:LineEditorRedraw` | Clear the screen, the line icons to be drawn again; on to LineEditorMenu. |
| `LineEditorMenu` | `stats94.asm:LineEditorMenu` | The screen and the icons, then the slot cursor: Start -> the exit menu, C -> a player for the slot, the pad moves the cursor (LineCursorTable; only the first line's five slots with OptLine). |
| `SelectLinePlayer` | `stats94.asm:SelectLinePlayer` | The players who can take the slot (forwards, or the defensemen for slots 1-2) in Satt, the list at the slot's player; up / down choose, left / right page the attribute columns, C puts the choice in the slot (SetLineSlot), Start goes back. |
| `attrib_column` | (a helper) | the attribute column entry n of a column list (PAttribColumns / GAttribColumns: a header String, then its selector long), stepping DispAttribCtr back when the list is shorter |
| `PrintAttribHeader` | `stats94.asm:PrintAttribHeader` | The player list of the line editor: the header of attribute column DispAttribCtr, then six rows of names and that attribute, the chosen one highlighted. |
| `DrawLineIcons` | `stats94.asm:DrawLineIcons` | The lines drawn with the cursor's (LineIconGroups' masks; the area cleared when they change), then the cursor slot's player in a box. |
| `DrawLineIcon` | `stats94.asm:DrawLineIcon` | Line d5: its name (linelist) and " Line", then its players' last names, at LineIconPosTable (the entry before with OptLine); the cursor's slot in the bold font, its player and attribute kept (d6, d7). |
| `ClearLineArea` | `stats94.asm:ClearLineArea` | Erase the lines' area and print the position header. |
| `DrawTeamScreen` | `stats94.asm:DrawTeamScreen` | The line editor's background (ScoutMap), frame and title, and the team's logo map (PutTeamBlock). |
| `ClearLineEditorTop` | `stats94.asm:ClearLineEditorTop` | Erase 40 x 10 at the top of map 2. |
| `ClearMenuFlags` | `stats94.asm:ClearMenuFlags` | Colours $2D and $3D of palfadenew cleared. |
| `LineEditorExit` | `stats94.asm:LineEditorExit` | Start in the line editor: its menu (LineEditorMenuText, or LineEditorMenuNoLoad when databuffer holds another team's lines), the pause menu's state kept; its first item leaves (ExitStatsScreen), the others redraw the editor. |
| `SetLineSlot` | `stats94.asm:SetLineSlot` | Player d0 (1 based) to line slot d2 of team a2; when he is already in another slot of that line, the slot's old player goes there (a swap). |
| `DecodeTeamLines` | `stats94.asm:DecodeTeamLines` | Team a2's lines from databuffer (byte 0 the team + 1, then a nibble a slot in AttributeOffsetTbl order, relative to the first forward, or defenseman for slots 1-2). |
| `EncodeTeamLines` | `stats94.asm:EncodeTeamLines` | The reverse: team a2's lines packed into databuffer, saved (WriteLineData). |
| `TeamRosterScreen` | `stats94.asm:TeamRosterScreen` | Team a2's roster (a menu item): pages of players (the goalies, then each line: DisplayPlayerList), left / right the attribute column, up / down the page, A the other team, Start leaves. |
| `CheckPlayerListScroll` | `stats94.asm:CheckPlayerListScroll` | Each frame: VertLineScrolling + PlayerScrollCtr within 0 ... |
| `UpdatePlayerListScroll` | `stats94.asm:UpdatePlayerListScroll` | VertLineScrolling = d0; on a page boundary ($80) the scroll stops; leaving one, the page coming into view is drawn; VSRAM word 1 = VertLineScrolling - $70. |
| `StopPlayerListScroll` | `stats94.asm:StopPlayerListScroll` | Out of range: the scroll stops. |
| `DisplayPlayerListUp` | `stats94.asm:DisplayPlayerListUp` | Roster page d0 (the quotient in d0's high word). |
| `DisplayPlayerListDown` | `stats94.asm:DisplayPlayerListDown` | The page after it. |
| `DisplayPlayerList` | `stats94.asm:DisplayPlayerList` | Roster page SelectedPlayerIdx (0 the goalies, 1-7 the lines): its title (PlayerStatMenuTxt), the attribute column's header, then its rows: the position (GoalieRowText / PlayerPositionText), the name and the attribute (GetNameandAttrib). |
| `GetNameandAttrib` | `stats94.asm:GetNameandAttrib` | Player d0's name, then at x $1E the column d4's low word picks (attribjmp: status, energy, handedness, weight, fighting, rating); for those past energy his rating of the attributes in d4's high word (CalcAttrib) goes to it. |
| `AttribStatus` | `stats94.asm:AttribStatus` | Player d0's status word ($66 of the team struct): Ice, Bench, an injury, or his penalty time. |
| `AttribEnergy` | `stats94.asm:AttribEnergy` | Energy: his word at $32 of the team struct / 40, at most 100. |
| `AttribFighting` | `stats94.asm:AttribFighting` | Fighting: the nibble without its bit 0 (the handedness), a rating out of d1 - 1. |
| `AttribRating` | `stats94.asm:AttribRating` | d0 x 100 / d1, adjusted (AttribAdjust), printed (AttribPrintPct). |
| `AttribPrintPct` | `stats94.asm:AttribPrintPct` | d0, 4 wide, then 4 blanks. |
| `AttribHanded` | `stats94.asm:AttribHanded` | Bit 0 of the sum: Righty / Lefty. |
| `AttribWeight` | `stats94.asm:AttribWeight` | Weight: 140 + 8 x the rating, in lb. |
| `summary_input` | (a helper) | the summaries' input: Start leaves (-> 1), up / down set the scroll |
| `ScoringSummaryScreen` | `stats94.asm:ScoringSummaryScreen` | The Scoring Summary (a menu item): a 4 row entry a goal (ScoreSum), scrolled to the last, then with up / down; Start leaves. |
| `CheckScoringScroll` | `stats94.asm:CheckScoringScroll` | Each frame: VertLineScrolling + PlayerScrollCtr within 0 ... |
| `UpdateScoringScroll` | `stats94.asm:UpdateScoringScroll` | VertLineScrolling = d0; on an entry boundary (32 lines) the arrows and the scroll stops; leaving one, the entry coming into view is drawn; VSRAM word 1 = VertLineScrolling - $50. |
| `DrawScoringRowUp` | `stats94.asm:DrawScoringRowUp` | Goal entry d0 (the quotient) at the top of the window. |
| `DrawScoringRowDown` | `stats94.asm:DrawScoringRowDown` | Goal entry d0 + 4 at the bottom of the window. |
| `summary_team` | (a helper) | the abbreviation of the team of a summary entry (bit 7 of its byte 2: the visitors) at x $C |
| `DrawScoringRow` | `stats94.asm:DrawScoringRow` | ScoreSum entry d3: the time, the team, the goal type (GoalTypeTbl), the scorer and his two assists (PrintScorerName). |
| `PrintScorerName` | `stats94.asm:PrintScorerName` | Player d0 (a byte; negative: none) and down a row. |
| `PenaltySummaryScreen` | `stats94.asm:PenaltySummaryScreen` | The Penalty Summary (a menu item): a 3 row entry a penalty (PenSum), scrolled as the Scoring Summary. |
| `CheckPenaltyScroll` | `stats94.asm:CheckPenaltyScroll` | Each frame: VertLineScrolling + PlayerScrollCtr within 0 ... |
| `UpdatePenaltyScroll` | `stats94.asm:UpdatePenaltyScroll` | As UpdateScoringScroll with 24 line entries. |
| `DisplayPenaltyLineUp` | `stats94.asm:DisplayPenaltyLineUp` | Penalty entry d0 (the quotient) at the top of the window. |
| `DisplayPenaltyLineDown` | `stats94.asm:DisplayPenaltyLineDown` | Penalty entry d0 + 5 at the bottom of the window. |
| `DisplayPenaltyEntry` | `stats94.asm:DisplayPenaltyEntry` | PenSum entry d3: the time, the team, the minutes and the penalty's name (Penaltylist), the player. |
| `DrawScrollArrowsPenalty` | `stats94.asm:DrawScrollArrowsPenalty` | The summaries' up / down arrows (ScrollArrowTbl). |
| `CrowdMeterScreen` | `stats94.asm:CrowdMeterScreen` | The Crowd Meter (a menu item): the levels and records (PrintCrowdMeter) until Start. |
| `PrintCrowdMeter` | `stats94.asm:PrintCrowdMeter` | The current, average and peak crowd levels in dB; with save RAM the home arena's record and the league's (ReadCrowdRecord; 0 counts as 80 dB). |
| `PrintCrowdDB` | `stats94.asm:PrintCrowdDB` | Crowd level d0 as sqrt(d0 x 4) + 65 dB. |
| `TimeoutMenu` | `stats94.asm:TimeoutMenu` | Timeout for team a2 (a pause menu item): the menu now PauseText2 (no second timeout), the team's name, both teams rested (RestoreTeamEnergy), then $78 frames. |
| `SelectGoalieMenu` | `stats94.asm:SelectGoalieMenu` | Change Goalie for team a2 (a menu item): its goalies (or none) listed (DrawGoalieSelectList); up / down, then C or Start: the goalie ($26 of the team struct, SetPersonel). |
| `DrawGoalieSelectList` | `stats94.asm:DrawGoalieSelectList` | The goalie list from row $D: "no goalie", then each goalie's number and name, row menuitem highlighted. |

## Sound: the sound driver and the Z80 that runs it

### `game/sound/sound.c` (28)

| Function | Original | What it does |
|---|---|---|
| `take_bus` | (a helper) | take the Z80 bus and wait for it (move.w #$100,(IO_Z80BUS) / btst #0 / bne) |
| `local_table` | (a helper) | local tables of UpdateChannelFrequencyAndVolume and SetChannelVolume. |
| `fnum_table` | (a helper) |  |
| `bend_table` | (a helper) |  |
| `vel_table` | (a helper) |  |
| `ClearAllTrackAndSFXSlots` | `sound94.asm:ClearAllTrackAndSFXSlots` | Free the 8 track slots and reset the 6 channel structs (output channels 0, 1, 2, 4, 5, 6). |
| `p_turnoff` | `sound94.asm:p_turnoff` | Silence everything: free all slots, key off and mute every output channel, send the buffer and stop the PCM channel. |
| `play_sfx_or_music_track` | `sound94.asm:play_sfx_or_music_track` | d0 = sound 0-$7A ($30 and up are songs, which stop the current song first): start its event stream (SoundStreamTable) in the free track slot (the one with the lowest pointer) and reset that track's eight voices (volume $7F). |
| `stop_current_song` | `sound94.asm:stop_current_song` | Free the slot playing a song (pointer at or past the first song stream), key off and mute its channels and send the buffer. |
| `ResetZ80Bus` | `sound94.asm:ResetZ80Bus` | Reset the Z80 and wait for its bus. |
| `bus_for_pads` | (a helper) | request the bus and wait up to $65 polls, then reset the Z80 (the pad readers) |
| `ReadPad3Button` | `sound94.asm:ReadPad3Button` | Read the 3 button pad at port a0 (TH low, then high), release the bus -> d0 = the pad byte, active low: buttons in bits 4-7, the directions (PadDirTable drops opposite pairs) in bits 0-3. |
| `read_4way` | (a helper) | ReadPad4Way1-4: take the bus, select pad n of the 4 way adaptor on port 2, read it on port 1 |
| `ReadJoyData` | `sound94.asm:ReadJoyData` | Read the pads every vblank to pad4way1-4: four through the 4 way adaptor with FourWayPlay, else pads 1 and 2. |
| `p_music_vblank` | `sound94.asm:p_music_vblank` | Vblank: read the pads, clear the change bits, run the 8 track slots (track 7 first), send the buffer if anything changed and age the channels. |
| `z80_bus_release_delay` | `sound94.asm:z80_bus_release_delay` | The Z80 is busy: give the bus back and wait a little (then UploadCommandBufferToZ80 tries again). |
| `UploadCommandBufferToZ80` | `sound94.asm:UploadCommandBufferToZ80` | Once the Z80 is idle (Z80 RAM $97 = 0, $96 = $7D), mark it busy ($96 = $D1) and copy the 33-byte command buffer to Z80 RAM $02. |
| `ProcessOneMusicTrack` | `sound94.asm:ProcessOneMusicTrack` | Count down track slot a5 (track d7) and run every event that is due (4 bytes: delay, status, two data bytes; the delay of the next event follows). |
| `handle_command_00` | `sound94.asm:handle_command_00` | Event $0x: key off note +2 on midi channel +1 bits 3-0 of the track: the first channel struct playing that key and note that is on. |
| `ReleaseChannelAndNote` | `sound94.asm:ReleaseChannelAndNote` | Key off channel struct a2 if it is on; a PCM patch stops the PCM channel instead. |
| `StopPCMChannel` | `sound94.asm:StopPCMChannel` | Clear the PCM rate byte (Z80 RAM $8E): the PCM channel stops. |
| `handle_command_10` | `sound94.asm:handle_command_10` | Event $1x: key on note +2 at volume +3 (0 = key off) on midi channel +1 bits 3-0 of the track. |
| `UpdateChannelFrequencyAndVolume` | `sound94.asm:UpdateChannelFrequencyAndVolume` | The frequency of channel struct a2 from its note (octave and the note's F-number) and its key's pitch bend (a3 = voice table) scaled by the patch's bend range (12 for PCM). |
| `SetChannelVolume` | `sound94.asm:SetChannelVolume` | The volume of channel struct a2: note volume * the voice volume (a3 + 4) / 128. |
| `handle_command_40` | `sound94.asm:handle_command_40` | Event $4x: the patch of the key (voice table +3) = +2. |
| `handle_command_60` | `sound94.asm:handle_command_60` | Event $6x: the pitch bend of the key = word +2 - $2000, and a new frequency for every channel playing that key. |
| `handle_command_30` | `sound94.asm:handle_command_30` | Event $3x: controller +2 = +3. |
| `p_initialZ80` | `sound94.asm:p_initialZ80` | Free all slots, load the Z80 program ($295 bytes) into Z80 RAM, build 29 tables of 256 bytes below Z80 RAM $2000 ((x - $80) * 8 / n + $80 for n = 8-$24), then reset and start the Z80. |

### `game/sound/z80.c` (18)

| Function | Original | What it does |
|---|---|---|
| `z80_read` | (a helper) | the byte at Z80 address a: RAM below $2000, the ROM through the bank window from $8000 |
| `ym_port` | (a helper) | a YM2612 port write at the end of the instruction (0 / 1 part I's address / data, 2 / 3 part II's): a data write sets the busy flag |
| `step` | (a helper) |  |
| `patch_address` | (a helper) | $025B with its CALL: HL = the patch of channel e (32 bytes from fm_instrument_patches, bank 5) |
| `channel` | (a helper) | $0280 with its CALL: the channel in its part (IX to part II for channels 4-6) |
| `command` | (a helper) | $00D1: the command in $02-$06 |
| `init` | (a helper) | $0029: start (from the reset, or the restart when the 68k patches $96 to $29) |
| `routine_step` | (a helper) | the routine's next step (an instruction, or a store at the end of the one before) |
| `instruction` | (a helper) | the instruction at pc: its cycles |
| `pop` | (a helper) | ---- SegaInit.asm:z80_reg, the startup program the 68k loads first ------------------------ It clears the Z80's RAM from $26 on, zeroes the registers from it and loops at 0. |
| `startup_instruction` | (a helper) |  |
| `catch_up` | (a helper) | run the Z80 until it has caught up with the client's time |
| `z80_relay_run` | (a helper) |  |
| `restart` | (a helper) | the Z80 and the YM2612 out of reset (the chip's reset clears its busy flag). |
| `z80_relay_reset` | (a helper) |  |
| `z80_relay_busreq` | (a helper) |  |
| `z80_relay_power_on` | (a helper) |  |
| `z80_relay_seed` | (a helper) |  |

## System: power on, the 68000 jumps through RAM, RAM, the ROM, save RAM, 4 Way Play, data tables, the teams

### `game/system/checksum.c` (1)

| Function | Original | What it does |
|---|---|---|
| `ValidationRoutine` | `checksum94.asm:ValidationRoutine` | Add up every ROM long below this routine but the one holding the header's checksum ($18C); the wrong sum turns the screen red and hangs. |

### `game/system/data.c` (38)

| Function | Original | What it does |
|---|---|---|
| `box` | `data94.asm:box` | Clear a $13 x 8 rectangle at x $B, y 2 of map 1. |
| `ShowGoalBox` | `data94.asm:ShowGoalBox` | The goal box: close both line change boxes, frame it (more rows per assist), "GOAL!" ("PP GOAL!" after a delayed penalty, "HAT TRICK!" on a skater's third goal, which starts the home crowd animation for the home team), the scorer with his goals and the assists with theirs. |
| `PenaltyShotBox` | `data94.asm:PenaltyShotBox` | Penalty shot box (gmode2 bit 7): "PENALTY SHOT!", the shooter, the penalty and who committed it. |
| `ShowThreeStars` | `data94.asm:ShowThreeStars` | Game over, once the referee animation is far enough (RefCnt <= $40) and once only (gmode bit 7): the "Stars of the Game" box with the three best star scores, and the home win song. |
| `CalcStarScores` | `data94.asm:CalcStarScores` | The star score of every roster slot of both teams to ThreeStars (26 longs each, home first); goalies need d5 = period time + d2 on the ice. |
| `CalcTeamStarScores` | `data94.asm:CalcTeamStarScores` | Team a2 against a3: a long per roster slot to (a4)+, each from the goal difference. |
| `TakeTopStarScore` | `data94.asm:TakeTopStarScore` | Take the highest of the 52 star scores (and clear it) -> d0 = the player slot, a2 = his team. |
| `ConvertByteToDigits` | `data94.asm:ConvertByteToDigits` | d0 = BCD byte: two ASCII digits to (a1)+, a leading 0 as a space. |
| `FinalizeTextBuffer` | `data94.asm:FinalizeTextBuffer` | a1 = the end of the text after mesarea's length word: set the length (with a pad byte when odd) -> a1 = mesarea. |
| `format_name` | (a helper) | FormatPlayerName / FormatPlayerNumInitial: "NN " then the name from after the first space (the first name), optionally after the first initial and ". |
| `FormatLastName` | `data94.asm:FormatLastName` | Name string a0 (a length word String): from after the first space (the first name) to its end or a 0 byte, to a1, space padded to TextBuffer + 12 -> a1 = mesarea (FinalizeTextBuffer). |
| `FormatPlayerNameShort` | `data94.asm:FormatPlayerNameShort` | d0 = roster slot, a2 = team -> a1 = mesarea " Last". |
| `FormatPlayerNameLast` | `data94.asm:FormatPlayerNameLast` | d0 = roster slot, a2 = team -> a1 = mesarea "Last". |
| `getname` | `data94.asm:getname` | d0 = roster slot, a2 = team -> a1 = mesarea "NN First Last" (the number, then appendz / appstring). |
| `temp_player` | (a helper) | the player of TempPlOffset (bit 15 set: the away team) |
| `GetPlayerName` | `data94.asm:GetPlayerName` | -> a1 = mesarea "NN First Last" for player TempPlOffset. |
| `GetPlayerNumInitial` | `data94.asm:GetPlayerNumInitial` | -> a1 = mesarea "NN F. |
| `ShowInjuryBox` | `data94.asm:ShowInjuryBox` | "Injury to:" player TempPlOffset, "Out for period" ("Out for game" with sflags7 bit 5, "the game" after a fight). |
| `GetShifter` | `data94.asm:GetShifter` | -> d1 = games in a playoff round - 1, d2 = its first bit of WinBits (by gamelevel). |
| `ReadTeamStats` | `data94.asm:ReadTeamStats` | Unpack the $68 playoff stat totals of the bit stream (widths from BitWidthTable by stat & 3) into statsbuffer words. |
| `AddPlayoffTeamStats` | `data94.asm:AddPlayoffTeamStats` | In the playoffs: add the playoff team's game stats (the $68 bytes from team struct + $B4) to its unpacked totals and pack them back, each clamped to its bit width. |
| `DefaultMenus` | `data94.asm:DefaultMenus` | The options at power on (defmenuoptions: regular season, one player, Montreal at Los Angeles, 10 minute periods ...); no demo has run yet. |
| `SuperDiv` | `data94.asm:SuperDiv` | Divide the 5-word number at a3 by d0 in place -> the remainder. |
| `ReadPassBits` | `data94.asm:ReadPassBits` | The playoff state from the 5 saved words at a3 (kept): each game's series wins (bit 2 of its flags: a series won), WinBits, pojoy (1 / 2 become 4 without FourWayPlay), potreeteam, gamelevel, bosgames, postarts. |
| `GetRandomUnusedTeam` | `data94.asm:GetRandomUnusedTeam` | -> d0 = a random playoff team whose bit in d3 is clear; set it. |
| `InitializeGameStructures` | `data94.asm:InitializeGameStructures` | Random team pairs for the 8 gsstruct games, none of them HomeTeam or VisTeam. |
| `FigureJoy` | `data94.asm:FigureJoy` | The teams of the pads (cont1team ... |
| `maketree` | `data94.asm:maketree` | The playoff tree (potree): the first round's 16 teams from playoffseats row postarts, each later round's winners by WinBits; then this round's games: scores cleared, their teams (in tree order for series games 2, 3 and 5, else flipped), FigureJoy. |
| `NewPO` | `data94.asm:NewPO` | New playoffs: a random tree row (from the one after a random one) that holds Team 1 (the all-stars count as the last playoff team), series wins cleared unless best-of-one; falls into maketree. |
| `ContinuePlayoffs` | `data94.asm:ContinuePlayoffs` | The playoff state saved at pwddatabuffer (ReadPassBits); when a game has been played, the tree (maketree) and OptNOP from pojoy (7 or $B - pojoy, pojoy moved to the pad set-ups). |
| `add_long_carry` | (a helper) | the long at p (two words of a 5-word number) plus v, the carry run up through at most n words above it (dbcc: n counts down to -1) |
| `ClrPassBits` | `data94.asm:ClrPassBits` | Clear the 5 words at a3. |
| `SuperAdd` | `data94.asm:SuperAdd` | The 5-word number at a3 plus d0 (a long). |
| `SuperMult` | `data94.asm:SuperMult` | The 5-word number at a3 times d0 (a word): each word's product added in from the lowest (the highest's spills into the word below a3, which the game's numbers never reach). |
| `PushBits` | `data94.asm:PushBits` | bits = bits x d1 + d0 (d1 the value's range, 2-$4000). |
| `WritePassBits` | `data94.asm:WritePassBits` | The playoff state into the 5 words at a3 (as ReadPassBits reads it): postarts, bosgames, gamelevel, potreeteam, pojoy, WinBits, then each game's series wins. |
| `ResolveGames` | `data94.asm:ResolveGames` | After a playoff game: its score into its game struct, then this round's results. |
| `EncodePW` | `data94.asm:EncodePW` | After a playoff game (not Demo, Regular Season or a shootout): the results (ResolveGames), the tree, and the state saved (WritePassBits to pwddatabuffer, WriteLineData); the playoffs over (or the user's team out): cleared, and the mode back to new playoffs. |

### `game/system/dispatch.c` (3)

| Function | Original | What it does |
|---|---|---|
| `call68k` | (a helper) |  |
| `run68k` | (a helper) |  |
| `restart_at` | (a helper) |  |

### `game/system/fourway.c` (1)

| Function | Original | What it does |
|---|---|---|
| `Detect4WayPlay` | `fourway94.asm:Detect4WayPlay` | Look for the 4 Way Play adaptor on port 2 (FourWayPlay = 1 when found), with the Z80 held in reset. |

### `game/system/ram.c` (3)

| Function | Original | What it does |
|---|---|---|
| `ram_pack` | (a helper) |  |
| `ram_unpack` | (a helper) |  |
| `ram_bytes_fail_` | (a helper) | (game/mem.h's ram_bytes on what is not a byte buffer: a bug, the bytes would not be the machine's) |

### `game/system/rom.c` (3)

| Function | Original | What it does |
|---|---|---|
| `rom_load` | (a helper) |  |
| `rom_bad_address` | (a helper) | a ROM read past the cartridge (between its end and work RAM at $E00000): a bug in the port |
| `rom_find` | (a helper) | the ROM address of the first copy of bytes in [from, to), 0 if none: for data that sits inline after a call and has no label (a remap table, a raw String) |

### `game/system/rosters.c` (3)

| Function | Original | What it does |
|---|---|---|
| `rosters_bad_team` | (a helper) |  |
| `read_text` | (a helper) | the String at a (as it is: its length word, bytes and pad), on past it |
| `rosters_read` | (a helper) | each team's data (TeamList: its players, lines, texts, colours and the rest), its tables' entries, and the playoff draws |

### `game/system/sram.c` (8)

| Function | Original | What it does |
|---|---|---|
| `ReadSRAM` | `sram94.asm:ReadSRAM` | d0 = first save RAM byte, d1 = count, a0 = destination. |
| `WriteSRAM` | `sram94.asm:WriteSRAM` | d0 = first save RAM byte, d1 = count, a0 = source (each byte written as the low byte of a word). |
| `MakeSRAMChecksum` | `sram94.asm:MakeSRAMChecksum` | Read all of save RAM to M68K_RAM, put the byte sum of bytes 0-$1FFD in byte $1FFF and its complement in $1FFE, and write those two back. |
| `InitSaveRAM` | `sram94.asm:InitSaveRAM` | Copy save RAM to work RAM $0000-$1FFF and check it (ValidateSRAM); a bad one is cleared and read again. |
| `SRAMWaitVblank` | `sram94.asm:SRAMWaitVblank` | Wait for the next vblank. |
| `VBcount` | `sram94.asm:VBcount` | The vblank while InitSaveRAM runs: vcount + 1 only. |
| `ValidateSRAM` | `sram94.asm:ValidateSRAM` | Check the copy of save RAM at work RAM 0: byte $1FFF must be the sum of bytes 0-$1FFD and byte $1FFE its complement. |
| `ClearSRAM` | `sram94.asm:ClearSRAM` | Zero work RAM $0000-$1FFF but byte $1FFE = $FF (the checksum of zeros), write it to save RAM, then byte 0 = 1 to save RAM byte 1, then MakeSRAMChecksum. |

### `game/system/start.c` (2)

| Function | Original | What it does |
|---|---|---|
| `SegaInit` | (a helper) | sega/SegaInit.asm:SegaInit. |
| `Start` | `main94.asm:Start` | Power on: SegaInit, the checksum, then the game. |
