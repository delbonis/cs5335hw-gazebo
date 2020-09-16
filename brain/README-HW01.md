**Name:** James "Trey" Del Bonis
**Repo:** https://github.com/delbonis/cs5335hw-gazebo
**Demo:** https://tr3y.io/media/cs4610-a1.webm
**Comments:**

I probably should have commented this code some more, but the idea is that
there's two main modes of operation.

The default mode if there's no nearby obstacles between the robot and the goal
is to just move forward while turning towards the goal.  If the angle turned away from the goal is higher then it turns much harder.

But if there's obstacles nearby between the robot and the goal then we switch 
into an active avoidance behavior.  It's primitive, so there's probably some
pathological situations, but it should eventually work itself out of them given
enough time.  It looks at each raycast hit and depending on where the hit is
and how close it is to the robot and computes a sum of these "jiggles" to try
to turn away.  The size of the jiggle is inversely proportional to the
distance of each raycast hit, so a nearby obstacle will give a few strong
jiggles.

Unfortunately I know there's an oversight here though.  If the robot is
approacing an obstacle exactly perpendicularly then the jiggles can all cancel
out and it'll just run into the wall.  I could have fixed this by adding some
little extra nondeterminism or having a "if we're really close then always turn
right" case to get out of this but that's more work than it was worth.

