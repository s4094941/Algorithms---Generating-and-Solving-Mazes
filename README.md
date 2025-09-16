Team Member Allocations:
- Group Member 1: Maze Solving, E1 (Build Maze Without Flattening Terrain)
- Caiden da Costa (4094941) (GitHub user: s4094941): Maze Building & Cleaning World, E2 (Find Shortest Path)
- Group Member 2: Maze Generation, E3 (Validate User Input Maze)

Compiling and Running:
- Run ```make && ./mazeRunner``` in a unix terminal

Runing in test mode
- Run ```./mazeRunner -testmode``` in a unix terminal

Assumptions:

Solving Manually:
- Solving manually can only occur after a maze has been generated and built.
- The random location within a maze is chosen via the set of all air blocks in one y-coord level within a maze, rather than the set of all blocks within a maze in one y-coord level. Meaning the algorithm does not just loop naively until a random air block is found.

Placing the Maze:
- Minecraft blocks that are unaccessible by the mcpp library will not be in the terrain upon building.
- Blocks that occupy more than one coordinate will not be in the terrain.

Show Escape Route:
- Showing the escape route can only occur after a maze has been generated and built.
- The player has to be inside the maze and the blue carpet marking the exit is one block out of the maze (where an edge of a maze could go).
- Also assume that the lime carpet can be oriented in any way, such that the 'player's right hand' is facing a wall.

Maze Generation:
- Row and column must both be positive odd integers. If a value below 3 is passed, it will be converted to 3.
- Any even values will have 1 added to them such that they are odd.

A maze is built up of wall nodes, path nodes, and path connection nodes.
- Only one exit node can exist, which is a path node on the outer edge of the maze.
- By default, path nodes can only be on [odd][odd] positions (eg. maze[1][1], which is technically the 2nd row and column).
    * As such, any isolated wall nodes encapsulated by a loop must have an [even][even] node within the cluster.
- In non-user-generated modes, [odd][odd] nodes will always be path nodes, and [even][even] nodes will always be wall nodes.

In user-generated modes:
- [odd][odd] nodes may be walls, but [even][even] nodes must still be walls.
- Maze edges must be all walls, with only one exit node. Any additional exit nodes will be converted to walls.
- Exit points must be connected to at least one path node.
- All corners must be filled in with walls.
- The main path stemming from the exit point must follow regular maze conventions.
        
Enhancement 1:
- The obstacles/unflattened terrain cannot obstruct all the maze border blocks as well as the starting point (from recursive backtracking).
- The obstacles are also represented by 'x's within the maze like the maze walls. Also any 2 or more blocks deeper than the basepoint areas are filled in as those areas can be inaccessible.
- Assuming the basepoint acts as the 'leveled block' (from demo video on canvas), means if there is terrain that is 2 or more blocks deeper than a terrain that is 1 or more blocks higher than the basepoint, possible unavoidable inaccessiblities can occur.

Enhancement 2:
- No walls will be broken whilst path is being calculated
- The solve function will only be called once to find the exit

Enhancement 3:
- If multiple entry points detected, only first one will count. Any additionals will be turned to wall.
- If isolated wall is found in a node connection (odd x even or even x odd), it will correct itself to an odd x odd node
- If multiple walls are between an isolated path node and the main path, the program will randomly tunnel through walls until they connect

Enhancements:
- Are available through the main menu options in this repository.
